/*
- loop:
    - make measurement
    - connect wifi in low power mode (compare power consumption with normal mode)
    - send UDP with measurement in JSON format
    - shut off WiFi and go to deepSleep()

improvements:
- removed power LED to minimize power consumnption
- add loop counter?
- measure battery voltage (electronics needed? Must ensure no power is consumed in sleep mode)
- add serial output when connected to USB 
- use RTC and send timestamp with data (check if affected by deepSleep)
- use NTP to set RTC time (at boot and at some regular intervall). Requires rtc to survive deep sleep... (if not how much does it consume?)
- there is a "WiFi.getTime()" function!
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
#include <RTCZero.h>
#include "helpers.h"

#include "arduino_secrets.h"

int status = WL_IDLE_STATUS;
char  buffer[256];

WiFiUDP Udp;
unsigned int localPort = 2390;
IPAddress remoteIp = {192, 168, 0, 23};
RTCZero rtc;
unsigned long epoch = 0;

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
    rtc.begin();
}

void loop() {
    // make measurement
    int adcReading = analogRead(ADC_BATTERY);

    status = WL_IDLE_STATUS;    // best choice?
    while (status != WL_CONNECTED) {
        status = WiFi.begin(ssid, pass);
        WiFi.lowPowerMode(); // is this ok here or after it has connected?
        delay(5000);
    }
    Udp.begin(localPort);
    if (epoch == 0) {
        epoch = getNTP(Udp);
        rtc.setEpoch(epoch);
    }
    blinkLED(2);
    
    Udp.beginPacket(remoteIp, 19988);
    epoch = rtc.getEpoch();
    generateJSON(epoch, adcReading, buffer);
    Udp.write(buffer);
    Udp.endPacket();
    delay(1000); // some delay seems to be needed

    WiFi.end();
    LowPower.deepSleep(10000);
    blinkLED(3);
}
