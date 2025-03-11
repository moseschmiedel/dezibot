#include "Dezibot.h"


Dezibot dezibot = Dezibot();

void setup() {
  dezibot.begin();
  dezibot.communication.begin();
  dezibot.communication.setGroupNumber(5);
}

void loop() {
  switch (dezibot.motion.detection.getTiltDirection()) {
    case Front:
      dezibot.multiColorLight.setTopLeds(GREEN);
      dezibot.communication.broadcast("vorn");
      break;
    case Left:
      dezibot.multiColorLight.setTopLeds(YELLOW);
      dezibot.communication.broadcast("links");
      break;
    case Right:
      dezibot.multiColorLight.setTopLeds(TURQUOISE);
      dezibot.communication.broadcast("rechts");
      break;
    case Back:
      dezibot.multiColorLight.setTopLeds(BLUE);
      dezibot.communication.broadcast("stop");
      break;
    case Flipped:
      dezibot.multiColorLight.setTopLeds(PINK);
      dezibot.communication.broadcast("stop");
      break;
    case Neutral:
      dezibot.multiColorLight.turnOffLed();
      dezibot.communication.broadcast("stop");
      break;

    case Error:
      dezibot.multiColorLight.setTopLeds(RED);
      dezibot.communication.broadcast("stop");
      break;
  }

  delay(100);
}
