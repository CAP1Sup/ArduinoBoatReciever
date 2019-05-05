#include <iBUStelemetry.h>

iBUStelemetry telemetry(4);

byte i;
int voltage;
int sensorValue;
int value;


void setup(){
  telemetry.begin(115200);
  telemetry.addSensor(0);
  Serial.begin(300);
  //pinMode(13, INPUT);  
}

void loop(){
  
  if (Serial.available()) {
    voltage = Serial.read();
    Serial.println(voltage);
    // send a byte to the software serial port
    telemetry.setSensorMeasurement(1, voltage);
    
  }
  //Serial.write((int)(voltage*10.0));
  //Serial.println(digitalRead(13));
  
  delay(1000);
}
