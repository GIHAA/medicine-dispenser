#include <AccelStepper.h>
#include <Servo.h> 
#include <Time.h>

#define openPosition 90
#define closePosition 0
#define openTime 1000

String readString;
bool reset_incoming = false;
bool set_time_incoming = false;
int move_distance = 1000;
int EN = D1; // Enable pin for motor
int IN1 = D2; // Input pin 1 for motor
int IN2 = D3; /

Servo myservo;

void setup() {
  Serial.begin(9600);
  pinMode(EN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  myservo.attach(9);
  Time.begin();

}

void loop() {

  while (Serial.available()) {
    delay(10); 
    char c = Serial.read(); 
    if (c == ',') {
      break;
    }  
    readString += c; 
  } 

  if(reset_incoming == true){
    //Serial.println(readString); //prints string to serial port out

      int hour, min, sec, month, day, year;
      char dot;
      int pos = 0;

    hour = readString.substring(pos, readString.indexOf('.')).toInt();
    pos = readString.indexOf('.') + 1;
    min = readString.substring(pos, readString.indexOf('.', pos)).toInt();
    pos = readString.indexOf('.', pos) + 1;
    sec = readString.substring(pos, readString.indexOf('.', pos)).toInt();
    pos = readString.indexOf('.', pos) + 1;
    month = readString.substring(pos, readString.indexOf('.', pos)).toInt();
    pos = readString.indexOf('.', pos) + 1;
    day = readString.substring(pos, readString.indexOf('.', pos)).toInt();
    pos = readString.indexOf('.', pos) + 1;
    year = readString.substring(pos).toInt();
    
    Time.zone(+8);
    setTime(hour, min, sec, month, day, year); 
    readString=""; //clears variable for new readString
    reset_incoming = false;
  }

  if(set_time_incoming == true){

    int targetHour = 0;
    int targetMinute = 0;
    int targetSec = 0;
    char dot;
    int pos = 0;

    year = readString.substring(pos, readString.indexOf('.')).toInt();
    pos = readString.indexOf('.') + 1;
    targetMinute = readString.substring(pos, readString.indexOf('.', pos)).toInt();
    pos = readString.indexOf('.', pos) + 1;
    targetSec = readString.substring(pos, readString.indexOf('.', pos)).toInt();

    Alarm.alarmRepeat(targetHour, targetMinute, targetSec, alarm_goes_off);

    readString=""; //clears variable for new readString
    set_time_incoming = false;
    
  }

  if(readString == "reset_incoming" ){
    readString="";
    reset_incoming = true;
  }
  
  if(readString == "set_time_incoming"){
    readString="";
    set_time_incoming = true;
  }

}

void alarm_goes_off(){

  analogWrite(EN, 128);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(7000);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  myservo.write(openPosition);
  delay(openTime);
  myservo.write(closePosition);

}