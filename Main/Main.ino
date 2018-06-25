#include <SoftwareSerial.h>


SoftwareSerial roombaSerial(10,11);
int Xaxis = 0;
int Yaxis = 0;
int button[] = {0,0,0,0,0,0,0,0,0,0,0,0};
int inNum;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);          //this Serial line will read data from the xbee
  roombaSerial.begin(57600);    //this Serial line will send commands to the roomba

  roombaSerial.write(128);
  delay(1000);
  roombaSerial.write(130); //this code initializes the roomba
  delay(1000);  
  roombaSerial.write(132);
  delay(1000);
  //roombaSerial.write(134);
  //driveRoomba(0,0,14,16);
  
  }

void loop() {
  // put your main code here, to run repeatedly:

  while(Serial.available() > 0){
    inNum = int(Serial.read());

    switch(inNum >>5){
      case 0:
        Xaxis = inNum & 31;
        break;
      case 1:
        Yaxis = inNum & 31;
        break;        
      case 2: //this case updates all the buttons
        int button_address = (inNum & 30)>>1;
        button[button_address] = inNum & 1;
        break;
        
    }

    Serial.print("X: " + (String)Xaxis+ " Y: " + (String)Yaxis);
    for(int i =0; i < 12; i++)
    {
      Serial.print(" " + (String)i + " " + (String)button[i]);
    }
    Serial.println("");
    driveRoomba(Xaxis, Yaxis, 14,16);
  }

}

void driveRoomba(int joyX, int joyY, int boundsLow, int boundsHigh)
{
  int speed = map(joyY, 0,30, -500,500);
  int radius = map(joyX,0,30, -200,200);
  if(joyX < boundsLow)
  {
    radius = map(joyX,0,15,-1,-200);
  }
  if(joyX > boundsHigh)
  {
    radius = map(joyX,15,30,200,0);
  }
  
  if(joyX > boundsLow && joyX < boundsHigh)
  {
     radius = 32768;
  }
  if(joyY > boundsLow && joyY < boundsHigh)
  {
    speed = 0;
  }
  
  roombaSerial.write(137);
  roombaSerial.write((speed >> 8) & 255);
  roombaSerial.write((speed) & 255);
  roombaSerial.write((radius >> 8) & 255);
  roombaSerial.write((radius) & 255);

  Serial.println(radius);
  
}

