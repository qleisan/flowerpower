
// the defines don't work?
#ifndef HELPERS_H
#define HELPERS_H

void blinkLED(int times);
void generateJSON(unsigned long int counter,
                  unsigned long int ep,
                  int meas,
                  int battery1,
                  int battery2,
                  int battery3,
                  char * buffer);
unsigned long getNTP(WiFiUDP& Udp);

#endif

