// String incomingString = "";
// void setup() {
//   Serial.begin(9600);
  
// }

// void loop() {
//   if (Serial.available()) {
    
//     while (Serial.available()) {
//       char temp = (char)Serial.read();
//       incomingString += temp;
//     }
//   }
//   Serial.println("Received: " + incomingString);
// }

// void setup() {
//   Serial.begin(9600); // start serial communication with a baud rate of 9600
// }

// void loop() {
//   if (Serial.available() > 0) { // check if there is data available to read
//     String incomingData = Serial.readString(); // read incoming data as a string
//     Serial.println(incomingData); // print the incoming data to the serial monitor
//      int colonIndex = incomingData.indexOf(":");
//     String hourString = incomingData.substring(0, colonIndex);
//     String minuteString = incomingData.substring(colonIndex + 1);
//     int alarmHour = hourString.toInt();
//     int alarmMinute = minuteString.toInt();
//     Serial.println(alarmHour);
//     Serial.println(alarmMinute);
//   }
// }


#include <ESP8266WiFi.h> // Library for WiFi
#include <time.h> // Library for time functions
#include <Servo.h>

#define IN1 D2
#define IN2 D3
#define EN D4
#define touchPin D5
#define buzzer D6
#define led D7

Servo myservo;

const char* ssid = "Gihaa.dev";
const char* password = "12344566";

time_t now;
struct tm timeinfo;

int alarmHour = 99; // Alarm hour (24-hour format)
int alarmMinute = 99; // Alarm minute

int hour, minute, second;
int touchState = 0;

int motor_offset = 350;
int time_offset = 19800;

void setup() {

  myservo.attach(D1); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(touchPin, INPUT_PULLUP);


  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    //Serial.println("Connecting to WiFi...");
  }
  //Serial.println("Connected to WiFi");

  // Connect to NTP server and set the time
configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  while (!getLocalTime(&timeinfo)) {
    //Serial.println("Waiting for NTP time sync...");
    delay(1000);
  }
  //Serial.println(asctime(&timeinfo));
  //Serial.flush();
    Serial.begin(9600);

}

void loop() {

 if (Serial.available() > 0) { // check if there is data available to read
    String incomingString = Serial.readString(); // read incoming data as a string
    Serial.println(incomingString);
    int colonIndex = incomingString.indexOf(":");
    String hourString = incomingString.substring(0, colonIndex);
    String minuteString = incomingString.substring(colonIndex + 1);
    alarmHour = hourString.toInt();
    alarmMinute = minuteString.toInt();
    Serial.println(alarmHour);
    Serial.println(alarmMinute);
  }

  myservo.write(0);
  // Get the current time
  now = time(nullptr);
  localtime_r(&now, &timeinfo);
  //Serial.println(asctime(&timeinfo));

  hour = timeinfo.tm_hour;
  minute = timeinfo.tm_min;
  second = timeinfo.tm_sec;

  add_offset(hour, minute, second, time_offset);

  Serial.print(hour);
  Serial.print(" :: ");
  Serial.print(minute);
  Serial.print(" :: ");
  Serial.print(second);
  Serial.print("\n\n");

  // Check if the current time matches the alarm time
  if (hour == alarmHour && minute == alarmMinute ) {
    Serial.println("alaram on");
    alarm_triggred();
  } else {
    digitalWrite(led, LOW); 
    //Serial.println("alaram Off");
  }

}

void add_offset(int &hour, int &minute, int &second, int offset_in_seconds) {
  second += offset_in_seconds;
  while (second >= 60) {
    second -= 60;
    minute += 1;
  }
  while (minute >= 60) {
    minute -= 60;
    hour += 1;
  }
  while (hour >= 24) {
    hour -= 24;
  }
}

void alarm_triggred(){

  int timer = 0;

      //debug
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(EN, 128);
      delay(motor_offset);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);

  while (timer != 10){

    touchState = digitalRead(touchPin);
    digitalWrite(buzzer, HIGH); 
    digitalWrite(led, HIGH); 

    if (touchState == HIGH) {
      Serial.println("sensor is pressed");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(EN, 128);
      delay(motor_offset);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);

      myservo.write(90);
      delay(4000);
      myservo.write(0);
      digitalWrite(D1, LOW);
      digitalWrite(led, LOW); 
      break;
    }
    else {
      Serial.println("Touch sensor is not pressed");
    }
    delay(1000);
    timer += 1;

  }
   digitalWrite(D1, LOW);
   digitalWrite(led, LOW); 
  //logic
}