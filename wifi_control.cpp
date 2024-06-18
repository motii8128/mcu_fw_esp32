#include "wifi_control.h"

bool enable_ap = true;
String device_name = "mcu-esp-01";
String wifi_name = "mcuServer";
String wifi_pass = "mcu64205";
uint8_t local_port = 64205;

uint8_t remote_port = 64203;
IPAddress wifi_ip(192, 168, 4, 1);
IPAddress wifi_gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress broadcast(255, 255, 255, 255);
WiFiUDP udp;
char buf[512];

void initializeUDP()
{
  if(enable_ap)
  {
    WiFi.softAPConfig(wifi_ip, wifi_gateway, subnet);
    if (!WiFi.softAP(wifi_name, wifi_pass, 2, 0, 4, false)) {
      Serial.println("[WiFi_ERROR]Soft AP creation failed.");
      while (1);
    }

    Serial.println("[WiFi_INFO]Start Wifi on AccesPointMode");
  }
  else
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_name, wifi_pass);
    if(WiFi.status() != WL_CONNECTED)
    {
      Serial.println("[WiFi_ERROR]Failed to connect to WiFi.");
      while(1);
    }

    Serial.println("[WiFi_INFO]Start Wifi on AccesPointMode");
  }
  int err = udp.begin(local_port);

  if(err != 1)
  {
    Serial.println("[WiFi_ERROR]Failed to start UDP socket");
  }
}

void RecieveUDP()
{
  if(udp.parsePacket() > 0)
  {
    udp.read(buf, 512);

    String receive_data(buf);
    


  }
}

void broadcastUDP()
{
  
}