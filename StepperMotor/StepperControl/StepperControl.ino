/*********************************************************************
 * 
 *   /$$$$$$            /$$                                        
 *  /$$__  $$          |__/                                        
 * | $$  \__/  /$$$$$$$ /$$  /$$$$$$  /$$$$$$$   /$$$$$$$  /$$$$$$ 
 * |  $$$$$$  /$$_____/| $$ /$$__  $$| $$__  $$ /$$_____/ /$$__  $$
 *  \____  $$| $$      | $$| $$$$$$$$| $$  \ $$| $$      | $$$$$$$$
 *  /$$  \ $$| $$      | $$| $$_____/| $$  | $$| $$      | $$_____/
 * |  $$$$$$/|  $$$$$$$| $$|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$$
 *  \______/  \_______/|__/ \_______/|__/  |__/ \_______/ \_______/
 * 
 * Shenzhen Science Technology Co. LTD Example Code
 *
 * [Title]     stepper motor  .
 * [Diagram]
 *         Arduino PIN 8   ===================  Stepper motor A
 *         Arduino PIN 9   ===================  Stepper motor B
 *         Arduino PIN 10  ===================  Stepper motor C
 *         Arduino PIN 11  ===================  Stepper motor D
*/

#include "KW_Stepper.h"
#define STEPS 64
Stepper stepper(STEPS, 8, 9, 10, 11);

// the previous reading from the analog input
int previous = 0;

void setup() {
  // set the speed of the motor to 200 RPMs
  stepper.setSpeed(200);
  pinMode(A0, INPUT);
}

void loop() {
  // get the sensor value
  int val = analogRead(A0);

  // move a number of steps equal to the change in the
  // sensor reading
  stepper.step(val - previous);

  // remember the previous value of the sensor
  previous = val;
}
