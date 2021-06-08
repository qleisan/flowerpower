
// the defines don't work?
#ifndef HELPERS_H
#define HELPERS_H

void blinkLED(int times);
void generateJSON(unsigned long int ep, int battery, char * buffer);
unsigned long getNTP(WiFiUDP& Udp);

#endif

