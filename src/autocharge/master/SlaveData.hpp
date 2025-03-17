#ifndef SLAVEDATA_H
#define SLAVEDATA_H

#include "../slave/SlaveState.hpp"
#include <cstdint>

class SlaveData {
public:
  const uint32_t id;
  SlaveState state;

  SlaveData(const uint32_t& _id, SlaveState _state) : id(_id), state(_state) {}
};

#endif // SLAVEDATA_H
