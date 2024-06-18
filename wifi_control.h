#ifndef WIFI_CONTROL_H_
#define WIFI_CONTROL_H_

#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiAP.h>

// SETTINGS
extern bool enable_ap;
extern String device_name;
extern String wifi_name;
extern String wifi_pass;
extern uint8_t local_port;
//

// CONSTANTS
extern uint8_t remote_port;
extern IPAddress wifi_ip;
extern IPAddress wifi_gateway;
extern IPAddress subnet;
extern IPAddress broadcast;
extern WiFiUDP udp;
extern char buf[512];

void initializeUDP();
void RecieveUDP();
void broadcastUDP();
#endif