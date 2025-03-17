#ifndef SLAVEDATA_H
#define SLAVEDATA_H

#include "../slave/SlaveState.hpp"
#include <cstdint>

class SlaveData {
public:
  const uint32_t id;
  SlaveState state;
};

#endif // SLAVEDATA_H
