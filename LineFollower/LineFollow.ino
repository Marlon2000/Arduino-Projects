#include <Wire.h>
#include <Zumo32U4.h>
//declaration
Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
//initialises sensor 
uint16_t lineSensorValues[5] = { 0, 0, 0, 0, 0 };
// defines whitch sensor is used
bool proxLeftActive;
bool proxFrontActive;
bool proxRightActive;

void setup()
{

//waits for someone to press the A button
  buttonA.waitForButton();
   //initializes the sensors
  lineSensors.initFiveSensors();
  proxSensors.initFrontSensor();

  calibrateLineSensors();
}

//to calibrate the sensors, so they are accurate
void calibrateLineSensors()
{
  
  ledYellow(1);
  lcd.clear();
  lcd.print(F("Line cal"));
//calibrates for 4 seconds
  for (uint16_t i = 0; i < 400; i++)
  {
    lcd.gotoXY(0, 1);
    lcd.print(i);
    lineSensors.calibrate();
  }

  ledYellow(0);
  lcd.clear();
}



void loop() {
  // this code runs repeatedly
  lineSensors.readCalibrated(lineSensorValues);

//if it has no problem it drives
  if (lineSensorValues[3] > 400 && lineSensorValues[1] < 400) {
   
     motors.setLeftSpeed(321);
     motors.setRightSpeed(321);
    //if it is black it turns
  } else if (lineSensorValues[3] < 300) {
   //turns right if it senses black
    motors.setLeftSpeed(150);
    motors.setRightSpeed(-150);
     //turns left if it senses black
  } else if(lineSensorValues[1] > 300){
     motors.setLeftSpeed(-150);
    motors.setRightSpeed(150);
  }

 
}
