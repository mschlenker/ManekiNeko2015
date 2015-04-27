#include <RCSwitch.h>

#define SYSTEMCODE "11111"
#define UNITCODE "00010"

RCSwitch mySwitch = RCSwitch();

void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  mySwitch.enableTransmit(4);
}

void loop() {
  mySwitch.switchOn(SYSTEMCODE, UNITCODE);
  delay(3000);
  mySwitch.switchOff(SYSTEMCODE, UNITCODE);
  delay(3000);
}
