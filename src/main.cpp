#include <Arduino.h>
#include <PPM.hpp>
#include <iBUStelemetry.h>


int iBUS_pin = 4;
int throttle_pin = 3;

/*
Mappings are as follows:
7 - ON/OFF switch - Potentiometer power adjustment | 25% power | Full power
8 - 2nd potentiometer - For power adjustment

*/

PPM reciever;
iBUStelemetry telemetry(iBUS_pin);

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

void writeSensorData() {
  //Do some stuff to figure out telemetry
  telemetry.setSensorMeasurement(1, 1.2);

}

void setup() {
  // put your setup code here, to run once:
  reciever.begin(2,1);
  telemetry.begin(115200);
  telemetry.addSensor(0);

}

void loop() {
  // put your main code here, to run repeatedly:
  applyThrottle();
  writeSensorData();
}