//Main program for Xbee Communication & Roomba Control
//Loosely based on the principals of SerialCTL written by Nash Kaminski
//By learning and experimenting with the principals of the program, future ITR members will gain a strong base of understanding
//for the SerialCTL program


#include <SoftwareSerial.h>

int roombaRX = 10, roombaTX = 11; //these are the pins that will transmit and recieve data from the Roomba
SoftwareSerial roombaSerial(roombaRX,roombaTX);

int Xaxis = 0; //This value represents the recieved X value (0 to 30)
int Yaxis = 0; //This value represents the recieved Y value
int button[] = {0,0,0,0,0,0,0,0,0,0,0,0}; //This array holds the values of the buttons 
int inNum; //This holds the recieved byte from the Xbee

void setup() {  
  Serial.begin(57600);          //this Serial line will read data from the xbee
  roombaSerial.begin(57600);    //this Serial line will send commands to the roomba

  roombaSerial.write(128); //These two bytes initialize the roomba
  delay(1000);
  roombaSerial.write(130);
  delay(1000);  
  roombaSerial.write(132); //This sets the Roomba in 'unsafe' mode
  delay(1000);
    
  }

void loop() {
  //The laptop-xbee-remote combination, or control station, will transmit bytes to the roomba that contain information on the state of the remote

  while(Serial.available() > 0){ //if there is new data from the control station, interpret it
    
    inNum = int(Serial.read());  //get the value

    switch(inNum >>5){ //Bit shift to the right 5 places, leaving us with the 3 most significant bits.  This value shows what data the byte represents
      case 0: //The byte represents the X value 
        Xaxis = inNum & 31; //update x axis with contained value
        break;
      case 1: //The byte represents the Y value
        Yaxis = inNum & 31; //update y axis with contained value
        break;        
      case 2: //The byte represents the first five button values
      
        for(int i = 0; i<5; i++)
        {
          button[i] = (inNum >> i) & 1;
          Serial.println((String)i+" " + button[i]);
        }
        
  
        break;
        
    }
  
    driveRoomba(Xaxis, Yaxis, 14,16); //This method takes the current X and Y value of the remote and translates it into Roomba motion.
  }

}

void driveRoomba(int joyX, int joyY, int boundsLow, int boundsHigh)
{
  //This function uses input values to move the robot
  //Then sends an instruction to the robot
  
  int speed = map(joyY, 0,30, -500,500);  //The Y value acts as the power to the motors
  int radius = map(joyX,0,30, -200,200); //the X value acts as the radius for the robots turn (the further from the center of the joystick the smaller the radius)

  //for example to move straight, you simply move the Y axis up 
  //0,0 for the joysticks is in the top left corner

  

  
  if(joyX < boundsLow) //these functions invert the radius (remember further from center the smaller the radius so tighter turns)
  {
    radius = map(joyX,0,15,-1,-200);
  }
  if(joyX > boundsHigh)
  {
    radius = map(joyX,15,30,200,0);
  }
  
  if(joyX > boundsLow && joyX < boundsHigh) //if the stick is in the center (based on given bounds) then we want to set it to 0
  {
     radius = 32768;//this is the given 0 value for the radius, part of the documentation
  }
  if(joyY > boundsLow && joyY < boundsHigh)
  {
    speed = 0;
  }
  
  roombaSerial.write(137);      //This line sends the instructions over serial
  roombaSerial.write((speed >> 8) & 255);
  roombaSerial.write((speed) & 255);
  roombaSerial.write((radius >> 8) & 255);
  roombaSerial.write((radius) & 255);

   
  
}

