#include <RCSwitch.h>
#define LIGHTPIN 13
#define ON_SEQ 1381717
#define OFF_SEQ 1381716

RCSwitch mySwitch = RCSwitch();


void setup() {
  // Serial.begin(9600);
  // Serial.println("Bitte Taste auf Fernbedienung druecken..."); 
  mySwitch.enableReceive(0);  // Int 0 = Pin 2
  pinMode(LIGHTPIN, OUTPUT);
}

void loop() {
  if (mySwitch.available()) {
    // Serial.print("Uptime: ");
    // Serial.println(millis());
    if ( mySwitch.getReceivedValue() == OFF_SEQ ) {
      digitalWrite(LIGHTPIN, LOW);
    }
    if ( mySwitch.getReceivedValue() == ON_SEQ ) {
      digitalWrite(LIGHTPIN, HIGH);
    }
    mySwitch.resetAvailable();
  }
}

