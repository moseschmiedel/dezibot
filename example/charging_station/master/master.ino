#include <Dezibot.h>
#include <autocharge/Autocharge.hpp>

void start_chg(SlaveData &slave) {}
void end_chg(SlaveData &slave) {}

auto chargingSlaves = Fifo<SlaveData*>();

Master m = Master(chargingSlaves,
  start_chg,
  end_chg);

void setup() {

  m.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
