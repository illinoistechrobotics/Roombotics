import pygame
import serial
import time #These are the required libraries (make sure they are installed)

def sendPacket(byte): #user defines packet
    packet = [byte] #now we will send this packet over to the arduino

    for i in packet: #probably a better way to do this
        elements = [i]
        print( bytes(elements))
        ser.write(bytes(elements)) #sends packets over xbee
    time.sleep(.005); #wait a bit between sending

    return;

ser = serial.Serial('/dev/ttyUSB0', 57600, timeout=1) #this is the port for your xbee


pygame.joystick.init() 
joysticks = [pygame.joystick.Joystick(x) for x in range(pygame.joystick.get_count())] #pygame will find any number of joysticks present on your computer


pygame.init()
pygame.joystick.init()
done = False
Xaxis = 0
Yaxis = 0
button = []

for i in range(12):#create an array to put button values in
    button.append(0)


while done==False:
    for event in pygame.event.get(): #whenever we quit pygame pygame will end
        if event.type == pygame.QUIT:
            done=True

    joystick_count = pygame.joystick.get_count() #we find out how many joysticks are plugged in

    for i in range(joystick_count): #take that num and iterate through joysticks to get their data
        joystick = pygame.joystick.Joystick(i)
        joystick.init()
        buttonPacket = 0
        for j in range(5): #send the first few values of buttons (adding more and decreasing the delay on sending packets seems to make recieved packets less accurate)
            buttonPacket = (buttonPacket<<1)+joystick.get_button(j)
           
           
        sendPacket(((2)<<5)+buttonPacket)
        print(buttonPacket)
            

        

        Xaxis = round(joystick.get_axis(0) * 15 + 15) #this should be the "drone" setup for X and Y axes.  The last joystick found is the primary
        Yaxis = round(joystick.get_axis(3)* 15 + 15)

        #The joystick axis are indexed at 0, so Left X = 0, Left Y = 1, Right X = 2, Right Y = 3 and so on if you have more

        print("X axis = " + str(Xaxis) + " Y axis = " + str(Yaxis))

        packet = [Xaxis, Yaxis + ( 1 << 5)] #now we will send this packet over to the arduino
        sendPacket(Xaxis)
        sendPacket(Yaxis + (1 << 5))
        time.sleep(.05)     #delay is needed for accuracy on arduino 




