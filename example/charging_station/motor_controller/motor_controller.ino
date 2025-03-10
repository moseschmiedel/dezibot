#include <AccelStepper.h>

#define STEP_PIN 12
#define DIR_PIN 13

const float stepsPerRevolution = 200;

int microstepSetting = 1;

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

float convert_rotational_position_to_steps(float rotations) {
  return rotations * stepsPerRevolution * microstepSetting;
}
 
 void setup() {
  float MaxRPM = 600;
  float Max_Speed_StepsPerSec = microstepSetting * stepsPerRevolution * MaxRPM / 60;
  stepper.setMaxSpeed(Max_Speed_StepsPerSec);

  float AccelRPMperSec = 3000;
  float Accel_StepsPerSec2 = microstepSetting * stepsPerRevolution * AccelRPMperSec / 60;
  stepper.setAcceleration(Accel_StepsPerSec2);

  stepper.setCurrentPosition(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.moveTo(1000);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  stepper.moveTo(-1000);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  delay(500);
}
