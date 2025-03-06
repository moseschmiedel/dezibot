#pragma once
#include "../../Dezibot.h"
#include "MasterData.hpp"
#include "SlaveState.hpp"
#include <functional>

#ifndef SLAVE_H
#define SLAVE_H

class Slave : public Dezibot {
public:
  Slave(SlaveState state,
        const std::function<void()> findChargingStation,
        const std::function<void()> enterChargingStation,
        const std::function<void()> exitChargingStation)
      : state(state),
        findChargingStation(findChargingStation),
        enterChargingStation(enterChargingStation),
        exitChargingStation(exitChargingStation) {}

  void requestCharge(MasterData masterData);
  void notifyWalkToCharge1(MasterData masterData);
  void notifyInWait(MasterData masterData);
  void notifyWalkToCharge2(MasterData masterData);
  void notifyInCharge(MasterData masterData);
  void requestStopCharge(MasterData masterData);

private:
  SlaveState state;
  const std::function<void()> findChargingStation;
  const std::function<void()> enterChargingStation;
  const std::function<void()> exitChargingStation;

  void handleRequestChargeResponse(MasterData master);
  void handleEnjoinChargeCommand(MasterData master);
  void handleCancelChargeCommand(MasterData master);
  void handleRequestStopChargeResponse(MasterData master);
};

#endif // SLAVE_H
