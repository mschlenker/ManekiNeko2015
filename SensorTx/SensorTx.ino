#include <Manchester.h>
#define TX_PIN 12 
#define POWER_PIN 13 

const int sensorPin = A0;
unsigned long val;

uint8_t data[10] = {
  100, // Network ID
  23,  // Target Node
  42,  // My Node
  0,   // Metadata
  0,   // payload byte 1
  0,   // payload byte 2
  0,   // payload byte 3
  0,   // payload byte 4
  0,   // payload byte 5
  0    // checksum
};

void insertPayload() {
   // read the sensor
   val = analogRead(sensorPin);
   // split the value to two bytes
   /* data[4] = val & 0xFF;
   data[5] = (val >> 8) & 0xFF; */
   data[4] = 1;
   data[5] = 0; 
   // debug:
   Serial.println(data[4]);   
   Serial.println(data[5]);   
   // calculate checksum:
   byte checksum = B10101010;
   for (int i=0; i<sizeof(data)-1; i++) {
      checksum = checksum ^ data[i];
   }
   data[9] = checksum; 
   Serial.print("Checksum: "); 
   Serial.println(checksum);
}

void setup() {
  pinMode(POWER_PIN, OUTPUT);
  man.setupTransmit(TX_PIN, MAN_600);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(POWER_PIN, HIGH);
  delay(100);  
  insertPayload(); 
  man.transmitArray(sizeof(data), data);
  digitalWrite(POWER_PIN, LOW);
  delay(60000);
}
