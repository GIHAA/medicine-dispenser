#include <AccelStepper.h>

#define FULLSTEP 4
#define in4 8
#define in4 8
#define in4 8
#define in4 8

AccelStepper myStepper(FULLSTEP, in1 , in2 , in3 , in4 );

void setup() {
  Serial.begin(9600);
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(100.0);
  myStepper.setSpeed(200);
  myStepper.moveTo(1000);
}


void loop() {
	// Change direction once the motor reaches target position
	if (myStepper.distanceToGo() == 0) 
		myStepper.moveTo(-myStepper.currentPosition());

	// Move the motor one step
	myStepper.run();
}