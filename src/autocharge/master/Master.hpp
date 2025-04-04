#pragma once

#include "../../Dezibot.h"
#include "ChargingStationState.hpp"
#include "SlaveData.hpp"
#include "abstractSet/AbstractSet.hpp"
#include <functional>
#include <unordered_map>

class Master : public Dezibot {
public:
  Master(
      AbstractSet<SlaveData *> &chargingSlaves,
      const std::function<void(Master* master, SlaveData *slave)> &handle_slave_charge_request,
      const std::function<void(Master* master, SlaveData *slave)> &handle_slave_stop_charge_request)
      : chargingSlaves(chargingSlaves),
        handleSlaveChargeRequest(handle_slave_charge_request),
        handleSlaveStopChargeRequest(handle_slave_stop_charge_request) {
    master = this;
  }

  void begin() override;
  void step();
  void enjoinCharge(SlaveData *slave);
  void cancelCharge(SlaveData *slave);

protected:
  static Master *master;

private:
  std::unordered_map<uint32_t, SlaveData *> *registeredSlaves =
      new std::unordered_map<uint32_t, SlaveData *>();
  AbstractSet<SlaveData *> &chargingSlaves;
  ChargingStationState chargingStationState = ChargingStationState::OPEN;
  SlaveData *currentChargingSlave = nullptr;
  const std::function<void(Master* master, SlaveData *slave)> handleSlaveChargeRequest;
  const std::function<void(Master* master, SlaveData *slave)> handleSlaveStopChargeRequest;

  void stepOpen();
  bool stepLowerGear();
  bool stepLiftGear();
  void stepClosed();
  bool stepAttachGear();
  void stepSlaveCharge();

  void handleWorkInfo(SlaveData *slave);
  void handleWalkToChargeInfo(SlaveData *slave);
  void handleInWaitInfo(SlaveData *slave);
  void handleWalkIntoChargeInfo(SlaveData *slave);
  void handleInChargeInfo(SlaveData *slave);
  void handleExitChargeInfo(SlaveData *slave);

  static void onReceiveSingle(uint32_t from, String &message) {
    Serial.printf("Received single from Node(%u): %s\n", from, message.c_str());

    auto it = master->registeredSlaves->find(from);
    SlaveData *slave = (it != master->registeredSlaves->end()) ? it->second : nullptr;
    if (slave == nullptr) {
      slave = new SlaveData(from);
      master->registeredSlaves->insert({from, slave});
    }

    // SlaveData *slave = new SlaveData(4200495964, SlaveState::WORK);

    if (message.equals("requestCharge")) {
      master->handleSlaveChargeRequest(master, slave);
    } else if (message.equals("stopCharge")) {
      master->chargingStationState = ChargingStationState::LIFTING_GEAR;
    } else if (message.equals("notifyWork")) {
      master->handleWorkInfo(slave);
    } else if (message.equals("notifyWalkToCharge")) {
        master->handleWalkToChargeInfo(slave);
    } else if (message.equals("notifyInWait")) {
      master->handleInWaitInfo(slave);
    } else if (message.equals("notifyWalkIntoCharge")) {
      master->handleWalkIntoChargeInfo(slave);
    } else if (message.equals("notifyInCharge")) {
      master->handleInChargeInfo(slave);
    } else if (message.equals("notifyExitCharge")) {
      master->handleExitChargeInfo(slave);
    } else {
        Serial.printf("Unknown message from Node(%u): %s\n", from, message.c_str());
    }
  }
};
