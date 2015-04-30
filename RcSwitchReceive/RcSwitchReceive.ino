#include <RCSwitch.h>
#define LIGHTPIN 13
#define ON_SEQ 1381717
#define OFF_SEQ 1381716

const long maxrun = 110000;
unsigned long uptime;
unsigned long lastOn;
RCSwitch mySwitch = RCSwitch();

void setup() {
  // Serial.begin(9600);
  // Serial.println("Bitte Taste auf Fernbedienung druecken..."); 
  uptime = 0;
  lastOn = 0;
  mySwitch.enableReceive(0);  // Int 0 = Pin 2
  pinMode(LIGHTPIN, OUTPUT);
}

void loop() {
  uptime = millis();
  if (mySwitch.available()) {
    // Serial.print("Uptime: ");
    // Serial.println(millis());
    if ( mySwitch.getReceivedValue() == OFF_SEQ ) {
      digitalWrite(LIGHTPIN, LOW);
      // running = 0;
    }
    if ( mySwitch.getReceivedValue() == ON_SEQ ) {
      lastOn = uptime; 
      digitalWrite(LIGHTPIN, HIGH);
    }
    mySwitch.resetAvailable();
  }
  if (uptime - lastOn > maxrun) {
       digitalWrite(LIGHTPIN, LOW);
  }
}


