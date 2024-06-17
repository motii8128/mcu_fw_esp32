#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiUdp.h>

// USER
bool enable_ap = true;
String device_name = "mcu-esp";
const char* wifi_name = "motiServer";
const char* wifi_pass = "moti8128";
const uint8_t local_port = 64205;
//

// CONSTANTS
const uint8_t remote_port = 64203;
const IPAddress wifi_ip(192, 168, 4, 1);
const IPAddress wifi_gateway(192, 168, 4, 1);
const IPAddress subnet(255, 255, 255, 0);
hw_timer_t *timer = NULL;
bool timer_flag = false;
void ARDUINO_ISR_ATTR onTimer()
{
  if(!timer_flag)
  {
    timer_flag = true;
  }
}

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  delay(1000);

  if(enable_ap)
  {
    WiFi.softAPConfig(wifi_ip, wifi_gateway, subnet);
    if (!WiFi.softAP(wifi_name, wifi_pass, 2, 0, 4, false)) {
      Serial.println("Soft AP creation failed.");
      while (1);
    }
  }
  else
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_name, wifi_pass);
    if(WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Failed to connect to WiFi.");
      // while(1);
    }
  }
  udp.begin(local_port);
  Serial.printf("Start WiFi. name:%s, ip_addr:%s", WiFi.softAPSSID(), WiFi.softAPIP().toString());

  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000000, true, 0);

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if(timer_flag)
  {
    String send_msg = WiFi.softAPIP().toString() + "," + device_name;
    int err = udp.beginPacket(WiFi.softAPBroadcastIP(), remote_port);
    if(err != 1)
    {
      Serial.println("Failed to send begin packet");
    }
    udp.printf(send_msg.c_str());
    err = udp.endPacket();
    if(err != 1)
    {
      Serial.println("Failed to send end packet");
    }

    timer_flag = false;
  }
}
