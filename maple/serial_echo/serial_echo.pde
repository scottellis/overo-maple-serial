/*
  Serial test between the Maple and a gumstix using serial at TTL. 
  Using USART1, TX = pin7 and RX = pin8
  
  Reads from the serial port until a '\n' is received and then return
  the chararcters recieved in one block after converting to upper-case.
 */

#include <Wire.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 127

int pos;
char buff[BUF_SIZE + 1];


void setup()
{
    Serial1.begin(115200);
}

void loop()
{
    while (Serial1.available()) {
        if (pos > BUF_SIZE) {
            pos = 0;
        }

        buff[pos] = toupper(Serial1.read());          

        if (buff[pos] == '\n') {         
            Serial1.println(buff);
            pos = 0;
            memset(buff, 0, sizeof(buff));
        }
        else {
            pos++;
        }
    }
}  
            
            
          
    
