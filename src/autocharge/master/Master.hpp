#pragma once

#include "../../Dezibot.h"
#include "ChargingStationState.hpp"
#include "SlaveData.hpp"
#include "abstractSet/AbstractSet.hpp"
#include <functional>
#include <unordered_map>

class Master : Dezibot {
public:
  Master(
      AbstractSet<SlaveData *> &chargingSlaves,
      const std::function<void(Master* master, SlaveData &slave)> &handle_slave_charge_request,
      const std::function<void(Master* master, SlaveData &slave)> &handle_slave_stop_charge_request)
      : chargingSlaves(chargingSlaves),
        handleSlaveChargeRequest(handle_slave_charge_request),
        handleSlaveStopChargeRequest(handle_slave_stop_charge_request) {
    master = this;
  }

  void begin() override;
  void step();
  void enjoinCharge(SlaveData &slave);
  void cancelCharge(SlaveData &slave);

protected:
  static Master *master;

private:
  std::unordered_map<uint32_t, SlaveData *> *registeredSlaves =
      new std::unordered_map<uint32_t, SlaveData *>();
  AbstractSet<SlaveData *> &chargingSlaves;
  ChargingStationState chargingStationState = ChargingStationState::OPEN;
  SlaveData *currentChargingSlave = nullptr;
  const std::function<void(Master* master, SlaveData &slave)> handleSlaveChargeRequest;
  const std::function<void(Master* master, SlaveData &slave)> handleSlaveStopChargeRequest;

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
    SlaveData *slave = master->registeredSlaves->at(from);
    if (slave == nullptr) {
      slave = new SlaveData(from);
      master->registeredSlaves->insert({from, slave});
    }

    if (message == "requestCharge") {
      master->handleSlaveChargeRequest(master, *slave);
    } else if (message == "stopCharge") {
      master->handleSlaveStopChargeRequest(master, *slave);
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
};
