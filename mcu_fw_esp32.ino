#include <WiFiUdp.h>
#include <WiFi.h>
#include <lwip/dns.h>

// Access point settings
const bool EnableAccessPoint = true;
const IPAddress wifi_ip(192, 168, 0, 3);
const IPAddress wifi_gateway(192, 168, 0, 1);
const IPAddress wifi_subnet(255, 255, 255, 0);

// Common settings
const char *WiFiName = "Test";
const char *WiFiPass = "Test1000";

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
  pinMode(T3, OUTPUT);
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

    WiFi.softAPConfig(wifi_ip, wifi_gateway, wifi_subnet);

    Serial.println("[INFO]Start connecting");
    WiFi.softAP(WiFiName, WiFiPass);

    ip_addr_t dns_server = IPADDR4_INIT_BYTES(8, 8, 8, 8);
    dns_setserver(0, &dns_server);
    
    Serial.println("[INFO]Connection Established!!");
    if(udp.begin(default_ip, local_port) == 0)
    {
      Serial.println("[ERROR] Failed to begin UDP");
    }

    Serial.printf("[INFO] IP:%s\n", WiFi.softAPIP().toString());

    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    if(!WiFi.mode(WIFI_STA))
    {
      Serial.println("[ERROR]Failed to setting WiFi mode!!");
    }

    Serial.println("[INFO]Start connecting");
    if(WiFi.begin(WiFiName, WiFiPass) != WL_CONNECTED)
    {
      Serial.println("[ERROR] Failed to connect WiFi.");
    }

    Serial.println("[INFO]Connection Established!!");
    Serial.print("[INFO]");
    Serial.println(WiFi.SSID());
    udp.begin(local_port);

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
    dacWrite(T3, pwm_rate);

    String send_str = WiFi.localIP().toString() + "," + DeviceName;
    Serial.println(send_str.c_str());
    udp.beginPacket(broadcast_ip, 64203);
    udp.printf(send_str.c_str());
    udp.endPacket();
  }
}