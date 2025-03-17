#pragma once
#include "../../Dezibot.h"
#include "MasterData.hpp"
#include "SlaveState.hpp"
#include <functional>

#ifndef SLAVE_H
#define SLAVE_H

class Slave : public Dezibot {
public:
  Slave(SlaveState state, const std::function<void()> stepWork,
        const std::function<bool(MasterData master)> stepToCharge,
        const std::function<void(MasterData master)> stepWaitCharge,
        const std::function<bool(MasterData master)> stepIntoCharge,
        const std::function<void(MasterData master)> stepCharge,
        const std::function<bool(MasterData master)> stepExitCharge)
      : state(state), stepWork(stepWork), stepToCharge(stepToCharge),
        stepWaitCharge(stepWaitCharge), stepIntoCharge(stepIntoCharge),
        stepCharge(stepCharge), stepExitCharge(stepExitCharge) {
    Slave::slave = this;
  }

  static Slave *slave;

  void begin() override;
  void step();
  void requestCharge();
  void requestStopCharge();

private:
  SlaveState state;
  MasterData master;
  const std::function<void()> stepWork;
  const std::function<bool(MasterData master)> stepToCharge;
  const std::function<void(MasterData master)> stepWaitCharge;
  const std::function<bool(MasterData master)> stepIntoCharge;
  const std::function<void(MasterData master)> stepCharge;
  const std::function<bool(MasterData master)> stepExitCharge;

  void notifyWork();
  void notifyWalkToCharge();
  void notifyInWait();
  void notifyWalkIntoCharge();
  void notifyInCharge();
  void notifyExitCharge();

  void handleEnjoinChargeCommand();
  void handleCancelChargeCommand();

  static void onReceiveSingle(uint32_t from, String &message) {
    Serial.printf("Received single from Node(%u): %s", from, message.c_str());
    if (from == slave->master.id) {
      if (message == "enjoinCharge") {
        slave->handleEnjoinChargeCommand();
      } else if (message == "cancelCharge") {
        slave->handleCancelChargeCommand();
      }
    }
  }
};

#endif // SLAVE_H
