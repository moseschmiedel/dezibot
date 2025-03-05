#pragma once
#include "../../Dezibot.h"
#include "SlaveState.hpp"
#include <functional>

#ifndef SLAVE_H
#define SLAVE_H

class Slave : public Dezibot {
public:
  Slave(const SlaveState state, const std::function<void()> findChargingStation,
        const std::function<void()> enterChargingStation,
        const std::function<void()> exitChargingStation)
      : state(state), findChargingStation(findChargingStation),
        enterChargingStation(enterChargingStation),
        exitChargingStation(exitChargingStation) {}

private:
  SlaveState state;
  const std::function<void()> findChargingStation;
  const std::function<void()> enterChargingStation;
  const std::function<void()> exitChargingStation;
};

#endif // SLAVE_H
