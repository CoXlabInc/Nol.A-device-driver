# VL53L5CX

ST VL53L5CX ULD 2.0.0을 Nol.A `TwoWire`에 연결한 드라이버입니다.
원본: `x-cube-tof1/Drivers/BSP/Components/vl53l5cx/modules`.
라이선스: `inc/VL53L5CX/LICENSE.md`. STM32Cube BSP/HAL 의존성은 없습니다.

```cpp
#include <cox.h>
#include <VL53L5CX.hpp>

static VL53L5CX sensor(Wire); // 7비트 주소 0x29
static VL53L5CX_ResultsData results;

// setup() 안에서 각 호출의 ST 상태 코드(0=성공)를 확인합니다.
// sensor.begin();
// sensor.setResolution(VL53L5CX_RESOLUTION_8X8);
// sensor.setRangingMode(VL53L5CX::RangingMode::Autonomous);
// sensor.setRangingFrequencyHz(10);
// sensor.setIntegrationTimeMs(20);
// sensor.startRanging();
```

실행 가능한 예제는 코어 저장소의 `apps/st-nucleo-l452re/`에 있습니다.
`begin()`은 연결된 TwoWire를 초기화하고 장치 확인과 센서 펌웨어 다운로드를
수행합니다. 기본은 Continuous입니다. 생성자의 주소는 현재 센서 주소이며,
센서 주소를 변경하려면 초기화 후 `setAddress()`를 사용합니다.

측정 중 설정은 `stopRanging()` → 설정 → `startRanging()` 순서입니다.
`setRangingMode()`/`getRangingMode()`는 Continuous와 Autonomous를 지원하고,
`setIntegrationTimeMs()`/`getIntegrationTimeMs()`는 적분 시간을 설정/조회합니다.
Continuous에서는 센서가 최대 적분 시간으로 동작하며, 사용자 적분 시간은
Autonomous에서 적용됩니다. 주파수·해상도·적분 시간의 제약은 ST ULD와 같습니다.

`checkDataReady(bool&)`는 상태 코드와 준비 여부를 분리합니다. 준비된 경우에만
`getRangingData(results)`를 호출하십시오. 기본 존당 타깃은 1개이며 거리와
타깃 상태는 ST 결과 구조체 그대로 반환합니다. 타깃 상태의 유효성을 확인한
뒤 거리를 사용하십시오.

누화 보정, 임계값, 모션 API는 각 플러그인 헤더와 `configuration()`으로
사용합니다. 원본 API를 직접 호출하면 래퍼의 측정 상태 추적과 일치하도록
호출자가 순서를 관리해야 합니다. 동일 버스를 사용하는 호출은 직렬화해야
하며 ISR에서 호출하지 않습니다. I2C 클록은 호출자가 `Wire.setClock()`으로
선택하며 공유 버스에 대한 래퍼 자체의 클록 변경은 없습니다.

출력 필드/타깃 수 매크로를 변경할 때는 드라이버 라이브러리와 애플리케이션을
같은 매크로로 재빌드해야 합니다. 애플리케이션에서만 변경하면 구조체 ABI가
맞지 않습니다. 펌웨어 약 84 KiB가 Flash에 포함되며 상태·결과는 정적 저장소에
할당하는 것이 좋습니다.

## 전송 복구와 상태 확인

읽기는 레지스터 포인터를 다시 설정해 최대 3회 시도합니다. 펌웨어 다운로드의
RAM 쓰기만 같은 조각을 최대 3회 시도하며, 재시도 전에 뱅크를 복원합니다.
측정 시작/중지·주소 변경 등 일반 제어 명령 쓰기는 재전송하지 않습니다.
시도를 소진하면 MCU 오류를 반환합니다. 최악 지연은 버스 timeout과 시도 횟수에
의존하므로 예제는 `Wire.timeout = 100` ms를 사용합니다.

`getTransportRetryCount()`로 객체 생성 이후 누적 재시도 수를 조회할 수 있습니다.
세부 값은 `configuration()->platform.readRetries`와 `firmwareWriteRetries`입니다.
반복 오류나 높은 재시도 수가 관측되면 전원·배선·풀업과 버스 타이밍을 확인하십시오.

STM32L4에서는 초기화 전에 `Wire.begin(); Wire.setClock(400000);`을 호출하여
센서 펌웨어 다운로드부터 같은 속도를 사용할 수 있습니다. 해당 보드의
`begin()`은 이미 시작된 버스 설정을 유지합니다. L476RG SATEL 예제는 PWR_EN,
LPn, I2C_RST 초기화도 포함합니다. 보드 전원 제어는 센서 라이브러리에서 수행하지
않습니다.

## 호스트 전송 테스트

저장소 루트에서 아래 명령으로 32바이트 버퍼 경계, 32 KiB 전송, 주소 증가,
NACK/짧은 수신/쓰기 실패, 제한된 재시도와 펌웨어 뱅크 복원, 제어 명령 미재전송,
endian 변환과 긴 지연 분할을 검증합니다.
실제 MCU·센서 동작 검증을 대체하지 않습니다.

```sh
c++ -std=c++14 -Wall -Wextra -Werror \
  -Idev/tests/vl53l5cx/stubs -Idev/inc \
  dev/src/VL53L5CX/vl53l5cx_platform.cpp \
  dev/tests/vl53l5cx/platform_test.cpp -o /tmp/vl53l5cx-platform-test
/tmp/vl53l5cx-platform-test
```
