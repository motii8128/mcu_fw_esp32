#include "wifi_control.h"

hw_timer_t *timer = NULL;
bool timer_flag = false;
void BroadcastTimer()
{
  if(!timer_flag)
  {
    timer_flag = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  initializeUDP();
  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &BroadcastTimer);
  timerAlarm(timer, 1000000, true, 0);


  Serial.printf("Start WiFi. name:%s, ip_addr:%s", WiFi.softAPSSID(), WiFi.softAPIP().toString());

  
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if(timer_flag)
  {
    String send_msg = WiFi.softAPIP().toString() + "," + device_name;
    Serial.println(broadcast);
    int err = udp.beginPacket(broadcast, remote_port);
    if(err != 1)
    {
      Serial.println("Failed to send begin packet");
    }
    udp.print(send_msg.c_str());
    err = udp.endPacket();
    if(err != 1)
    {
      Serial.println("Failed to send end packet");
    }

    timer_flag = false;
  }
}
