#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "uart.h"
#define ADDRESS     "tcp://192.168.55.2:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "test"
#define TOPIC1      "test1"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient client;



void Setup_mqtt()
{
  MQTTClient_create(&client, ADDRESS, CLIENTID,
  MQTTCLIENT_PERSISTENCE_NONE, NULL);
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;
}

void delivered(void *context, MQTTClient_deliveryToken dt)
{
 //   printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;

  //  printf("Message arrived\n");
  //  printf("     topic: %s\n", topicName);
    printf("Nhan: ");
    payloadptr = message->payload;
    Send_Uart(message->payload);   
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
     MQTTClient_freeMessage(&message);
     MQTTClient_free(topicName);
    return 1;
}


void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}


int Receive_mqtt()
{    
    int rc;  
   MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
    printf("Failed to connect, return code %d\n", rc);
    exit(EXIT_FAILURE);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
      MQTTClient_subscribe(client,TOPIC1, QOS);
   // return rc;
}



int Send_mqtt(char argv[])
{    

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;  
    pubmsg.payload = argv;
    pubmsg.payloadlen = strlen(argv);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    /*printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), argv, TOPIC, CLIENTID);
            */
          
//    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
//    printf("Message with delivery token %d delivered\n", token);
//    return rc;
}






























/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "mqtt.h"

#define ADDRESS     "tcp://192.168.55.2:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "test"
#define QOS         1
#define TIMEOUT     10000L

MQTTClient client;
int Send_server(char argv[])
{    
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    pubmsg.payload = argv;
    pubmsg.payloadlen = strlen(argv);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), argv, TOPIC, CLIENTID);
            
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

*/

