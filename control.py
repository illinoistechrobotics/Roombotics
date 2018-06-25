import pygame
import serial
import time #These are the required libraries (make sure they are installed)

def sendPacket(byte):
    packet = [byte] #now we will send this packet over to the arduino

    for i in packet: #make this a function Joe
        elements = [i]
        print( bytes(elements))
        ser.write(bytes(elements))
    time.sleep(.01);

    return;

ser = serial.Serial('/dev/ttyUSB0', 57600, timeout=1) #this is the port for your xbee


pygame.joystick.init()
joysticks = [pygame.joystick.Joystick(x) for x in range(pygame.joystick.get_count())]

pygame.init()
pygame.joystick.init()
done = False
Xaxis = 0
Yaxis = 0
button = []

for i in range(12):
    button.append(0)


while done==False:
    for event in pygame.event.get(): #whenever we quit pygame pygame will end
        if event.type == pygame.QUIT:
            done=True

    joystick_count = pygame.joystick.get_count() #we find out how many joysticks are plugged in

    for i in range(joystick_count): #take that num and iterate through joysticks to get their data
        joystick = pygame.joystick.Joystick(i)
        joystick.init()

        for i in range(4):#joystick.get_numbuttons()):
            button[i] = joystick.get_button(i)
            sendPacket(((2)<<5)+button[i]+(i<<1))
            print(button)
            

        #print("Controller is " + str(i) + " " + joystick.get_name())

        #if pygame.joystick.get_count() == 2:
        #   print("Num joysticks correct")

        #for f in range(joystick.get_numaxes()):
        #    print("Axis " + str(f) + " position " + str(joystick.get_axis(f))) 

        Xaxis = round(joystick.get_axis(0) * 15 + 15) #this should be the "drone" setup for X and Y axes.  The last joystick found is the primary
        Yaxis = round(joystick.get_axis(3)* 15 + 15)

        print("X axis = " + str(Xaxis) + " Y axis = " + str(Yaxis))

        packet = [Xaxis, Yaxis + ( 1 << 5)] #now we will send this packet over to the arduino
        sendPacket(Xaxis)
        sendPacket(Yaxis + (1 << 5))
        time.sleep(.1)




