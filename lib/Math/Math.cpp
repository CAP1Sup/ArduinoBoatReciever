#include "Math.hpp"

const int array_size = 3;

int numReadings;
int readings[array_size]; //Change this number to the iterations needed
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total

void Math::initializeAverages(int iterations) {
        numReadings = iterations;

        if (array_size != iterations) {

        }

        for (int thisReading = 0; thisReading < numReadings; thisReading++) {
            readings[thisReading] = 0;
        }
    
}

double Math::average(double currentValue) {
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = currentValue;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
 
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  return (total / numReadings);
}