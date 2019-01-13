
////////////////////////////
#include <Arduino.h>
#include <TM1637Display.h>
#include <NewPing.h>
#include <TimerFreeTone.h>
#define TRIGGER_PIN  10  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     9  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define BUZPIN  2

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int duration[] = { 250, 125, 125, 250, 250, 250, 250, 250 };

// Module connection pins (Digital Pins)
#define CLK 3
#define DIO 4

// The amount of time (in milliseconds) between tests
#define TEST_DELAY  500
TM1637Display display(CLK, DIO);
uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
/////////////////////////
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 50;           // interval at which to blink (milliseconds)


void setup() {
  display.setBrightness(0x0f);
  
  display.setSegments(data);
  Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.

  pinMode(BUZPIN, OUTPUT); // buzz pin is output to control buzzering
  
  pinMode(5, OUTPUT); // červená LED
  pinMode(6, OUTPUT); // žlutá LED
  pinMode(7, OUTPUT); // zelená LED
}

int old_distanc=0;
void loop() {
  unsigned long currentMillis = millis();
  int distance;
  
  if (currentMillis - previousMillis >= interval) {
   
   distance = sonar.ping_cm();// get the distance and store it in "distance" variable
   if (abs(old_distanc - distance) > 2 ){
      old_distanc = distance;
   }
  }
                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");


  Serial.print(old_distanc) ; // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

  display.showNumberDec(old_distanc, false, 3,1);


    TimerFreeTone(BUZPIN, distance*2+300, distance*2+150); 
    delay(50); 


if (distance <= 150 && distance >= 100) {
  digitalWrite(7, HIGH);  // zapínání/vypínání LED
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);


}
else if (distance <= 100 && distance >= 50) {
  digitalWrite(7, HIGH);  
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
}
else if (distance <= 50 && distance >= 0) {
  digitalWrite(7, HIGH); 
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);  
}
else{
    digitalWrite(7, LOW); 
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);  
}


  
}