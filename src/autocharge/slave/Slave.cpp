#include "Slave.hpp"
#include "communication/Communication.h"

Slave *Slave::slave = nullptr;

void Slave::begin(void) {
  this->Dezibot::begin();
  this->communication.begin();
  this->communication.onReceiveSingle(&onReceiveSingle);
}

void Slave::step() {
  switch (this->state) {
  case SlaveState::WORK: {
    this->stepWork(this);
    break;
  }
  case SlaveState::WALKING_TO_CHARGE: {
    const bool arrived = this->stepToCharge(this->master);
    if (arrived) {
      this->notifyInWait();
      this->state = SlaveState::WAIT_CHARGE;
    }
    break;
  }
  case SlaveState::WAIT_CHARGE: {
    this->stepWaitCharge(this->master);
    break;
  }
  case SlaveState::WALKING_INTO_CHARGE: {
    const bool entered = this->stepIntoCharge(this->master);
    if (entered) {
      this->notifyInCharge();
      this->state = SlaveState::CHARGE;
    }
    break;
  }
  case SlaveState::CHARGE: {
    this->stepCharge(this->master);
    break;
  }
  case SlaveState::EXITING_CHARGE: {
    const bool exited = this->stepExitCharge(this->master);
    if (exited) {
      this->notifyWork();
      this->state = SlaveState::WORK;
    }
    break;
  }
  }
}

void Slave::requestCharge() {
  this->communication.unicast(this->master.id, "requestCharge");
  Serial.printf("Requested master(%u) to charge", this->master.id);
}

void Slave::requestStopCharge() {
  this->communication.unicast(this->master.id, "stopCharge");
  Serial.printf("Requested master(%u) to stop charging", this->master.id);
}

void Slave::notifyWork() {
  this->communication.unicast(this->master.id, "notifyWork");
  Serial.printf("Notified master(%u) of work", this->master.id);
}

void Slave::notifyWalkToCharge() {
  this->communication.unicast(this->master.id, "notifyWalkToCharge");
  Serial.printf("Notified master(%u) of walk to charge", this->master.id);
}

void Slave::notifyInWait() {
  this->communication.unicast(this->master.id, "notifyInWait");
  Serial.printf("Notified master(%u) of wait", this->master.id);
}

void Slave::notifyWalkIntoCharge() {
  this->communication.unicast(this->master.id, "notifyWalkIntoCharge");
  Serial.printf("Notified master(%u) of walk into charge", this->master.id);
}

void Slave::notifyInCharge() {
  this->communication.unicast(this->master.id, "notifyInCharge");
  Serial.printf("Notified master(%u) of in charge", this->master.id);
}

void Slave::notifyExitCharge() {
  this->communication.unicast(this->master.id, "notifyExitCharge");
  Serial.printf("Notified master(%u) of exit charge", this->master.id);
}

void Slave::handleEnjoinChargeCommand() {
  switch (this->state) {
  case SlaveState::WORK: {
    this->notifyWalkIntoCharge();
    this->state = SlaveState::WALKING_INTO_CHARGE;
    break;
  }
  case SlaveState::WALKING_TO_CHARGE: {
    this->notifyWalkIntoCharge();
    this->state = SlaveState::WALKING_INTO_CHARGE;
    break;
  }
  case SlaveState::WAIT_CHARGE: {
    this->notifyWalkIntoCharge();
    this->state = SlaveState::WALKING_INTO_CHARGE;
    break;
  }
  case SlaveState::WALKING_INTO_CHARGE: {
    this->notifyWalkIntoCharge();
    break;
  }
  case SlaveState::CHARGE: {
    this->notifyInCharge();
    break;
  }
  case SlaveState::EXITING_CHARGE: {
    this->notifyWalkIntoCharge();
    this->state = SlaveState::WALKING_INTO_CHARGE;
    break;
  }
  }
}

void Slave::handleCancelChargeCommand() {
  switch (this->state) {
  case SlaveState::WORK: {
    this->notifyWork();
    break;
  }
  case SlaveState::WALKING_TO_CHARGE: {
    this->notifyWork();
    this->state = SlaveState::WORK;
    break;
  }
  case SlaveState::WAIT_CHARGE: {
    this->notifyWork();
    this->state = SlaveState::WORK;
    break;
  }
  case SlaveState::WALKING_INTO_CHARGE: {
    this->notifyWork();
    this->state = SlaveState::WORK;
    break;
  }
  case SlaveState::CHARGE: {
    this->notifyExitCharge();
    this->state = SlaveState::EXITING_CHARGE;
    break;
  }
  case SlaveState::EXITING_CHARGE: {
    break;
  }
  }
}
