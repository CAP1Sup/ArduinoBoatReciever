#include <Arduino.h>
#include <PPM.hpp>

PPM reciever;

int throttle_pin = 3;

/*
Mappings are as follows:
7 - ON/OFF switch - Potentiometer power adjustment | 25% power | Full power
8 - 2nd potentiometer - For power adjustment

*/

void applyThrottle() {

  if (reciever.getValue(7) > 0 && reciever.getValue(7) < 50) {

    analogWrite( throttle_pin, (reciever.getValue(1)*.25) );

  }

  else if (reciever.getValue(7) >= 50) {

    int variableThrottle = (reciever.getValue(1) * ( map(reciever.getValue(8), 0, 255, 0, 1) ));
    analogWrite( throttle_pin, variableThrottle );
    
  }

  else {

    analogWrite( throttle_pin, reciever.getValue(1) );

  }

}

void setup() {
  // put your setup code here, to run once:
  reciever.begin(2,1);

}

void loop() {
  // put your main code here, to run repeatedly:
  applyThrottle();

}