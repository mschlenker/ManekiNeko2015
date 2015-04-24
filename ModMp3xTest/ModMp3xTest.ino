#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

int song = 1; 

void setup()  
{
  mySerial.begin(9600);
  delay(5000);
  mySerial.println("L");
  delay(3000);
  mySerial.println("ON");
}

void loop() // run over and over
{
  if (mySerial.available())
    mySerial.read();
  mySerial.print("PSONGNAME ");
  mySerial.print(song);
  mySerial.println(".MP3"); 
  song = (song + 1) % 10; 
  delay(3000); 
}

