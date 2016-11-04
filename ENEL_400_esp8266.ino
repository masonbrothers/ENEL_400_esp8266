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

// FirebaseRoom_ESP8266 is a sample that demo using multiple sensors
// and actuactor with the FirebaseArduino library.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "aquaponics-monitoring.firebaseio.com"
#define FIREBASE_AUTH ""
#define WIFI_SSID "airuc-guest"

struct VariableAndValue
{
  String variable;
  double value;
};

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID);
  Serial.println("connecting to " + WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int i = 0;

void loop() {

  if (Serial.available()) {
    String test = Serial.readString();
    int x = test.toInt();
    Serial.println(test);
    String path = (String)"realTimeAmbientLight";
    Firebase.set(path, x);
  }
}

VariableAndValue getVariableAndValue(String input)
{
  VariableAndValue output;
  return output;
}

