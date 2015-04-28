#include <Manchester.h>
#define TX_PIN 12 
#define CONTROL_PIN 13 
#define SENSOR_PIN 2 
  
int sensorVal = 0; 
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
  // digitalWrite(POWER_PIN, HIGH);
  // delay(100);  
  insertPayload(); 
  man.transmitArray(sizeof(data), data);
  // digitalWrite(POWER_PIN, LOW);
}

void setup() {
  pinMode(CONTROL_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT); 
  // attachInterrupt(0, transmitMotion, RISING);
  man.setupTransmit(TX_PIN, MAN_300);
  Serial.begin(9600);
}

void loop() {
  sensorVal = digitalRead(SENSOR_PIN); 
  if (sensorVal == HIGH) {
    digitalWrite(CONTROL_PIN, HIGH);
    transmitMotion();
  } else {
    digitalWrite(CONTROL_PIN, LOW);
  }
  // Serial.println("Transmitted something!");
  delay(2000);
}
