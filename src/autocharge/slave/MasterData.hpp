#ifndef MASTERDATA_H
#define MASTERDATA_H

#include <string>

struct MasterData {
  std::string id;

  MasterData(std::string _id) : id(_id) {}
};

#endif // MASTERDATA_H
