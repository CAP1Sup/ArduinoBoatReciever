
#include <Arduino.h>
#include <PPM.hpp>
#include <iBUStelemetry.h>


int iBUS_pin = 4;
int throttle_pin = 3;

bool Serial_output = true;


/*
Mappings are as follows:
5 - ON/OFF switch - Potentiometer power adjustment | 25% power | Full power
6 - 2nd potentiometer - For hydrofoil adjustment

*/

//PPM reciever;
//iBUStelemetry telemetry(iBUS_pin);

void applyThrottle() {
  float ch1 = map(pulseIn(2, HIGH), 0, 2000, 0, 255);
  float lastCh1 = 0;
  int resetCounts;
  float ch5 = map(pulseIn(3, HIGH), 0, 2000, 0, 255);

  ch1 = (ch1 + lastCh1) / 2;

  if (ch5 == 255) {

    analogWrite( throttle_pin , (ch1 * .25) ); //25% power

  }

  else if (ch5 == 0) {

    analogWrite( throttle_pin , (ch1 * .50) ); //50% power
    
  }

  else {

    analogWrite( throttle_pin , ch1 ); //100% power

  }

  if (Serial_output) {
    Serial.print(ch1);
    Serial.print(", ");
    Serial.print(ch5);
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
}

void loop() {
  // put your main code here, to run repeatedly:
  applyThrottle();
}

