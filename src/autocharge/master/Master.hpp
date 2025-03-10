#pragma once
#include "../../Dezibot.h"
#include "ChargingStationState.hpp"
#include "SlaveData.hpp"
#include "abstractSet/AbstractSet.hpp"
#include <functional>

#ifndef MASTER_H
#define MASTER_H

class Master : public Dezibot {
public:
  Master(AbstractSet<SlaveData> &registered_slaves,
         ChargingStationState state,
         SlaveData *charging_slave,
         const std::function<void(SlaveData &slave)> &handle_slave_charge_request,
         const std::function<void(SlaveData &slave)> &handle_slave_stop_charge_request)
      : registeredSlaves(registered_slaves),
        chargingStationState(state),
        chargingSlave(charging_slave),
        handleSlaveChargeRequest(handle_slave_charge_request),
        handleSlaveStopChargeRequest(handle_slave_stop_charge_request) {}

  void step();
  void enjoinCharge(SlaveData &slave);
  void cancelCharge(SlaveData &slave);

private:
  AbstractSet<SlaveData> &registeredSlaves;
  ChargingStationState chargingStationState;
  SlaveData *chargingSlave;
  const std::function<void(SlaveData &slave)> handleSlaveChargeRequest;
  const std::function<void(SlaveData &slave)> handleSlaveStopChargeRequest;

  bool stepLowerGear();
  bool stepLiftGear();
  void stepClosed();
  bool stepAttachGear();
  void stepSlaveCharge();

  void handleWorkInfo(SlaveData &slave);
  void handleWalkToChargeInfo(SlaveData &slave);
  void handleInWaitInfo(SlaveData &slave);
  void handleWalkIntoChargeInfo(SlaveData &slave);
  void handleInChargeInfo(SlaveData &slave);
  void handleExitChargeInfo(SlaveData &slave);
};

#endif // MASTER_H
