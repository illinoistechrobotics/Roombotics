#include <SoftwareSerial.h>
#include <Servo.h>

Servo lift,pusher;



SoftwareSerial roombaSerial(10,11);
int Xaxis = 0; //This value represents the recieved X value (0 to 30)
int Yaxis = 0; //This value represents the recieved Y value
int button[] = {0,0,0,0,0,0,0,0,0,0,0,0}; //This array holds the values of the buttons 
int inNum; //This holds the recieved byte
int pusherTime=0;


void setup() {
  // put your setup code here, to run once:
  lift.attach(9);
  pusher.attach(3);
  
  Serial.begin(57600);          //this Serial line will read data from the xbee
  roombaSerial.begin(57600);    //this Serial line will send commands to the roomba

  roombaSerial.write(128); //this code initializes the roomba
  delay(1000);
  roombaSerial.write(130);
  delay(1000);  
  roombaSerial.write(132);
  delay(1000);
    
  }

void loop() {

  while(Serial.available() > 0){ //if there is new data from the remote, update the robot
    inNum = int(Serial.read());  //get the value

    switch(inNum >>5){ //find out what the value represents with bitwise operators
      case 0: 
        Xaxis = inNum & 31; //update x axis with contained value
        break;
      case 1:
        Yaxis = inNum & 31; //update y axis with contained value
        break;        
      case 2: //this case updates all the buttons
      Serial.println("worked");
      
        for(int i = 0; i<5; i++)
        {
          button[i] = (inNum >> i) & 1;
          Serial.println((String)i+" " + button[i]);
        }
        
          
  if(button[0]==1)
  {
    lift.write(180);
    //liftAngle=false;
  }
  else if(button[1]==1)
  {
    lift.write(110);
    //liftAngle=true;
  }
  
  if(button[2]==1)
  {
    pusher.write(55);
  }
  else
  {
    pusher.write(180);
  }
  
        break;
        
    }

   /* Serial.print("X: " + (String)Xaxis+ " Y: " + (String)Yaxis);
    for(int i =0; i < 12; i++)
    {
      Serial.print(" " + (String)i + " " + (String)button[i]);
    }
    Serial.println("");   //telemetry for arduino
*/
  
    driveRoomba(Xaxis, Yaxis, 14,16); // use current X and Y values to drive the robot
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

