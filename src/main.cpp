
#include <Arduino.h>
#include <PPM.hpp>
//#include <iBUStelemetry.h>
#include <Math.cpp>


int iBUS_pin = 4;
int throttle_pin = 12;

bool Serial_output = true;

int threshold = 15;
/*
const int numReadings = 3;
 
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
*/
float ch1 = 0;

/*
Mappings are as follows:
5 - ON/OFF switch - Potentiometer power adjustment | 25% power | Full power
6 - 2nd potentiometer - For hydrofoil adjustment

*/
Math math;
//PPM reciever;
//iBUStelemetry telemetry(iBUS_pin);

void applyThrottle() {
  
  
  /*
    // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = map(pulseIn(2, HIGH), 0, 2000, 0, 510) - 249;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
 
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  */
  
  ch1 = ( math.average ( map ( pulseIn ( 2, HIGH ), 0, 2000, 0, 510 ) - 249 ) ) - 1;
  
  float ch5 = map(pulseIn(3, HIGH), 0, 2000, 0, 255); // 1: 126 2: 189-190 3: 253-254

  if (ch5 < 180) { //1st position

    if (ch1 > threshold) {
      analogWrite( throttle_pin , ch1 ); //100% power
    }
    Serial.print(ch1);

  }

  else if (ch5 == 189 || ch5 == 190) { //2nd position

    if ( (ch1 * .50) > threshold) {
      analogWrite( throttle_pin , (ch1 * .50) ); //50% power
    }
    Serial.print(ch1 * .50);
    
  }

  else { //3rd position

    if ( (ch1 * .25) > threshold) {
      analogWrite( throttle_pin , ch1 * .25 ); //25% power
    }
    Serial.print(ch1 * .25);

  }

  if (Serial_output) {
    //Serial.print(ch1);
    //Serial.print(", ");
    //Serial.print(ch5);
    Serial.println();
  }

}








/*
void applyThrottle() {

  if (reciever.getValue(7) == 0 ) {

    analogWrite( throttle_pin, (reciever.getValue(1)*.25) ); //25% power

  }

  else if (reciever.getValue(7) < 0) {

    int variableThrottle = (reciever.getValue(1) * ( map(reciever.getValue(8), 0, 255, 0, 1) ));
    analogWrite( throttle_pin, variableThrottle );
    
  }

  else {

    analogWrite( throttle_pin, reciever.getValue(1) );

  }

}
*/


/*
void writeSensorData() {
  //Do some stuff to figure out telemetry
  telemetry.setSensorMeasurement(1, 1.2);

}
*/

void setup() {
  // put your setup code here, to run once:
  //reciever.begin(2,1);
  //telemetry.begin(115200);
  //telemetry.addSensor(0);
  if (Serial_output) {
    Serial.begin(115200);
  }
  /*
  //Initialize averaging stuff
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  */
  ch1 = map(pulseIn(2, HIGH), 0, 2000, 0, 510) - 250;
  math.initializeAverages(3);
}

void loop() {
  // put your main code here, to run repeatedly:
  applyThrottle();
}

