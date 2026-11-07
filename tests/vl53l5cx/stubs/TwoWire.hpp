#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <algorithm>

// Host fake of the SDK's 32-byte TwoWire transport, including short/error paths.
class TwoWire {
public:
  enum MasterWriteError_t { SUCCESS, DATA_TOO_LONG, NACK_ON_ADDRESS, NACK_ON_DATA, OTHER_ERROR };
  struct Transaction { uint8_t address; std::vector<uint8_t> data; bool stop; };
  std::vector<Transaction> transactions;
  std::vector<size_t> reads;
  uint8_t address = 0;
  std::vector<uint8_t> tx;
  size_t pending = 0, cursor = 0;
  unsigned nackRemaining = 0, shortReadsRemaining = 0;
  bool nack = false, shortRead = false, failWrite = false, failRead = false;
  void beginTransmission(uint8_t value) { address = value; tx.clear(); }
  uint8_t write(uint8_t value) {
    if (failWrite || tx.size() == 32) return 0;
    tx.push_back(value); return 1;
  }
  uint8_t write(const uint8_t *data, uint8_t size) {
    uint8_t written = 0;
    for (uint8_t i = 0; i < size; ++i) written += write(data[i]);
    return written;
  }
  MasterWriteError_t endTransmission(bool stop = true) {
    transactions.push_back({address, tx, stop});
    if (nackRemaining) { --nackRemaining; return NACK_ON_ADDRESS; }
    return nack ? NACK_ON_ADDRESS : SUCCESS;
  }
  size_t requestFrom(uint8_t value, size_t size, bool = true) {
    address = value; reads.push_back(size); pending = (shortRead || shortReadsRemaining) ? size - 1 : size;
    if (shortReadsRemaining) --shortReadsRemaining;
    return pending;
  }
  int read() { return failRead || !pending-- ? -1 : int(cursor++ & 255); }
};
