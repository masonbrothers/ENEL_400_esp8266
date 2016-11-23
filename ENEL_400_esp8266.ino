#define FIREBASE_HOST                                     "aquaponics-monitoring.firebaseio.com"
#define FIREBASE_AUTH                                     ""
#define WIFI_SSID                                         "airuc-guest"
#define DEVICE_NAME                                       "club01"


// Open Source Licences Used:
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

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>



//#define SERIAL_DEBUG_MODE
#define UNSIGNED_LONG_MAX                                 2147483647


struct VariableAndValue
{
  String variable;
  float value;
};

unsigned long counter = 0;

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID);
#ifdef SERIAL_DEBUG_MODE
  Serial.print("connecting to ");
  Serial.print(WIFI_SSID);
#endif
  while (WiFi.status() != WL_CONNECTED) {
#ifdef SERIAL_DEBUG_MODE
    Serial.print(".");
#endif
    delay(200);
  }
#ifdef SERIAL_DEBUG_MODE
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
#endif
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  counter = Firebase.getInt((String)DEVICE_NAME + "/" + counter);
}

void loop() {

  if (Serial.available()) {
    String stringRead = Serial.readString();
    /* if (stringRead == "N")
      deviceName = stringRead;
    */
    if (stringRead == "P")
    {
      Serial.print((String)Firebase.getInt(DEVICE_NAME "/u")); //get the pump should be on value
      counter++;
      if (counter == UNSIGNED_LONG_MAX)
        counter = 0;
      Firebase.set(DEVICE_NAME "/c", counter); // Set the counter.
    }
    else //if (deviceName != "")
    {
      VariableAndValue variableAndValue = getVariableAndValue(stringRead);
      Firebase.set(DEVICE_NAME "/" + variableAndValue.variable + "/" + (String)counter, variableAndValue.value);
    }
  }
  delay(100);
}

VariableAndValue getVariableAndValue(String input)
{
  VariableAndValue output;
  int colonLocation = input.indexOf(":");
  if (colonLocation == -1 || colonLocation + 1 == input.length())
  {
    output.variable = DEVICE_NAME "/e"; // e is for error
    output.value = 1;
    return output;
  }
  output.variable = input.substring(0, colonLocation);
  output.value = input.substring(colonLocation + 1).toFloat();
  return output;
}

