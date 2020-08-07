#include "Arduino.h"
#include "Appliance.h"

Appliance::Appliance(int pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
  machineRunning = false;
  lastState = false;
  lastTripped = 0;
  tripBucket = 0;
  tripBucketLastDripped = 0;
}

bool Appliance::hasStarted()
{
  return machineRunning;
}

bool Appliance::hasStopped()
{
  int now = millis();
  int sinceLastTripped = now - lastTripped;
  int sinceLastDrip = now - tripBucketLastDripped;

  if (tripBucket > 0 && sinceLastDrip > SECOND) {
    tripBucket--;
    tripBucketLastDripped = now;
    Serial.print(_pin);
    Serial.print(": ");
    Serial.println(tripBucket);
  }

  // Read the state and see if the sensor was tripped
  bool state = digitalRead(_pin) == 0 ? false : true;
  if (lastState != state) {
    lastState = state;

    // Can be tripped a maximum of once per second
    if (sinceLastTripped > QUARTER_SECOND) {
      lastTripped = now;

      if (tripBucket < MAX_BUCKET_SIZE) {
        tripBucket++;
      }
    }
  }

  if (machineRunning && tripBucket == 0) {
    machineRunning = false;
    return true;
  }

  if (!machineRunning && tripBucket > DRIP_TRIP) {
    machineRunning = true;
  }

  return false;
}
