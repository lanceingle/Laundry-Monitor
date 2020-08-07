#ifndef Appliance_h
#define Appliance_h

#include "Arduino.h"

#define SECOND 1000
#define QUARTER_SECOND 250
#define DRIP_TRIP 300
#define MAX_BUCKET_SIZE 310

class Appliance
{
  public:
    Appliance(int pin);
    bool hasStopped();
    bool hasStarted();
  private:
    int _pin;
    bool machineRunning;
    bool lastState;
    int lastTripped;
    int tripBucket;
    int tripBucketLastDripped;
};

#endif
