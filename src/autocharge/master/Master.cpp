#include "Master.hpp"
#include "SlaveData.hpp"

Master *Master::master = nullptr;

void Master::begin(void) {
  this->Dezibot::begin();
  this->communication.begin();
  this->communication.onReceiveSingle(&onReceiveSingle);
}

void Master::step() {
  switch (this->chargingStationState) {
  case ChargingStationState::OPEN: {
    if (this->currentChargingSlave != nullptr) {
      SlaveData *next = *this->chargingSlaves.pick();
      if (next != nullptr) {
        this->currentChargingSlave = next;
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
  case ChargingStationState::ATTACHING_GEAR: {
    const bool attached = this->stepAttachGear();
    if (attached) {
      this->chargingStationState = ChargingStationState::SLAVE_CHARGE;
    }
    break;
  }
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

void Master::enjoinCharge(SlaveData &slave) {
  this->communication.unicast(slave.id, "enjoinCharge");
  Serial.printf("Commanded slave(%u) to charge\n", slave.id);
}

void Master::cancelCharge(SlaveData &slave) {
  this->communication.unicast(slave.id, "cancelCharge");
  Serial.printf("Commanded slave(%u) to cancel charge\n", slave.id);
}

bool Master::stepLowerGear() {}

bool Master::stepLiftGear() {}

void Master::stepClosed() {}

bool Master::stepAttachGear() {}

void Master::stepSlaveCharge() {}

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
