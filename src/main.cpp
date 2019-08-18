//Library definitions
#include <Arduino.h>
//#include <Ramp.h>

//Use the debugging mode
//#define Serial_output
//Motor pin defintions
int L_motor_pin = 3;
int R_motor_pin = 5;
//Channel pin defintions
int ch1_pin = 4;
int ch3_pin = 2;
int ch5_pin = 6;
int ch6_pin = 7;
//Create reciever pin
int reciever_pin = 9;
//Initialize variables for channel values
float ch1 = 0; //Steering
float ch3 = 0; //Throttle
float ch5 = 0; //3-way switch
float ch6 = 0; //VrB
//Initialize variables to store motor values in after steering calculations
float L_motor_value = 0;
float R_motor_value = 0;
//Create count value for voltage readings
int count = 0;
//Create storage value for readVcc
long readVcc_value = 0;
//Create instances of the RAMP library
//rampFloat L_motor_ramp;
//rampFloat R_motor_ramp;

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
     ADMUX = _BV(MUX5) | _BV(MUX0) ;
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

void calculateThrottle() {
  ch1 = map ( pulseIn( ch1_pin, HIGH), 986, 1972, -100, 100 );// + 2; 
  ch3 = map ( pulseIn( ch3_pin, HIGH), 993, 1990, 0, 255 );
  //ch5 = int ( map ( pulseIn( ch5_pin, HIGH), 500, 1500, 0, 2 )); 
  //ch6 = map ( pulseIn( ch6_pin, HIGH), 994, 1988, 0, 1000 );

  if (ch1 < 3 || ch1 > -3) { //Steering is moved
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

  /*
  if (ch5 == 0) { //No ramping applied
    //L_motor_ramp.go( (L_motor_value), ch6, NONE );
    //R_motor_ramp.go( (R_motor_value), ch6, NONE );
  }

  else if (ch5 == 1) { //Linear ramp applied
    //L_motor_ramp.go( (L_motor_value), ch6, LINEAR );
    //R_motor_ramp.go( (R_motor_value), ch6, LINEAR );
  }

  else if (ch5 == 2) { //Quadratic ramp applied
    //L_motor_ramp.go( (L_motor_value), ch6, QUADRATIC_OUT );
    //R_motor_ramp.go( (R_motor_value), ch6, QUADRATIC_OUT );
  }
  */

  //L_motor_ramp.update();
  //R_motor_ramp.update();
}

void updateReadVcc() {
  if ( count > 4 ) {
    readVcc_value = readVcc();
  }
  count++;
}

void setup() {
  // put your setup code here, to run once:
  //Start Serial connection if needed
  #ifdef Serial_output 
    Serial.begin(9600);
  #endif
  //Set up pins as inputs
  pinMode( ch1_pin, INPUT );
  pinMode( ch3_pin, INPUT );
  pinMode( ch5_pin, INPUT );
  pinMode( ch6_pin, INPUT );

}


void loop() {
  // put your main code here, to run repeatedly:
  calculateThrottle();
  //Set readVcc variable
  //updateReadVcc();
  //Set pins to the values calculated if they are above the threshold
  //if ( L_motor_value > 2 ) {
  analogWrite( L_motor_pin, L_motor_value );
  //}
  //if ( R_motor_value > 2 ) {
  analogWrite( R_motor_pin, R_motor_value );
  //}
  //analogWrite( reciever_pin, readVcc_value );
  //Write out Serial data if enabled
  #ifdef Serial_output
    Serial.print( L_motor_value );
    Serial.print(", ");
    Serial.print( R_motor_value );
    //Serial.print(", ");
    //Serial.print( readVcc_value );
    Serial.println();
  #endif
}