#pragma once
#include "../../Dezibot.h"
#include "ChargingStationState.hpp"
#include "SlaveData.hpp"
#include "abstractSet/AbstractSet.hpp"

#ifndef MASTER_H
#define MASTER_H

class Master : public Dezibot {
public:
  Master(const AbstractSet<SlaveData> &registered_slaves,
         const ChargingStationState state)
      : registeredSlaves(registered_slaves), chargingStationState(state) {}

private:
  const AbstractSet<SlaveData> &registeredSlaves;
  ChargingStationState chargingStationState;
};
;

#endif // MASTER_H
