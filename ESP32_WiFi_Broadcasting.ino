#include <WiFi.h>
#include <WiFiUdp.h>

const char ssid[] = "MyUDPBroadcasting_WiFi"; // SSID
const char pass[] = "aiueoabc";  // password
const int localPort = 8888;      // Port from esp
const int remotePort = 9818;     // Port to device (Set this on device to listen)
const IPAddress ip(192, 168, 4, 1);       // IP (Use as gateway, too)
const IPAddress subnet(255, 255, 255, 0); // subnet
static const char *RemoteIpadr = "255.255.255.255"; // ToBroadCast

WiFiUDP udp;

// WiFi Initialize Function
void wifiSetup() {
  Serial.println("WiFiSetup");
  WiFi.softAP(ssid, pass);           // SSID password Setting
  delay(100);                        // wait and let esp32 to set SSID and password
  WiFi.softAPConfig(ip, ip, subnet); // set ip, gateway, subnet

  Serial.print("AP IP address: ");
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);

  Serial.println("Starting UDP");
  udp.begin(localPort);

  Serial.print("Local port: ");
  Serial.println(localPort);
}

unsigned long stime;

void setup() {
  Serial.begin(115200);
  wifiSetup();
  stime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  int example_number = 0120;
  String message = "PutWhatYouWantToSend_like: " + String(example_number);
  
  Serial.print("This is what you send to remote port => ");
  Serial.println(message);
  
  int msgLength = message.length() + 1;
  char char_array[msgLength];
  message.toCharArray(char_array, msgLength);
  
  if (millis() - stime > 100) {  // send once in 100 msecs
    udp.beginPacket(RemoteIpadr, remotePort);
    for (int i = 0; i < msgLength; i++) {
      char onechar = char_array[i];
      udp.write(int(onechar));
    }
    udp.endPacket();
    stime = millis();
  }
}
