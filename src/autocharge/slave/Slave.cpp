#include "Slave.hpp"
#include "Communication.h"

void Slave::step() {
  switch (this->state) {
  case SlaveState::WORK:
    this->stepWork(this->master);
    break;
  case SlaveState::WALKING_TO_CHARGE:
    const bool arrived = this->stepToCharge(this->master);
    if (arrived) {
      this->notifyInWait();
      this->state = SlaveState::WAIT_CHARGE;
    }
    break;
  case SlaveState::WAIT_CHARGE:
    this->stepWaitCharge(this->master);
    break;
  case SlaveState::WALKING_INTO_CHARGE:
    const bool entered = this->stepIntoCharge(this->master);
    if (entered) {
      this->notifyInCharge();
      this->state = SlaveState::CHARGE;
    }
    break;
  case SlaveState::CHARGE:
    this->stepCharge(this->master);
    break;
  case SlaveState::EXITING_CHARGE:
    const bool exited = this->stepExitCharge(this->master);
    if (exited) {
      this->notifyWork();
      this->state = SlaveState::WORK;
    }
    break;
  }
}

void Slave::requestCharge() {
    this->communication.unicast(this->master.id, "requestCharge");
}

void Slave::requestStopCharge() {
    this->communication.unicast(this->master.id, "stopCharge");
}

void Slave::notifyWork() {
    this->communication.unicast(this->master.id, "notifyWork");
}

void Slave::notifyWalkToCharge() {
    this->communication.unicast(this->master.id, "notifyWalkToCharge");
}

void Slave::notifyInWait() {
    this->communication.unicast(this->master.id, "notifyInWait");
}

void Slave::notifyWalkIntoCharge() {
    this->communication.unicast(this->master.id, "notifyWalkIntoCharge");
}

void Slave::notifyInCharge() {
    this->communication.unicast(this->master.id, "notifyInCharge");
}

void Slave::notifyExitCharge() {
    this->communication.unicast(this->master.id, "notifyExitCharge");
}

void Slave::handleEnjoinChargeCommand() {
  switch (this->state) {
  case SlaveState::WORK:
    this->notifyWalkIntoCharge();
    this->state = SlaveState::WALKING_INTO_CHARGE;
    break;
  case SlaveState::WALKING_TO_CHARGE:
    this->notifyWalkIntoCharge();
    this->state = SlaveState::WALKING_INTO_CHARGE;
    break;
  case SlaveState::WAIT_CHARGE:
    this->notifyWalkIntoCharge();
    this->state = SlaveState::WALKING_INTO_CHARGE;
    break;
  case SlaveState::WALKING_INTO_CHARGE:
    this->notifyWalkIntoCharge();
    break;
  case SlaveState::CHARGE:
    this->notifyInCharge();
    break;
  case SlaveState::EXITING_CHARGE:
    this->notifyWalkIntoCharge();
    this->state = SlaveState::WALKING_INTO_CHARGE;
    break;
  }
}

void Slave::handleCancelChargeCommand() {
  switch (this->state) {
  case SlaveState::WORK:
    this->notifyWork();
    break;
  case SlaveState::WALKING_TO_CHARGE:
    this->notifyWork();
    this->state = SlaveState::WORK;
    break;
  case SlaveState::WAIT_CHARGE:
    this->notifyWork();
    this->state = SlaveState::WORK;
    break;
  case SlaveState::WALKING_INTO_CHARGE:
    this->notifyWork();
    this->state = SlaveState::WORK;
    break;
  case SlaveState::CHARGE:
    this->notifyExitCharge();
    this->state = SlaveState::EXITING_CHARGE;
    break;
  case SlaveState::EXITING_CHARGE:
    break;
  }
}

void Slave::handleRequestChargeResponse(bool approved) {
  if (approved) {
    this->notifyWalkToCharge();
    this->state = SlaveState::WALKING_TO_CHARGE;
  }
}

void Slave::handleRequestStopChargeResponse(bool approved) {
  if (approved) {
    this->notifyExitCharge();
    this->state = SlaveState::EXITING_CHARGE;
  }
}
