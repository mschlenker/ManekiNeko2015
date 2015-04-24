#include <Manchester.h>

#define RX_PIN 7
#define BUFFER_SIZE 5
#define OUTPIN 13
#define MY_NODE_ID 23
#define MY_NET_ID 100

uint8_t rcvBuffer[BUFFER_SIZE];

int checkValidity() {
  if (rcvBuffer[1] > 0 && rcvBuffer[1] != MY_NODE_ID) {
    return 0;
  }
  byte checksum = B10101010;
  for (int i=0; i<sizeof(rcvBuffer)-1; i++) {
    checksum = checksum ^ rcvBuffer[i];
  }
  if (checksum == rcvBuffer[BUFFER_SIZE - 1]) {
    return 1;
  }
  return 0;
}

void setup() {
  pinMode(OUTPIN, OUTPUT);
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_600);
  man.beginReceiveArray(BUFFER_SIZE, rcvBuffer);
}

void loop() {
  if (man.receiveComplete()) {
    Serial.println("Received something!"); 
    uint8_t receivedSize = 0;
    if (checkValidity())
      if (rcvBuffer[3] > 0) {
        digitalWrite(OUTPIN, HIGH);
        delay(500);
        digitalWrite(OUTPIN, LOW);
      }
      man.beginReceiveArray(BUFFER_SIZE, rcvBuffer);
  }
}
