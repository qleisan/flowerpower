/*
- loop:
    - make measurement
    - connect wifi in low power mode (compare pwer consumption with normal mode)
    - send UDP with measurement in JSON format
    - shut off WiFi and go to deepSleep()

improvements:
- removed power LED to minimize power consumnption
- measure battery voltage (electronics needed? Must ensure no power is consumed in sleep mode)
- use RTC and send timestamp with data
- use NTP to set RTC time (at boot and at some regular intervall)
- use compile time info (build time)
- use unique ID (if one exist)
- check WiFi module version
- if connected to USB user serial communication to print info
- transmit status information together with data
- accumelate data that is sent in batches

*/

#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "ArduinoLowPower.h"

#include "arduino_secrets.h"

int status = WL_IDLE_STATUS;
char  buffer[] = "A message from Arduino";

WiFiUDP Udp;
unsigned int localPort = 2390;
IPAddress remoteIp = {192, 168, 0, 23};


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    if (WiFi.status() == WL_NO_MODULE) {
        while (true)
        {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(50);
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
        }
    }
    blinkLED(1);
}

void loop() {
    // make measurement

    status = WL_IDLE_STATUS;    // best choice?
    while (status != WL_CONNECTED) {
        status = WiFi.begin(ssid, pass);
        WiFi.lowPowerMode(); // is this ok here or after it has connected?
        delay(5000);
    }
    blinkLED(2);
    Udp.begin(localPort);
    Udp.beginPacket(remoteIp, 19988);
    Udp.write(buffer);
    Udp.endPacket();
    delay(1000); // some delay seems to be needed

    WiFi.end();
    LowPower.deepSleep(10000);
    blinkLED(3);
}


void blinkLED(int times)
{
  for(int i=0; i<5;i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
  delay(500);
  for (int i = 0; i<times; i++)
  {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
   
  }
}
