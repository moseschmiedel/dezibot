#include <Dezibot.h>
#include <autocharge/Autocharge.hpp>

auto master = MasterData(1);

void step_work() {
    Serial.printf("Execute 'step_work'");
}

bool step_to_charge(MasterData &master) {
    Serial.printf("Execute 'step_to_charge' with master %u", master.id);
    return true;
}

void step_wait_charge(MasterData &master) {
    Serial.printf("Execute 'step_wait_charge' with master %u", master.id);
}

bool step_into_charge(MasterData &master) {
    Serial.printf("Execute 'step_into_charge' with master %u", master.id);
    return true;
}

void step_charge(MasterData &master) {
    Serial.printf("Execute 'step_charge' with master %u", master.id);
}

bool step_exit_charge(MasterData &master) {
    Serial.printf("Execute 'step_exit_charge' with master %u", master.id);
    return true;
}

Slave m = Slave(SlaveState::WORK, master,
  step_work,
  step_to_charge,
  step_wait_charge,
  step_into_charge,
  step_charge,
  step_exit_charge);

void setup() {

  m.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
