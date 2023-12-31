//////////////////////////////////////ADD LIBRARIES/////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include "GY521.h"

//////////////////////////////////////INITIALIZE VARIABLES//////////////////////////////////////////

#ifndef STASSID
#define STASSID "iPhone van Jonathan"
#define STAPSK "baardkrent"
#endif

IPAddress outIp(172, 20, 10, 2);
unsigned int localPort = 2323;
char address[] = "/voet/1";

char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];
char ReplyBuffer[] = "acknowledged\r\n";

GY521 sensor(0x68);
WiFiUDP Udp;

int sensorReading = 0;
const int knockSensor = A0;

//////////////////////////////////////INITIALIZE SETUP//////////////////////////////////////////

void setup() {

  Serial.begin(115200);

  // INITIALIZE WIFI MODULE
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Wire.begin();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);

  delay(100);

  // INITIALIZE SENSOR MODULE
  while (sensor.wakeup() == false)
  {
    Serial.print(millis());
    Serial.println("\tCould not connect to GY521");
    delay(1000);
  }
  sensor.setAccelSensitivity(2);  //  8g
  sensor.setGyroSensitivity(1);   //  500 degrees/s
  sensor.setThrottle();
  Serial.println("start...");
  
  sensor.axe = 0.574;
  sensor.aye = -0.002;
  sensor.aze = -1.043;
  sensor.gxe = 10.702;
  sensor.gye = -6.436;
  sensor.gze = -0.676;

}

//////////////////////////////////////RUN//////////////////////////////////////////

void loop() {

  // INITIALIZE SENSOR MODULE
  sensor.read();
  float gx = sensor.getAngleX();
  float gy = sensor.getAngleY();
  float gz = sensor.getAngleZ();

//  float ax = sensor.getAccelX();
//  float ay = sensor.getAccelY();
//  float az = sensor.getAccelZ();

  // COMPOSE OSC MESSAGE
  OSCMessage msg(address);
  msg.add(gx);
  msg.add(gy);
  msg.add(gz);

  sensorReading = analogRead(knockSensor);
  msg.add(sensorReading);

  // ACTUALLY SEND THE OSC MESSAGE
  Udp.beginPacket(outIp, localPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();

}
