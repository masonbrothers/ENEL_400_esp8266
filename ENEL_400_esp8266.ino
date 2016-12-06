#define FIREBASE_HOST                                     "aquaponics-monitoring.firebaseio.com"
#define FIREBASE_AUTH                                     "CKXyVCcy1n9XNYNNvEsbWzT9KSlexNa8VFM2k0Ch"
//#define WIFI_SSID                                       "airuc-guest"
#define WIFI_SSID "TR iPhone"
#define WIFI_PSK "mingiscool"

#define DEVICE_NAME                                       "club01"
//#define SERIAL_DEBUG_MODE
//#define DEBUG_WIFI_CONNECTION  //LEAVE THIS OFF UNLESS NEED MAC ADDRESS. THERE IS A BUG THAT WILL NOT ALLOW TO WRITE OUT TO FIREBASE.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define UNSIGNED_LONG_MAX                                 2147483647


struct VariableAndValue
{
  String variable;
  float floatValue;
  int intValue;
};

unsigned long counter = 0;

/*
void setup() {
  Serial.begin(9600);

  // connect to wifi.
#ifdef WIFI_PSK
  WiFi.begin(WIFI_SSID, WIFI_PSK);
#else
  WiFi.begin(WIFI_SSID);
#endif

#ifdef SERIAL_DEBUG_MODE
  Serial.print("connecting to ");
  Serial.print(WIFI_SSID);
#endif
  while (WiFi.status() != WL_CONNECTED) 
  {
#ifdef SERIAL_DEBUG_MODE
    Serial.print(".");
#endif
    delay(200);
  }
#ifdef SERIAL_DEBUG_MODE
  Serial.println();
  Serial.print("Connected: ");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC: ");
  Serial.println(getMACAddress());
#endif
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println(Firebase.success());
  delay(1000);
  
  counter = Firebase.getInt(DEVICE_NAME "/c");
  if (!Firebase.success())
    counter = 0;
#ifdef SERIAL_DEBUG_MODE
  Serial.println("Get Count" + (String)Firebase.success());
#endif
  
  Firebase.set("MASON", 1234);
  
  Serial.println("TESTING!!!" + (String)Firebase.success());
}
*/
void setup() {
  Serial.begin(9600);

  // connect to wifi.
#ifdef WIFI_PSK
  WiFi.begin(WIFI_SSID, WIFI_PSK);
#else
  WiFi.begin(WIFI_SSID);
#endif
#ifdef SERIAL_DEBUG_MODE
  Serial.println("connecting to " + (String)WIFI_SSID);
#endif
  while (WiFi.status() != WL_CONNECTED) {
#ifdef SERIAL_DEBUG_MODE
    Serial.print(".");
#endif
    delay(200);
  }
#ifdef SERIAL_DEBUG_MODE
  Serial.println();
  Serial.print("Connected: ");
#ifdef DEBUG_WIFI_CONNECTION
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC: ");
  Serial.println(getMACAddress());
#endif
#endif

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  counter = Firebase.getInt(DEVICE_NAME "/c");
  delay(7000);
  while (!Firebase.success())
  {
    counter = Firebase.getInt(DEVICE_NAME "/c");
    delay(4000);
  }
  Serial.flush();
}



/*
void loop() {

  if (Serial.available()) {
    String test = Serial.readString();
    int x = test.toInt();
    Serial.println(test);
    String path = (String)"realTimeAmbientLight";
    Firebase.set(path, x);
  }
}
*/
void loop() {
  if (Serial.available()) {
    String stringRead = Serial.readString();
    //if (stringRead == "N")
    //  deviceName = stringRead;

    if (stringRead.length() >= 1)
    {
      /*
      if (stringRead[0] == 'U')
      {
        boolean checkingPumpOn = (boolean)Firebase.getInt(DEVICE_NAME "/u"); //get the pump should be on value
        if (Firebase.success())
          Serial.print((String)checkingPumpOn); 
        counter++;
        if (counter == UNSIGNED_LONG_MAX)
          counter = 0;
        Firebase.set(DEVICE_NAME "/c", counter); // Set the counter.
      }
      else
      {
      */
        VariableAndValue variableAndValue = getVariableAndValue(stringRead);
  #ifdef SERIAL_DEBUG_MODE
        Serial.println(DEVICE_NAME "/" + variableAndValue.variable + "/" + (String)counter);
        Serial.println(variableAndValue.floatValue);
        Serial.println(variableAndValue.intValue);
  #endif
        if (variableAndValue.variable == "t")
          Firebase.set(DEVICE_NAME "/" + variableAndValue.variable + "/" + (String)counter, variableAndValue.intValue);
        else if (variableAndValue.variable == "U")
        {
          boolean checkingPumpOn = (boolean)Firebase.getInt(DEVICE_NAME "/u"); //get the pump should be on value
          if (Firebase.success())
            Serial.print((String)checkingPumpOn); 
          counter++;
          if (counter == UNSIGNED_LONG_MAX)
            counter = 0;
          Firebase.set(DEVICE_NAME "/c", counter); // Set the counter.
        }
        else
          Firebase.set(DEVICE_NAME "/" + variableAndValue.variable + "/" + (String)counter, variableAndValue.floatValue);
      //} //end else
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
    output.variable = "e"; // e is for error
    output.floatValue = 1;
    output.intValue = 1;
    return output;
  }
  output.variable = input.substring(0, colonLocation);
  output.floatValue = input.substring(colonLocation + 1).toFloat();
  output.intValue = input.substring(colonLocation + 1).toInt();
  return output;
}

String getMACAddress()
{
  WiFi.mode(WIFI_AP);
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID;
  for (int i = (WL_MAC_ADDR_LENGTH - 1); i >= 0; i--)
  {
    macID += String(mac[WL_MAC_ADDR_LENGTH - i], HEX);
    if (i != 0)
      macID += ":";
  }
  
  macID.toUpperCase();
  return macID;
}

