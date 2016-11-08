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
#define FIREBASE_AUTH "CKXyVCcy1n9XNYNNvEsbWzT9KSlexNa8VFM2k0Ch"
#define WIFI_SSID "airuc-guest"

struct VariableAndValue
{
  String variable;
  float value;
};

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID);
  Serial.print("connecting to ");
  Serial.print(WIFI_SSID);
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
    String stringRead = Serial.readString();
    if (stringRead == "PUMP")
    {
      Serial.print((String)Firebase.getInt("pumpShouldBeOn"));
    }
    else
    {
      VariableAndValue variableAndValue = getVariableAndValue(stringRead);
      Firebase.set(variableAndValue.variable, variableAndValue.value);
    }
  }
}

VariableAndValue getVariableAndValue(String input)
{
  VariableAndValue output;
  int colonLocation = input.indexOf(":");
  if (colonLocation == -1 || colonLocation + 1 == input.length())
  {
    output.variable = "Error";
    output.value = 1;
    return output;
  }
  output.variable = input.substring(0, colonLocation);
  output.value = input.substring(colonLocation + 1).toFloat();
  return output;
}

