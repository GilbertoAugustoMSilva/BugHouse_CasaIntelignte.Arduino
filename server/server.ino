//#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <DNSServer.h>
//#include <ESP8266WebServer.h>
#include <WiFiManager.h>         
#include "fauxmoESP.h"
#include <RCSwitch.h>

#include <HTTPClient.h>

#define RF_RECEIVER 2 
#define RELAY_PIN_1 16
#define RELAY_PIN_2 5
#define SERIAL_BAUDRATE 115200
#define WIFI_SSID "TR0J4N_H0RS3"
#define WIFI_PASS "369258147147Giba"
#define LAMP_1 "Luz Principal"
#define LAMP_2 "Ventilador"


String UrlApi ="http://localhost:50706/api/v1/postlastip?Oauth=MyKeyApiEndPoitn_ArduinoIpSetings";




bool Trehad = false;

fauxmoESP fauxmo;

RCSwitch mySwitch = RCSwitch();

void DispConfig() {


  WiFiManager wifiManager;

  wifiManager.resetSettings();

  wifiManager.autoConnect("QuartoGiba");

  Serial.println("connected...yeey :)");
}

void wifiSetup() {
  WiFi.mode(WIFI_STA);

  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {

  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  wifiSetup();

  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, HIGH);

  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_2, HIGH);

  mySwitch.enableReceive(RF_RECEIVER);  // Receiver on interrupt 0 => that is pin #2

  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices

  fauxmo.enable(true);

  fauxmo.addDevice(LAMP_1);
  fauxmo.addDevice(LAMP_2);

  fauxmo.onSetState([](unsigned char device_id, const char* device_name, bool state, unsigned char value) {

    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ((strcmp(device_name, LAMP_1) == 0)) {
      // this just sets a variable that the main loop() does something about
      Serial.println("RELAY 1 switched by Alexa");
      //digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
      if (state) {
        digitalWrite(RELAY_PIN_1, HIGH);

      }
      else {
        digitalWrite(RELAY_PIN_1, LOW);
      }
    }
    if ((strcmp(device_name, LAMP_2) == 0)) {
      // this just sets a variable that the main loop() does something about
      Serial.println("RELAY 2 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_2, HIGH);

      }
      else {
        digitalWrite(RELAY_PIN_2, LOW);

      }
    }
    });

}

void loop() {
  // fauxmoESP uses an async TCP server but a sync UDP server
  // Therefore, we have to manually poll for UDP packets
  fauxmo.handle();

  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }

  if (mySwitch.available()) {
    /*Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );*/
    if (mySwitch.getReceivedValue() == 6819768) {
      digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
    }
    if (mySwitch.getReceivedValue() == 9463928) {
      digitalWrite(RELAY_PIN_2, !digitalRead(RELAY_PIN_2));
    }
    delay(600);
    mySwitch.resetAvailable();
  }
}

void EnvioApiIp() {

  if (Trehad = false)
  {
    Trehad = true;

    do {
      if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

        HTTPClient http;

        http.begin(UrlApi ); //Specify the URL
        int httpCode = http.GET();  //Make the request

        if (httpCode > 0) { //Check for the returning code

          String payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload);
        }

        else {
          Serial.println("Error on HTTP request");
        }

        http.end(); //Free the resources
      }

      delay(30000);
    } while (true);
  }
}
