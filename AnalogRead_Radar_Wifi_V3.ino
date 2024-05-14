/* 
 *  Arduino 1.8.19  , ESP32 Der module
 *  
 *  21-03-2567  AnalogRead_Radar_V1
 *  14-5-2567   AnalogRead_Radar_Wifi_V3  , ESP32 to https://netpie.io/
 *  
 */
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
//#include "ArduinoJson.h"
//Topic การเขียนข้อมูล Shadow
#define UPDATEDATA   "@shadow/data/update"

 //กำหนดข้อมูลเชื่อมต่อ WiFi
const char* ssid      = "xxxxxxxxxxxxx";     //ต้องแก้ไข 
const char* password  = "xxxxxxxxxxxxx";     //ต้องแก้ไข  IP address

//กำหนดข้อมูลเชื่อมต่อ NETPIE2020 นำ KEY ที่ได้จาก NETPIE มาใส่
const char* mqtt_server = "broker.netpie.io";
const char* client_id = "xxxxxxxxxxxxx";//ต้องแก้ไข
const char* token     = "xxxxxxxxxxxxx";    //ต้องแก้ไข
const char* secret    = "xxxxxxxxxxxxx";   //ต้องแก้ไข

WiFiClient espClient;
PubSubClient client(espClient);
uint8_t ledDD = 10; 

const unsigned long eventInterval = 8000;             // อ่านค่า  sensor ทุก ๆ 5 วินาที
unsigned long previousTime = 0;

int analogValue[500];
float analogValueff[500];
float analogValuefL[500];
unsigned long timett[500];
unsigned long timess;
unsigned long timessOld;
unsigned long microseconds;

long power;  // bin number
long voltage;  // total number of items
long ampere;
bool initialized = 0;  // flag to initialize variables
char msg[100];
String datastr;

//ฟังก์ชั่น เชื่อมต่อ WiFi
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(""));
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}
// ฟังก์ชั่น ตรวจสอบการเชื่อมต่อ และเชื่อมต่อ NETPIE2020
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    // Attempt to connect
    if (client.connect(client_id, token, secret)) {
      Serial.println(F("connected"));
    // ... and resubscribe from server
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print(F("Message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  // Set the CPU frequency to 80 MHz for power optimization 240, 160, 80    <<< For all XTAL types
  setCpuFrequencyMhz(240); 
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);
  ledcAttachPin(4, 2);//ledG = 4;
  ledcSetup(2, 12000, 8);// 12 kHz PWM, 8-bit resolution
  ledcWrite(2, ledDD);
    // เชื่อมต่อ WiFi
  setup_wifi();
  // ตั่งค่า Broker Netpie และพอร์ต 
  client.setServer(mqtt_server, 11);
  client.setCallback(callback);
  
  Serial.println("analogValue V2");
  timessOld = micros();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

   //timess = micros() - timessOld;//overflow (go back to zero), after approximately 70 minutes
/////////////////////1900/////////////////////////////////////////////
// อ่านค่าอุณหภูมิ และความชื้น
    float temp = 1.23;
    float humid = 2.34;
    float dustDensity = 3.65;
    
int readADC = analogRead(36);
microseconds = micros();
 for (int i = 0; i <= 500; i++) {
   //timett[i] = micros();
   timett[i] = microseconds; 
   analogValue[i] = analogRead(36) - 1926;//1900 ,,
   //analogValueff[i] = analogValueff[i] + (analogValue[i] - analogValueff[i])*0.5421765;//Low pass filter
   analogValueff[i] = (analogValueff[i-1] + analogValue[i] - analogValue[i-1])*0.8621765;//high-pass filter
   analogValuefL[i] = analogValuefL[i-1] + (analogValueff[i] - analogValuefL[i-1])*0.4421765;//Low pass filter
   //y[i] := α × (y[i−1] + x[i] − x[i−1]) high-pass filter
   if(abs(analogValuefL[i]) > 80){
    //ledcWrite(2, abs(analogValuefL[i])*0.254);
   }
     while(micros() - microseconds < 1000){///157 us // 200 us = 0.2 ms = 5000 Hz
        //empty loop
      }
      microseconds += 1000;
 }

//////////////// print out the values you read:///// 1000 us = 1 ms = 1000 Hz
  unsigned long currentTime = millis()/1000;
  //if (currentTime - previousTime > 40000) {
  delay(4000);
  power++; // bin is incremented by 1
  ampere = power+2;
  voltage+=random(1,25); // random number between 1 - 4
  if(power > 255){
    power = 0;
  }
    if(voltage > 255){
    voltage = 0;
  }
  datastr = "{\"data\": {\"power\":" + String(power) + ", \"voltage\":" + String(voltage) + 
  ", \"voltage\":" + String(ampere) +
  ", \"Time\":" + String(currentTime) +"}}";
  
  Serial.println(datastr);
  datastr.toCharArray(msg, (datastr.length() + 1));
  client.publish("@shadow/data/update", msg);
  //previousTime = currentTime;
    
     for (int i = 30; i <= 32; i++) {
      if(analogValuefL[i] > 80){//160 120 60
          for (int j = i; j <= 34; j++) {
              if(abs(analogValuefL[i]) > 80){
    ledcWrite(2, abs(analogValuefL[i])*0.254);
   }
          //ledcWrite(2, abs(analogValue[j])*0.254); 
          Serial.print(j);Serial.print("\t");
          //Serial.print(timett[j]);Serial.print("\t");
          //Serial.print(analogValueff[j]);Serial.print("\t");
          Serial.print(analogValuefL[j]);Serial.print("\t");
          Serial.println(analogValue[j]);
          }
          i = 34;
          Serial.println(" ");
      }
  //Serial.print(timess);Serial.print("\t");
  
 }

  //Serial.printf("ADC analog value = %d\n",analogValue);
  //Serial.printf("ADC millivolts value = %d\n",analogVolts);
 
for (int i = 0; i <= 500; i++) {
   analogValue[i] = 0;
   analogValueff[i] = 0;
   analogValuefL[i] = 0;
   ledcWrite(2, 0);
}
//////////////////////////////////////////////////////////////////
}
