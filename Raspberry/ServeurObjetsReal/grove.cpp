#include "grove.h"

void init()
{
    wiringPiSetupGpio();
    pinMode(12,OUTPUT);
}

void relay(bool etat)
{
    digitalWrite(12,etat);
}

float temperature()
{
    int fd;
    int msb;
    int lsb;
    float result ;

        fd=wiringPiI2CSetup (0x44) ;

        if(fd==-1)
        {
                printf("Can't setup the I2C device\n");
                return -1;
        }else{
            wiringPiI2CWriteReg8(fd, 0x24, 0x00);

            delay(16);

            msb=wiringPiI2CReadReg8(fd, 0x00);
            lsb=wiringPiI2CReadReg8(fd, 0x01);

            int temp = msb*256+lsb;
            result = -45 +(175*temp/65535.0);
        }
        return result;
}

float humidity()
{
    int fd;
    int msb;
    int lsb;
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

            msb=wiringPiI2CReadReg8(fd, 0x03);
            lsb=wiringPiI2CReadReg8(fd, 0x04);

            int humd = msb*256+lsb;
             result = 100 *humd/65535.0;

        }
        return result;
}
