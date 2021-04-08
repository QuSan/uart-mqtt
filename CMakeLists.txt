
cmake_minimum_required(VERSION 3.5.1)
project (zigbee)
#add_library(lpaho-mqtt3c SHARED IMPORTED)
link_libraries(paho-mqtt3c)
#link_libraries(MQTTClient.h)

include_directories(include)

set(SOURCES src/main.c src/mqtt.c src/uart.c)

add_executable(zigbee ${SOURCES})
