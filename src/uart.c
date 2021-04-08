// thu vien chuan cua c.
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <limits.h>  
#include <errno.h>   . 
#include <stdint.h>
#include <fcntl.h>     
#include <termios.h>  
#include <unistd.h>
#include <time.h>     
#include <pthread.h>  
#include <sys/ioctl.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include"uart.h"
#include"mqtt.h"

int fd;/*File Descriptor*/
void Send_Uart(char *SendMessage)
{
    int i;
	for(i = 0;i<255;i++)
	{ 
		if(SendMessage[i]=='#')
		{ break;}
	}
    printf("\nsend data size %d \n" ,i);
	write(fd, SendMessage,i);
}

void Receive_Uart()
{   char receiveBuffer[255];  
	int8_t receiveCount;
	int8_t i; 
	                
    char send_receiveBuffer[255];
	int8_t j=0; 
	int k=1;
			while(k)
			{     
				receiveCount = read(fd, receiveBuffer, 255);
				for(int i =0; i<receiveCount;i++)
				{   
					if(receiveBuffer[i]=='#')
					{		send_receiveBuffer[j]='\0';
					        k=0;
					        break;
					}
					send_receiveBuffer[j]=receiveBuffer[i];
					j++;			
					
				}
			}
			    
				printf("%s \n",send_receiveBuffer);
				Send_mqtt(send_receiveBuffer);

}

void Setup_Uart()
{			
	/*------------------------------- Opening the Serial Port -------------------------------*/
	/* Change /dev/ttyUSB0 to the one corresponding to your system */
    	fd = open("/dev/ttyAMA0",O_RDWR | O_NOCTTY);					
    	if(fd == -1)						/* Error Checking */
       		printf("\n  Error! in Opening ttyUSB0  ");
	/*---------- Setting the Attributes of the serial port using termios structure --------- */
	struct termios SerialPortSettings;	/* Create the structure                          */
	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */
	/* Setting the Baud rate */
	cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */
	/* 8N1 Mode */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CRTSCTS;  /* No Hardware flow Control                         */

	SerialPortSettings.c_iflag = 0;
	SerialPortSettings.c_oflag = 0;
	SerialPortSettings.c_lflag = 0;
	
	/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] = 1; /* Read at least 10 characters */
	SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */
	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
	    printf("\n  ERROR ! in Setting attributes");
        /*------------------------------- Read data from serial port -----------------------------*/
	tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */
    return 0;
}
