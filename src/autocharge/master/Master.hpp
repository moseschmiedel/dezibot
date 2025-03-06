#pragma once
#include "../../Dezibot.h"
#include "ChargingStationState.hpp"
#include "SlaveData.hpp"
#include "abstractSet/AbstractSet.hpp"
#include <optional>

#ifndef MASTER_H
#define MASTER_H

class Master : public Dezibot {
public:
  Master(AbstractSet<SlaveData> &registered_slaves,
         ChargingStationState state,
         std::optional<SlaveData> &charge_slave)
       : registeredSlaves(registered_slaves), chargingStationState(state),
         chargeSlave(charge_slave) {}

  // command a slave to go into the charging station
  void enjoinCharge(SlaveData slave);

  // command a slave to cancel charging and return to work
  void cancelCharge(SlaveData slave);

private:
  AbstractSet<SlaveData> &registeredSlaves;
  ChargingStationState chargingStationState;
  // the slave currently charging or about to charge (enjoined)
  std::optional<SlaveData> &chargeSlave;

  void handleChargeRequest(SlaveData slave);
  void handleWalkToCharge1Info(SlaveData slave);
  void handleInWaitInfo(SlaveData slave);
  void handleWalkToCharge2Info(SlaveData slave);
  void handleInChargeInfo(SlaveData slave);
  void handleStopChargeRequest(SlaveData slave);
};

#endif // MASTER_H
