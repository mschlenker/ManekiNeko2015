// #include <SoftwareSerial.h>
#include <Manchester.h>

// Output pin for some debugging
#define DEBUG_PIN 13

// Pin to drive the motor, must be PWM capable
#define MOTPIN 5

// Pin where the dead center of the cat's arm is polled
#define DEADPIN 3

// Pin which powers MPs player - just to save energy
#define MP3_POWER_PIN 12

// Pin which powers the 434MHz receiver
#define RX_POWER_PIN 11

// Pin which powers the 434MHz transmitter
#define TX_POWER_PIN 10

// Receiver pin for sensors located around the cat
#define RX_PIN 7

// Transmitter pin for 434MHz outlets
#define TX_PIN 8

// Number of MP3 files to cycle
#define FILENUM 12

// Minimum and maximum duration of a pause - divide by 10 to get seconds
#define MINPAUSE 15
#define MAXPAUSE 80

// Buffer size for Manchester messages
#define BUFFER_SIZE 5

// Minimum number of cycle the cat will waive it's arm 
#define TOTALCYCLES 5

// Milliseconds to keep the motor running after the arm reached it's dead center
#define BEYONDDEAD 300

// Duty cycle for the motor
#define MOTOR_DUTY_CYCLE 254

// Network and node ID of this Arduino
#define NETWORK_ID 100
#define MY_NODE_ID 23
 
uint8_t rcvBuffer[BUFFER_SIZE];
volatile uint8_t song = 0;
volatile uint8_t lastsong = 0; 
int val = 0; 
volatile int cyclesLeft = TOTALCYCLES;
volatile unsigned long uptime;

/*
  Check the validity of a message received via Manchester transmission.
*/

int checkValidity() {
  if (rcvBuffer[1] > 0 && rcvBuffer[1] != MY_NODE_ID ) {
    return 0;
  }
  byte checksum = B10101010;
  for (int i=0; i<sizeof(rcvBuffer)-1; i++) {
    checksum = checksum ^ rcvBuffer[i];
  }
  if (checksum == rcvBuffer[BUFFER_SIZE - 1]) {
    return 1;
  }
  // Now really check! 
  return 0;
}

/*
  Let's make UAHHHH!
*/

void playSound() {
  uptime = millis();
  song = uptime % FILENUM ;
  if (song == lastsong) {
    song = ( song + 1 ) % FILENUM ;
  } 
  Serial.print("PSONGNAME ");
  Serial.print(song);
  Serial.println(".MP3");
  lastsong = song; 
}

/*
  Initialize the MP3 player
*/

void initPlayer() {
  digitalWrite(MP3_POWER_PIN, HIGH);
  delay(1000);
  Serial.println("L");
  delay(1000);
  Serial.println("ON");
}

void setup() {
  attachInterrupt(0, playSound, RISING);
  pinMode(DEADPIN, INPUT);
  Serial.begin(9600);
  pinMode(DEBUG_PIN, OUTPUT);
  pinMode(TX_POWER_PIN, OUTPUT);
  pinMode(RX_POWER_PIN, OUTPUT);
  pinMode(MP3_POWER_PIN, OUTPUT);
  digitalWrite(RX_POWER_PIN, HIGH);
  initPlayer(); 
  analogWrite(MOTPIN, MOTOR_DUTY_CYCLE);
  man.setupReceive(RX_PIN, MAN_600);
  man.beginReceiveArray(BUFFER_SIZE, rcvBuffer);
}

void loop() {
  if (man.receiveComplete()) {
    uint8_t receivedSize = 0;
    //Serial.println(rcvBuffer[4]);
    if (checkValidity())
      if (rcvBuffer[3] > 0) {
        cyclesLeft = TOTALCYCLES; 
        analogWrite(MOTPIN, 254);
      }
      man.beginReceiveArray(BUFFER_SIZE, rcvBuffer);
  }
  if (Serial.available())
    Serial.read();
  val = digitalRead(DEADPIN);
  if (val > 0) {
    analogWrite(MOTPIN, MOTOR_DUTY_CYCLE / 2);
    while (val > 0) {
       val = digitalRead(DEADPIN);
    }
    analogWrite(MOTPIN, 0);
    // hard coded delay to continue running
    // delay(BEYONDDEAD); 
    analogWrite(MOTPIN, 0);
    uptime = millis();
    if (cyclesLeft > 0) {
      cyclesLeft -= 1;
      delay( ( MINPAUSE + (uptime % ( MAXPAUSE - MINPAUSE ))) * 100 );
      analogWrite(MOTPIN, 254);
    } 
  }
}

