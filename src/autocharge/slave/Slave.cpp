#include "Slave.hpp"

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

void Slave::requestCharge() {}

void Slave::requestStopCharge() {}

void Slave::notifyWork() {}

void Slave::notifyWalkToCharge() {}

void Slave::notifyInWait() {}

void Slave::notifyWalkIntoCharge() {}

void Slave::notifyInCharge() {}

void Slave::notifyExitCharge() {}

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
