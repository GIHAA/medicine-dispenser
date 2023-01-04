#include <AccelStepper.h>
#include <Servo.h> 

#define FULLSTEP 4
#define in1 8
#define in2 8
#define in3 8
#define in4 8
#define openPosition 90
#define closePosition 0
#define openTime 1000

char in;
int targetHour;
int targetMinute;
Servo myservo; 

AccelStepper myStepper(FULLSTEP, in1 , in2 , in3 , in4 );

void setup() {
  Serial.begin(9600);
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(100.0);
  myStepper.setSpeed(200);
  myStepper.moveTo(1000);
    myservo.attach(9);
  Time.begin();
}


void loop() {
	// Change direction once the motor reaches target position
	if (myStepper.distanceToGo() == 0) 
		myStepper.moveTo(-myStepper.currentPosition());

	// Move the motor one step
	myStepper.run();

    if (Serial.available() > 0) 
        in = Serial.read();

    if (in == 'a') {
        Time.zone(+8);
        setTime(12, 0, 0, 1, 1, 2020);  // Set the time to January 1, 2020 12:00:00 
    }

    if ( in == 'b'){
        targetHour = 12; // set the hour you want to open the door
        targetMinute = 0; // set the minute you want to open the door
    }

    Alarm.alarmRepeat(targetHour, targetMinute, 0, openDoor);

}

void openDoor(){
    myservo.write(openPosition);
    delay(openTime);
    myservo.write(closePosition);
}