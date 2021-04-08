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
{     Setup_Uart();
      Setup_mqtt();
      Receive_mqtt();
while(1)
{   
    Receive_Uart();
}

  //MQTTClient_disconnect(client, 10000);
  //  MQTTClient_destroy(&client);

return 0;

}

