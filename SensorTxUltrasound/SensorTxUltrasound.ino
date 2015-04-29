#include <Manchester.h>
#define TX_PIN 12 
#define CONTROL_PIN 13 
#define ECHO_PIN 2
#define TRIGGER_PIN 4
  
int sensorVal = 0; 
uint8_t croppedVal = 255;
long duration = 0; 

uint8_t data[3] = {
  23,
  0,
  0
};

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void insertPayload() {
   data[1] = 254; 
   // calculate checksum:
   byte checksum = B10101010;
   for (int i=0; i<sizeof(data)-1; i++) {
     checksum = checksum ^ data[i];
   }
   data[sizeof(data)-1] = checksum; 
}

void transmitMotion() {
  // Serial.println("Motion detected"); 
  insertPayload(); 
  man.transmitArray(sizeof(data), data);
  // digitalWrite(POWER_PIN, LOW);
}

void setup() {
  pinMode(CONTROL_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT); 
  man.setupTransmit(TX_PIN, MAN_300);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH, 50000);
  if (duration < 20000) {
    Serial.println(microsecondsToCentimeters(duration));
    transmitMotion();  
    delay(1000);
  }
  delay(10); 
}
