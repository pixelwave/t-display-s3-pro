#include "ui.h"
#include <ArduinoJson.h> // to send JSON objects via http
#include <HTTPClient.h>

String baseUrl = "http://192.168.178.59/";  // Replace IP of the WLED server

// Initialzie Variables
int delayTimeMS = 3000;
int relay1State;
int relay2State;
int relay3State;
int relay4State;

void updateStates() {
  HTTPClient http;
  String url = baseUrl + "relays";
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      String jsonString = payload;
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, jsonString);
      if (error) {
        Serial.print("Error parsing JSON: ");
        Serial.println(error.c_str());
        return;
      }
      relay1State = doc["0"];
      relay2State = doc["1"];
      relay3State = doc["2"];
      relay4State = doc["3"];
    }
  } else {
    Serial.println("Error on HTTP request");
  }
  http.end();

  if (relay1State == 1) {
    lv_obj_add_state(ui_btnLED, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(ui_btnLED, LV_STATE_CHECKED);
  }
  if (relay3State == 1) {
    lv_obj_add_state(ui_btnLow, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(ui_btnLow, LV_STATE_CHECKED);
  }
  if (relay4State == 1) {
    lv_obj_add_state(ui_btnHigh, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(ui_btnHigh, LV_STATE_CHECKED);
  }
}

void httpRequest(String url) {
  HTTPClient http;
  url = baseUrl + url;
  http.begin(url);
  int httpCode = http.sendRequest("GET");
  http.end();
}

void jsonRequest(String jsonData) {
  HTTPClient http;
  String url = baseUrl + "json/state";  // change to "json" for longer response

  // Send the POST request with JSON data in the body
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(jsonData);

  // Check for a successful POST request
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
  } else {
    Serial.println("Error in POST request");
  }

  http.end();
}

// Function definitions for UI
extern "C" void toggleRelay1() {
  // httpRequest("relays?toggle=1,0,0,0");
  // updateStates();
}
extern "C" void toggleRelay2() {
  // httpRequest("relays?toggle=0,1,0,0");
  // updateStates();
}
extern "C" void toggleRelay3() {
  // httpRequest("relays?toggle=0,0,1,0");
  // updateStates();
}
extern "C" void toggleRelay4() {
  // httpRequest("relays?toggle=0,0,0,1");
  // updateStates();
}
extern "C" void brightness(int value) {
  // httpRequest("win&A=" + String(value));
}
extern "C" void color(int angle) {
  // // Normalize the angle to be between 0 and 360
  // angle = angle % 360;
  // int red, green, blue;

  // // Define color transitions based on the angle
  // if (angle < 120) {
  //   red = map(angle, 0, 120, 255, 0);
  //   green = map(angle, 0, 120, 0, 255);
  //   blue = 0;
  // } else if (angle < 240) {
  //   red = 0;
  //   green = map(angle, 120, 240, 255, 0);
  //   blue = map(angle, 120, 240, 0, 255);
  // } else {
  //   red = map(angle, 240, 360, 0, 255);
  //   green = 0;
  //   blue = map(angle, 240, 360, 255, 0);
  // }
  
  // httpRequest("win&R="+String(red)+"&G="+String(green)+"&B="+String(blue)+"");

}

// HTTP Request String Library
/*
[ipadress]/win  -> basic URL scheme
&A=             -> 0 to 255	Master brightness
&T=             -> 0, 1, or 2	Master Off/On/Toggle
&FX=            -> 0 to 101	LED Effect Index
&R= &G= &B=     -> Red, Green, Blue values 0..255
&SX=            -> 0 to 255	Effect Speed

Multi-Relay (send with "GET" instead of "HEAD" !!!):
total of 4 relays, relay 2 will be toggled: http://[device-ip]/relays?toggle=0,1,0,0
total of 3 relays, relay 1&3 will be switched on: http://[device-ip]/relays?switch=1,0,1
*/

// JSON Request String Library
/*
{on:false}    -> turn light off
{bri:50}      -> brightness 1..255
{ps:5}        -> 1~17 can be used to iterate through presets 1-17.
*/