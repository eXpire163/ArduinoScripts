#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#define SERIAL_BAUDRATE 115200
fauxmoESP fauxmo;
// -----------------------------------------------------------------------------
// Wifi definieren
// -----------------------------------------------------------------------------
#define WIFI_SSID "Schattenpinki"
#define WIFI_PASS "cvbnmyxc"
// -----------------------------------------------------------------------------
// PINs definieren
// -----------------------------------------------------------------------------
#define RELAY_PIN1 13
#define RELAY_PIN2 15
void wifiSetup() {
// Set WIFI module to STA mode
WiFi.mode(WIFI_STA);
// Verbinden
Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
WiFi.begin(WIFI_SSID, WIFI_PASS);
// Warten
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(100);
}
Serial.println();
// Verbunden!
Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}
void callback(uint8_t device_id, const char * device_name, bool state) {
Serial.printf("[MAIN] %s state: %s\n", device_name, state ? "ON" : "OFF");
// -----------------------------------------------------------------------------
// Abfragen definieren
// -----------------------------------------------------------------------------
if ( (strcmp(device_name, "TEST1") == 0) ) {
// adjust the relay immediately!
if (state) {
digitalWrite(RELAY_PIN1, HIGH);
} else {
digitalWrite(RELAY_PIN1, LOW);
}
}
if ( (strcmp(device_name, "TEST2") == 0) ) {
// adjust the relay immediately!
if (state) {
digitalWrite(RELAY_PIN2, HIGH);
} else {
digitalWrite(RELAY_PIN2, LOW);
}
}
}
void setup() {
// -----------------------------------------------------------------------------
// PINs setzen
// -----------------------------------------------------------------------------
pinMode(RELAY_PIN1, OUTPUT);
digitalWrite(RELAY_PIN1, LOW);
pinMode(RELAY_PIN2, OUTPUT);
digitalWrite(RELAY_PIN2, LOW);
// Init serial port and clean garbage
Serial.begin(SERIAL_BAUDRATE);
Serial.println();
Serial.println();
Serial.println("FauxMo Relay Setup");
Serial.println("Nach der Verbindung sage: Alexa schalte test / test2 an / aus .");
// Wifi
wifiSetup();
// -----------------------------------------------------------------------------
// Endgeraete Namen definieren
// -----------------------------------------------------------------------------
// Fauxmo
fauxmo.addDevice("TEST1");
fauxmo.addDevice("TEST2");
fauxmo.onMessage(callback);
}
uint8 _tj = 0; 
void loop() {
fauxmo.handle();
}
