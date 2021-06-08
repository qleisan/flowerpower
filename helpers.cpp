#include <Arduino.h>
#include "helpers.h"

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

void generateJSON(unsigned long int ep, int battery, char * buffer)
{
    sprintf(buffer, "{\"time\":\"%lu\", \"meas\",567, \"batt\":%d}", ep, battery);
}
