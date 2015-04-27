
#include <Manchester.h>
#include <RCSwitch.h>

// Output pin for some debugging
#define DEBUG_PIN 13

// Pin to drive the motor, must be PWM capable
#define MOTPIN 10

// Pin where the dead center of the cat's arm is polled
#define DEADPIN 2

// Pin which powers MPs player - just to save energy
#define MP3_POWER_PIN A0

// Pin which powers the 434MHz receiver
#define RX_POWER_PIN 6

// Pin which powers the 434MHz transmitter
#define TX_POWER_PIN 5

// Receiver pin for sensors located around the cat
#define RX_PIN 7

// Transmitter pin for 434MHz outlets
#define TX_PIN 4

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
#define MOTOR_DUTY_CYCLE 100

// Network and node ID of this Arduino for Manchester transmission
#define NETWORK_ID 100
#define MY_NODE_ID 23

// System code and unit code for RC switch
// #define SYSTEMCODE "11111"
// #define UNITCODE "00010"

#define SYSTEMCODE 1
#define UNITCODE 1

 
uint8_t rcvBuffer[BUFFER_SIZE];
volatile uint8_t song = 0;
volatile uint8_t lastsong = 0; 
int val = 0; 
volatile int cyclesLeft = TOTALCYCLES;
volatile unsigned long uptime;
RCSwitch lightSwitch = RCSwitch();

/*
  Check the validity of a message received via Manchester transmission.
*/

int checkValidity() {
  return 1;
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
  delay(1200);
  Serial.println("L");
  delay(300);
  Serial.println("ON");
}

void stopPlayer() {
  digitalWrite(MP3_POWER_PIN, LOW);
}

/*
  Switch the light
*/

void lightOn() {
  digitalWrite(RX_POWER_PIN, LOW);
  digitalWrite(TX_POWER_PIN, HIGH);
  delay(100);
  lightSwitch.switchOn(SYSTEMCODE, UNITCODE);
  digitalWrite(TX_POWER_PIN, LOW);
  digitalWrite(RX_POWER_PIN, HIGH);
}

void lightOff() {
  digitalWrite(RX_POWER_PIN, LOW);
  digitalWrite(TX_POWER_PIN, HIGH);
  delay(100);
  lightSwitch.switchOff(SYSTEMCODE, UNITCODE);
  digitalWrite(TX_POWER_PIN, LOW);
  digitalWrite(RX_POWER_PIN, HIGH);
}

void setup() {
  attachInterrupt(1, playSound, RISING);
  pinMode(DEADPIN, INPUT);
  Serial.begin(9600);
  pinMode(DEBUG_PIN, OUTPUT);
  pinMode(TX_POWER_PIN, OUTPUT);
  pinMode(RX_POWER_PIN, OUTPUT);
  pinMode(MP3_POWER_PIN, OUTPUT);
  pinMode(MOTPIN, OUTPUT);
  digitalWrite(RX_POWER_PIN, HIGH);
  digitalWrite(TX_POWER_PIN, LOW);
  lightSwitch.enableTransmit(TX_PIN);
  initPlayer(); 
  analogWrite(MOTPIN, MOTOR_DUTY_CYCLE);
  // digitalWrite(MOTPIN, HIGH);
  digitalWrite(DEBUG_PIN, HIGH);
  man.setupReceive(RX_PIN, MAN_300);
  man.beginReceiveArray(BUFFER_SIZE, rcvBuffer);
}

void loop() {
  if (man.receiveComplete()) {
    uint8_t receivedSize = 0;
    if (checkValidity())
      if (rcvBuffer[3] > 0) {
        if (cyclesLeft < 1 ) {
          // switch on everything again
          lightOn();
          initPlayer();   
        }
        cyclesLeft = TOTALCYCLES; 
        analogWrite(MOTPIN, MOTOR_DUTY_CYCLE);
        digitalWrite(DEBUG_PIN, HIGH);
      }
      man.beginReceiveArray(BUFFER_SIZE, rcvBuffer);
  }
  if (Serial.available()) Serial.read();
  val = digitalRead(DEADPIN);
  if (val > 0) {
    analogWrite(MOTPIN, MOTOR_DUTY_CYCLE / 2);
    digitalWrite(DEBUG_PIN, HIGH);
    while (val > 0) {
       val = digitalRead(DEADPIN);
    }
    analogWrite(MOTPIN, 0);
    digitalWrite(DEBUG_PIN, LOW);
    // hard coded delay to continue running
    // delay(BEYONDDEAD); 
    // analogWrite(MOTPIN, 0);
    uptime = millis();
    if (cyclesLeft > 0) {
      cyclesLeft -= 1;
      delay( ( MINPAUSE + (uptime % ( MAXPAUSE - MINPAUSE ))) * 100 );
      analogWrite(MOTPIN, MOTOR_DUTY_CYCLE);
    } else {
      // we have just reached the last cycle, switch off everything
      stopPlayer(); 
      lightOff(); 
    }
  }
}

