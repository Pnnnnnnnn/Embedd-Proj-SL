//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "eblsys-8d5a3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ZY9dhlmiotItjBunuDilFgz6zsPduH1FNw8vy2ps"
#define WIFI_SSID "AIS_2.4G"
#define WIFI_PASSWORD "412389AIS"

SoftwareSerial mySerial(13, 15);//RX TX
char rd;

void setup() {
  pinMode(D3, INPUT);
  pinMode(D4, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  while(!mySerial){
    ; // wait for serial port to cennect. Needed for native USB port only
  }
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {
  // begin reading from nucleo
  while(mySerial.available()>0){
    rd = mySerial.read();
    Serial.println(rd);
  }bool firstTable;
  bool secondTable;
  if(rd == '0'){
    firstTable = false;
    secondTable = false;
  }else if(rd == '1'){
    firstTable = false;
    secondTable = true;
  }else if(rd == '2'){
    firstTable = true;
    secondTable = false;
  }else if(rd == '3'){
    firstTable = true;
    secondTable = true;
  }
  // end reading -------------
  
  // set bool value 1-----------------
  Firebase.setBool("firstTable", firstTable);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // set bool value 2-----------------
  Firebase.setBool("secondTable", secondTable);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
//  Serial.print("pushed: /logs/");
//  Serial.println(name);
  delay(1000);
}
