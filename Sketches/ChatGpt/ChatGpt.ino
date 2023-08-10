#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <SPI.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* host = "max_msp_ip_address";  // Replace with Max/MSP's IP
const int oscPort = 12345;  // OSC port
char address[] = "/motion/1";
static int sensorPin = 2;

WiFiUDP udp;
OSCMessage oscMessage;

IPAddress ip(23, 0, 0, 2);
//destination IP
IPAddress outIp(23, 0, 0, 1);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
  }
  udp.begin(oscPort);
}

void loop() {

  int32_t sensorVal = digitalRead(sensorPin);
  
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14, true);

  //  int16_t accelerometerX = Wire.read() << 8 | Wire.read();
  //  // Read other accelerometer and gyroscope values similarly
  //
  //  // Create OSC message and add accelerometerX as an argument
  //  oscMessage.setupMessage("/gyroscope");
  //  oscMessage.add(accelerometerX);
  //  oscMessage.add(yourOtherSensorValue);
  //  // Add other sensor values similarly
  //  oscMessage.send(udp, host, oscPort);

  OSCMessage msg(address);
  msg.add((int32_t)sensorVal);

  Udp.beginPacket(outIp, outPort);
  msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message


  delay(1000);  // Adjust as needed
}
