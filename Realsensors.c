#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdbool.h>
void init()
{      
	wiringPiSetupGpio () ;
	pinMode(12,OUTPUT);
}
 void Relais(bool etat)
{ 
	
	digitalWrite(12,etat); 
	 
}

float temperature()
{      
	int fd;
        int data0;
	int data1;
	float result ;
	
        fd=wiringPiI2CSetup (0x44) ; 
       
        if(fd==-1)
        {
                printf("Can't setup the I2C device\n");
                return -1;
        }
        else
        {
                
			wiringPiI2CWriteReg8(fd, 0x24, 0x00);

			delay(16);

			

                        data0=wiringPiI2CReadReg8(fd, 0x00);
			data1=wiringPiI2CReadReg8(fd, 0x01);

			int temp = data0*256+data1;
			 result = -45 +(175*temp/65535.0);
                        printf("temperature=%f °\n", result);
                
        }
        return result;
}	

float humidty()
{      
	int fd;
        int data0;
	int data1;
	float result; 
	
        fd=wiringPiI2CSetup (0x44) ; 
       
        if(fd==-1)
        {
                printf("Can't setup the I2C device\n");
                return -1;
        }
        else
        {
                       wiringPiI2CWriteReg8(fd, 0x24, 0x00);

			delay(16);

                        data0=wiringPiI2CReadReg8(fd, 0x03);
			data1=wiringPiI2CReadReg8(fd, 0x04);
		          
			int humd = data0*256+data1;
			 result = 100 *humd/65535.0;
                        printf("humidité=%f g/m3\n", result);
                
        }
        return result;
}	


int main (int argc, char *argv[])
{ 
  bool etat = true;
  init();
  Relais(etat) ;
  temperature(); 
  humidty()  ;  
        
       
}
