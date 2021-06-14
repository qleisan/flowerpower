/*
- loop:
    - make measurement
    - connect wifi in low power mode (compare power consumption with normal mode)
    - send UDP with measurement in JSON format
    - shut off WiFi and go to deepSleep()

improvements:
    power consumption:
        - remove power LED (expected to consume 10mA out of 11mA consumed during deepSleep with wifi off and no sensor added)
        - Adafruit sensor seems to consume 5mA during system deepSleep. Can it be turned off programatically? No "end()" method...
            - check power consumption before and after seesaw "begin()"
        - if necessary add hardware (transisor) to turn off Adafruit sensor
        - investigate if battery voltage measurement (ADC) consumes power during deepSleep. If so can ADC be turned off
        - (Crypto chip seems to be powered off by default)

- add serial output when but only when connected to USB. Seems like Serial.println() doesn't block
    experimented with Serial.availableForWrite() but not needed(?)
- RTC is set by NTP at boot, make more robust (multiple attempts at boot) and update at regular intervalls (make measurement of drift?)
- use compile time info (build time)
- use unique ID if one exist in the system e.g. crypro chip or CPU
- check WiFi module version vs latest when connected to serial
- check Adafruit sensor firmware when connected to serial
- Adafruit sensor seems to draw ~5 mA, no way to shut down? Need to add transistor??
- Current during sleep sometimes comes into a toggeling between 16 and 20mA is this the sensor?
- transmit status information together with data
- accumelate data that is sent in batches

*/

#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "ArduinoLowPower.h"
#include <RTCZero.h>
#include "helpers.h"
#include "Adafruit_seesaw.h"

#include "arduino_secrets.h"

int status = WL_IDLE_STATUS;
char  buffer[256];

WiFiUDP Udp;
unsigned int localPort = 2390;
IPAddress remoteIp = {192, 168, 0, 23};
RTCZero rtc;
unsigned long epoch = 0;
unsigned long NTP_epoch;
Adafruit_seesaw ss;
unsigned long int counter = 0;
boolean sensor_attached = false;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(115200);
    delay(2000); // is this necessary to get a printout
    Serial.println("Build time");
    Serial.println(__DATE__);
    Serial.println(__TIME__);

    if (!ss.begin(0x36)) {
        Serial.println("No sensor attached");
        sensor_attached = false;
    } else {
        Serial.print("seesaw started, version: ");
        Serial.println(ss.getVersion(), HEX);
        sensor_attached = true;
    }

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
    rtc.setEpoch(epoch);
}

void loop() {
    // make measurement
    float tempC = 0.0;
    uint16_t capread = 0;
    if (sensor_attached) {
        tempC = ss.getTemp();
        capread = ss.touchRead(0);
    }

    Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
    Serial.print("Capacitive: "); Serial.println(capread);

    // read battery
    int adcReading = analogRead(ADC_BATTERY);

    status = WL_IDLE_STATUS;    // best choice?
    while (status != WL_CONNECTED) {
        status = WiFi.begin(ssid, pass);
        WiFi.lowPowerMode(); // is this ok here or after it has connected?
        delay(5000);
    }
    Udp.begin(localPort);
    // check if less than jan 1 2021, then NTP time not yet successful
    if (epoch < 1609459200UL) {
        NTP_epoch = getNTP(Udp);
        if (NTP_epoch > 0) {
            rtc.setEpoch(NTP_epoch);
        }
    }
    blinkLED(2);
    
    Udp.beginPacket(remoteIp, 19988);
    epoch = rtc.getEpoch();
    generateJSON(counter, epoch, capread, adcReading, buffer);
    Udp.write(buffer);
    Udp.endPacket();
    delay(1000); // some delay seems to be needed

    // max powersave
    WiFi.end();
    LowPower.deepSleep(10000);

    blinkLED(3);
    counter++;
}
