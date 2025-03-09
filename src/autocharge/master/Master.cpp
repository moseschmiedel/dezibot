#include "Master.hpp"
#include "SlaveData.hpp"

void Master::step() {
  switch (this->chargingStationState) {
  case ChargingStationState::OPEN: {
    if (this->chargingSlave != nullptr) {
      SlaveData *next = this->registeredSlaves.pick();
      if (next != nullptr) {
        this->chargingSlave = next;
        this->enjoinCharge(*next);
      }
    }
    break;
  }
  case ChargingStationState::LOWERING_GEAR: {
    const bool lowered = this->stepLowerGear();
    if (lowered) {
      this->chargingStationState = ChargingStationState::CLOSED;
    }
    break;
  }
  case ChargingStationState::CLOSED:
    this->stepClosed();
    break;
  case ChargingStationState::SLAVE_CHARGE:
    this->stepSlaveCharge();
    break;
  case ChargingStationState::LIFTING_GEAR: {
    const bool lifted = this->stepLiftGear();
    if (lifted) {
      this->chargingStationState = ChargingStationState::OPEN;
    }
    break;
  }
  }
}

void Master::enjoinCharge(SlaveData &slave) {}

void Master::cancelCharge(SlaveData &slave) {}

bool Master::stepLowerGear() {}

bool Master::stepLiftGear() {}

bool Master::stepClosed() {}

bool Master::stepSlaveCharge() {}

void Master::handleWorkInfo(SlaveData &slave) {
    slave.state = SlaveState::WORK;
}

void Master::handleWalkToChargeInfo(SlaveData &slave) {
    slave.state = SlaveState::WALKING_TO_CHARGE;
}

void Master::handleInWaitInfo(SlaveData &slave) {
    slave.state = SlaveState::WAIT_CHARGE;
}

void Master::handleWalkIntoChargeInfo(SlaveData &slave) {
    slave.state = SlaveState::WALKING_INTO_CHARGE;
}

void Master::handleInChargeInfo(SlaveData &slave) {
    slave.state = SlaveState::CHARGE;
}

void Master::handleExitChargeInfo(SlaveData &slave) {
    slave.state = SlaveState::EXITING_CHARGE;
}
