#pragma once

#include "../../Dezibot.h"
#include "ChargingStationState.hpp"
#include "SlaveData.hpp"
#include "abstractSet/AbstractSet.hpp"
#include <functional>

class Master : Dezibot {
public:
  Master(
      AbstractSet<SlaveData> &registered_slaves,
      ChargingStationState state, SlaveData *charging_slave,
      const std::function<void(SlaveData &slave)> &handle_slave_charge_request,
      const std::function<void(SlaveData &slave)>
          &handle_slave_stop_charge_request)
      : registeredSlaves(registered_slaves), chargingStationState(state),
        chargingSlave(charging_slave),
        handleSlaveChargeRequest(handle_slave_charge_request),
        handleSlaveStopChargeRequest(handle_slave_stop_charge_request) {
    Master::master = this;
  }

  static Master *master;

  void begin() override;
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

  static void onReceiveSingle(uint32_t from, String &message) {
    Serial.printf("Received single from Node(%u): %s", from, message.c_str());
    SlaveData *slave = master->registeredSlaves.get(from);
    if (slave == nullptr) {
      return;
    } else {
      if (message == "requestCharge") {
        master->enjoinCharge(*slave);
      } else if (message == "stopCharge") {
        master->cancelCharge(*slave);
      } else if (message == "notifyWork") {
        slave->state = SlaveState::WORK;
      } else if (message == "notifyWalkToCharge") {
        slave->state = SlaveState::WALKING_TO_CHARGE;
      } else if (message == "notifyWalkInWait") {
        slave->state = SlaveState::WAIT_CHARGE;
      } else if (message == "notifyWalkIntoCharge") {
        slave->state = SlaveState::WALKING_INTO_CHARGE;
      } else if (message == "notifyInCharge") {
        slave->state = SlaveState::CHARGE;
      } else if (message == "notifyExitCharge") {
        slave->state = SlaveState::EXITING_CHARGE;
      }
    }
  }
};
