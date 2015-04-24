#include <SoftwareSerial.h>
#include <Manchester.h>

#define OUTPIN 13
#define MOTPIN 5
#define DEADPIN 3
#define FILENUM 12
#define MINPAUSE 1500
#define MAXPAUSE 8000
#define RX_PIN 7
#define BUFFER_SIZE 10
#define TOTALCYCLES 50

const uint8_t networkId = 100;
const uint8_t myNodeId = 23;
uint8_t rcvBuffer[BUFFER_SIZE];
// SoftwareSerial mySerial(10, 11); // RX, TX
volatile int song = 0;
int val = 0; 
volatile int cyclesLeft = TOTALCYCLES;
volatile unsigned long uptime;

int checkValidity() {
  if (rcvBuffer[1] > 0 && rcvBuffer[1] != myNodeId) {
    return 0;
  }
  byte checksum = B10101010;
  for (int i=0; i<sizeof(rcvBuffer)-1; i++) {
    checksum = checksum ^ rcvBuffer[i];
  }
  if (checksum == rcvBuffer[BUFFER_SIZE - 1]) {
    return 1;
  } else {
    return 1;
  }
  return 0;
}

void playSound() {
  uptime = millis();
  song = uptime % FILENUM ;
  Serial.print("PSONGNAME ");
  Serial.print(song);
  Serial.println(".MP3");
  // song = (song + 1) % FILENUM ;
}

void setup()
{
  analogWrite(MOTPIN, 254);
  attachInterrupt(0, playSound, RISING);
  pinMode(DEADPIN, INPUT);
  Serial.begin(9600);
  pinMode(OUTPIN, OUTPUT);
  delay(5000);
  Serial.println("L");
  delay(3000);
  Serial.println("ON");
  man.setupReceive(RX_PIN, MAN_600);
  man.beginReceiveArray(BUFFER_SIZE, rcvBuffer);
}

void loop() // run over and over
{
  if (man.receiveComplete()) {
    uint8_t receivedSize = 0;
    //Serial.println(rcvBuffer[4]);
    if (checkValidity())
      if (rcvBuffer[4] > 0) {
        cyclesLeft = TOTALCYCLES; 
        analogWrite(MOTPIN, 254);
      }
      man.beginReceiveArray(BUFFER_SIZE, rcvBuffer);
  }
  if (Serial.available())
    Serial.read();
  val = digitalRead(DEADPIN);
  if (val > 0) {
     analogWrite(MOTPIN, 0);
     uptime = millis();
     if (cyclesLeft > 0) {
       cyclesLeft -= 1;
       delay( MINPAUSE + (uptime % ( MAXPAUSE - MINPAUSE )) );
       analogWrite(MOTPIN, 254);
     } 
  }
}

