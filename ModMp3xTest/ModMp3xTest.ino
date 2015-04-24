#include <SoftwareSerial.h>
#define OUTPIN 13
#define MOTPIN 5
#define DEADPIN 3
#define FILENUM 12
#define MINPAUSE 1500
#define MAXPAUSE 8000

SoftwareSerial mySerial(10, 11); // RX, TX

volatile int song = 0;
int val = 0; 
volatile unsigned long uptime;

void playSound() {
  uptime = millis();
  song = uptime % FILENUM ;
  mySerial.print("PSONGNAME ");
  mySerial.print(song);
  mySerial.println(".MP3");
  // song = (song + 1) % FILENUM ;
}

void motorOff() {
 analogWrite(MOTPIN, 0);
}

void setup()
{
  analogWrite(MOTPIN, 254);
  attachInterrupt(0, playSound, RISING);
  pinMode(DEADPIN, INPUT);
  mySerial.begin(9600);
  pinMode(OUTPIN, OUTPUT);
  delay(5000);
  mySerial.println("L");
  delay(3000);
  mySerial.println("ON");
}

void loop() // run over and over
{
  if (mySerial.available())
    mySerial.read();
  val = digitalRead(DEADPIN);
  if (val > 0) {
     analogWrite(MOTPIN, 0);
     uptime = millis();
     delay( MINPAUSE + (uptime % ( MAXPAUSE - MINPAUSE )) );
     analogWrite(MOTPIN, 254); 
  }
}

