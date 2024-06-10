#include <WiFiUdp.h>
#include <WiFi.h>

// Access point settings
const bool EnableAccessPoint = true;
const IPAddress wifi_ip(192, 168, 0, 1);
const IPAddress wifi_gateway(192, 168, 0, 1);
const IPAddress wifi_subnet(255, 255, 255, 0);

// Common settings
const char *WiFiName = "TestServer";
const char *WiFiPass = "TestServer";

String DeviceName = "mcu-esp32-01";
// END SETTINGS


WiFiUDP udp;
const uint16_t local_port = 64202;

IPAddress default_ip(192, 168, 0, 100);
IPAddress default_myDns(192, 168, 0, 1);
IPAddress broadcast_ip(255, 255, 255, 255);

char buf[512];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  //  D2 pin is digital output
  pinMode(T2, OUTPUT);
  // D15 pin is pwm output
  // ledcSetup(0, 12800, 8);
  // ledcAttachPin(A13, 0);

  // Debug Serial
  Serial.begin(115200);
  digitalWrite(LED_BUILTIN, LOW);

  if(EnableAccessPoint)
  {
    if(!WiFi.mode(WIFI_AP))
    {
      Serial.println("[ERROR]Failed to setting WiFi mode!!");
    }
    if(!WiFi.softAPConfig(wifi_ip, wifi_gateway, wifi_subnet))
    {
      Serial.println("[ERROR]Failed to set config!!");
    }

    Serial.println("[INFO]Start connecting");
    WiFi.softAP(WiFiName, WiFiPass);
    
    Serial.println("[INFO]Connection Established!!");
    udp.begin(default_ip, local_port);

    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    if(!WiFi.mode(WIFI_STA))
    {
      Serial.println("[ERROR]Failed to setting WiFi mode!!");
    }

    Serial.println("[INFO]Start connecting");
    WiFi.begin(WiFiName, WiFiPass);
    

    Serial.println("[INFO]Connection Established!!");
    udp.begin(default_ip,local_port);

    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void loop() {
  int packet_size = udp.parsePacket();
  if(packet_size > 0){
    for(auto i = 0; i < 512; i++)
    {
      buf[i] = 0;
    }

    udp.read(buf, 512);

    String string_data(buf);
    string_data.trim();

    int power_int = string_data.toInt();

    if(string_data != "0" && power_int == 0)
    {
      Serial.println("Unknow data");
      return;
    }

    if(power_int > 0)
    {
      digitalWrite(T2, HIGH);
    }
    else
    {
      digitalWrite(T2, LOW);
    }

    uint pwm_rate = abs(power_int);
    pwm_rate = (int)((float)(pwm_rate)*2.55);

    if(pwm_rate > 255)
    {
      pwm_rate = 255;
    }

    // ledcWrite(0, pwm_rate);

    udp.beginPacket(broadcast_ip, 64203);
    String send_str = WiFi.localIP().toString() + "," + DeviceName;
    Serial.printf("Send %s", send_str.c_str());
    udp.print(send_str.c_str());
    udp.endPacket();
  }
}
