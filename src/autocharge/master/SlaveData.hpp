#ifndef SLAVEDATA_H
#define SLAVEDATA_H

#include "../slave/SlaveState.hpp"
#include <string>

class SlaveData {
public:
  const String id;
  SlaveState state;

  SlaveData(const String& _id, SlaveState _state) : id(_id), state(_state) {}
};

#endif // SLAVEDATA_H
