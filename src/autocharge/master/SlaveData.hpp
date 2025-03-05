#ifndef SLAVEDATA_H
#define SLAVEDATA_H

#include <string>

struct SlaveData {
  std::string id;

  SlaveData(std::string _id) : id(_id) {}
};

#endif // SLAVEDATA_H
