#include "RCSwitch.h"

#define SENDNUMBERX 5
RCSwitch mySwitch = RCSwitch();

// the setup function runs once when you press reset or power the board
void setup() {

  mySwitch.enableTransmit(17);  // Receiver on interrupt 0 => that is pin #2

  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}

unsigned char DEBUGMode = 0;

unsigned char RecvCommand = 0;
unsigned char BlindID = 0;
unsigned char BlindCmd = 0;
unsigned long SendCommand = 0;


void loop()
{

  RecvCommand = 0;
  
  if (Serial.available() > 0) {

    RecvCommand = Serial.read();

    if( RecvCommand == 0xFF )
    {
      DEBUGMode = 1;   
    }
    else
    {
           
      BlindID = RecvCommand & 0x3F;
      BlindCmd = (RecvCommand & 0xC0) >> 6;
      SendCommand = (unsigned long) (BlindID << 8); 
      SendCommand = (unsigned long) (SendCommand << 8); 
      SendCommand ^= (unsigned long) (0x65 << 8);
      SendCommand ^= (unsigned long) (BlindCmd + 0x30);
  
      for (int i = 0; i < SENDNUMBERX; i++)
      {
        mySwitch.send(SendCommand, 24);
        delay(50);
      }

      if ( DEBUGMode == 1 )
      {  
        Serial.print("Blind ID recived: ");
        Serial.println(BlindID, HEX);
        Serial.print("Blind Command recived: ");
        Serial.println(BlindCmd, HEX);
        Serial.print("Sending command: ");
        Serial.println(SendCommand, HEX);
      }
    }
    
  }
}
