#include <Manchester.h>
#define TX_PIN 12 
#define POWER_PIN 13 
#define PIR_PIN A3  
  
uint8_t data[5] = {
  100, // Network ID
  23,  // Target Node
  42,  // My Node
  0,   // payload byte
  0    // checksum
};

void insertPayload() {
   data[3] = 254; 
   // calculate checksum:
   byte checksum = B10101010;
   for (int i=0; i<sizeof(data)-1; i++) {
      checksum = checksum ^ data[i];
   }
   data[4] = checksum; 
}

void transmitMotion() {
  Serial.println("Motion detected"); 
  digitalWrite(POWER_PIN, HIGH);
  // delay(100);  
  insertPayload(); 
  man.transmitArray(sizeof(data), data);
  digitalWrite(POWER_PIN, LOW);
}

void setup() {
  pinMode(POWER_PIN, OUTPUT);
  attachInterrupt(0, transmitMotion, RISING);
  man.setupTransmit(TX_PIN, MAN_600);
  Serial.begin(9600);
}

void loop() {
  // Serial.println("Transmitted something!");
  // delay(10000);
}
