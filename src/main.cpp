//Library definitions
#include <Arduino.h>
#include <Ramp.h>

//Use the debugging mode
bool Serial_output = true;
//Motor pin defintions
int L_motor_pin = 2;
int R_motor_pin = 3;
//Channel pin defintions
int ch1_pin = 4;
int ch3_pin = 5;
int ch5_pin = 6;
int ch6_pin = 7;
//Initialize variables for channel values
float ch1 = 0; //Steering
float ch1_raw = 0;
float ch3 = 0; //Throttle
float ch5 = 0; //3-way switch
float ch6 = 0; //VrB
//Initialize variables to store motor values in after steering calculations
float L_motor_value = 0;
float R_motor_value = 0;
//Create instances of the RAMP library
rampFloat L_motor_ramp;
rampFloat R_motor_ramp;



void calculateThrottle() {

  ch1_raw = pulseIn( ch1_pin, HIGH);
  ch1 = map ( ch1_raw, 986, 1972, -100, 100 );// + 2; 
  ch3 = map ( pulseIn( ch3_pin, HIGH), 993, 1990, 0, 255 );
  ch5 = int ( map ( pulseIn( ch5_pin, HIGH), 500, 1500, 0, 2 )); 
  ch6 = map ( pulseIn( ch6_pin, HIGH), 994, 1988, 0, 1000 );


  if (ch1 < 2 || ch1 > -2) { //Steering is moved
    //Steering Calculations
    if ( ch1 > 0 ) { //Stick moved to the right, therefore boat should turn right. This is done by slowing down the right motor.
      L_motor_value = ch3;
      R_motor_value = ch3 * ( 1.5 - ( 100 / ( 200 - abs(ch1) ) ) );
      if ( R_motor_value < 0 ) {
        R_motor_value = 0;
      }
    }
    else {  //Stick must have moved to the left, therefore boat should turn left. This is done by slowing down the left motor.
      L_motor_value = ch3 * ( 1.5 - ( 100 / ( 200 - abs(ch1) ) ) );
      if ( L_motor_value < 0 ) {
        L_motor_value = 0;
      }
      R_motor_value = ch3;
    }
    
  }
  else { //Don't apply steering
    L_motor_value = ch3;
    R_motor_value = ch3;
  }


  if (ch5 == 0) { //No ramping applied
    L_motor_ramp.go( (L_motor_value), ch6, NONE );
    R_motor_ramp.go( (R_motor_value), ch6, NONE );
  }

  else if (ch5 == 1) { //Linear ramp applied
    L_motor_ramp.go( (L_motor_value), ch6, LINEAR );
    R_motor_ramp.go( (R_motor_value), ch6, LINEAR );
  }

  else if (ch5 == 2) { //Quadratic ramp applied
    L_motor_ramp.go( (L_motor_value), ch6, QUADRATIC_OUT );
    R_motor_ramp.go( (R_motor_value), ch6, QUADRATIC_OUT );
  }

  L_motor_ramp.update();
  R_motor_ramp.update();
}

void setup() {
  // put your setup code here, to run once:
  //Start Serial connection if needed
  if (Serial_output) {
    Serial.begin(9600);
  }
  //Set up pins as inputs
  pinMode( ch1_pin, INPUT );
  pinMode( ch3_pin, INPUT );
  pinMode( ch5_pin, INPUT );
  pinMode( ch6_pin, INPUT );

}


void loop() {
  // put your main code here, to run repeatedly:
  calculateThrottle();
  //Set pins to the values calculated
  analogWrite( L_motor_pin, L_motor_value );
  analogWrite( R_motor_pin, R_motor_value );
  //Write out Serial data if enabled
  if (Serial_output) {
    Serial.print( L_motor_value );
    Serial.print(", ");
    Serial.print( R_motor_value );
    Serial.print(", ");
    Serial.print( 200 - abs(ch1) );
    Serial.print(", ");
    Serial.print( 1.5 - ( 100 / ( 200 - abs(ch1) ) ) );
    Serial.print(", ");
    Serial.print( ch1_raw );
    Serial.println();
  }
}