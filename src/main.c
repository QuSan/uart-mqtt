#include<stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <limits.h>  
#include <errno.h>   
#include <fcntl.h>    
#include <termios.h>  
#include <unistd.h>
#include <time.h> 
#include "mqtt.h"
#include "uart.h"
#include <pthread.h> 


int main()
{    Setup_Uart();
   
   char SMS[100];
   while(1)
   {  
     ReceiveThread();
   }


    return 0;
}
