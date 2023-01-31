#include <ESP8266WiFi.h> // Library for WiFi
#include <time.h> // Library for time functions

const char* ssid = "Gihaa.dev";
const char* password = "12344566";

time_t now;
struct tm timeinfo;

int alarmHour = 18; // Alarm hour (24-hour format)
int alarmMinute = 15; // Alarm minute
int alarmSecond = 30; // Alarm second

int alarmPin = D1; // Pin connected to the alarm buzzer
int buttonPin = D3; // Pin connected to the button for setting the alarm
int led = 2;

int hour, minute, second;
int offset_in_seconds;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT); 


  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to NTP server and set the time
configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Waiting for NTP time sync...");
    delay(1000);
  }
  Serial.println(asctime(&timeinfo));


  // Set the alarm pin as an output
  pinMode(alarmPin, OUTPUT);

  // Set the button pin as an input
  pinMode(buttonPin, INPUT);
}

void loop() {
  // Get the current time
  now = time(nullptr);
  localtime_r(&now, &timeinfo);
  Serial.println(asctime(&timeinfo));

  hour = timeinfo.tm_hour;
  minute = timeinfo.tm_min;
  second = timeinfo.tm_sec;

  add_offset(hour, minute, second, 19800);

  Serial.print(hour);
  Serial.print(" :: ");
  Serial.print(minute);
  Serial.print(" :: ");
  Serial.print(second);
  Serial.print("\n\n");

  // Check if the current time matches the alarm time
  if (hour == alarmHour && minute == alarmMinute && second == alarmSecond) {
    digitalWrite(alarmPin, HIGH); // Turn on the alarm buzzer
    digitalWrite(led, HIGH); 
    Serial.println("alaram on");
    alarm_triggred();
  } else {
    digitalWrite(alarmPin, LOW); // Turn off the alarm buzzer
    digitalWrite(led, LOW); 
    delay(1000);
    Serial.println("alaram Off");
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

  //logic
}