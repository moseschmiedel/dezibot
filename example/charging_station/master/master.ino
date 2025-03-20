#include <Dezibot.h>
#include <autocharge/Autocharge.hpp>

void start_chg(SlaveData &slave) {
    Serial.printf("Execute 'start_chg for slave %u'", slave.id);
}
void end_chg(SlaveData &slave) {
    Serial.printf("Execute 'end_chg for slave %u'", slave.id);
}

auto chargingSlaves = Fifo<SlaveData*>();

Master master = Master(chargingSlaves,
  start_chg,
  end_chg);

void setup() {

  master.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
