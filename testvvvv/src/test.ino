#include <Arduino.h>
#include <TM1637Display.h>
#include <NewPing.h>
//#include <TimerFreeTone.h> //Nejdříve jsem problém, který měla knihovna NewPing s knihovnou funkce Tone chtěl vyřešil pomocí této knihovny
#include <NewTone.h> // Nakonec se však ukázalo že mi použití knihovny NewTone vyhovuje více

#define TRIGGER_PIN  10  // Trig pin na ultrazvukovém senzoru
#define ECHO_PIN     9  // Echo pin na ultrazvukovém senzoru
#define MAX_DISTANCE 200 // Maximální vzdálenost, na kterou chceme testovat ping. Maximální vzdálenost senzoru je mezi 400-500cm.
#define BUZPIN  2
#define CLK 3 // CLK pin na displeji
#define DIO 4 // DIO pio na diespleji

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing nastavení pinů a maximální vzdálenosti.

TM1637Display display(CLK, DIO);
uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };

unsigned long previousMillis = 0;        // will store last time LED was updated  
const long interval = 50;           // interval at which to blink (milliseconds)
int old_distance=0;

void setup() {
  display.setBrightness(0x0f);
  
  display.setSegments(data);
  Serial.begin(9600); 

  pinMode(BUZPIN, OUTPUT); // bzučák
  
  pinMode(5, OUTPUT); // červená LED
  pinMode(6, OUTPUT); // žlutá LED
  pinMode(7, OUTPUT); // zelená LED

}

void loop() {
  
    unsigned long currentMillis = millis();
    int distance;

    if (currentMillis - previousMillis >= interval) {
    
    distance = sonar.ping_cm();  // získá vzdálenost a uloží ji do proměnné distance
    if (abs(old_distance - distance) > 2 ){
        old_distance = distance;
    }
    }

  Serial.print("Ping: ");
  Serial.print(old_distance); // Odešle ping a dostane vzdálenost v cm a tu vypíše do sériového monitoru (0 = znamená mimo určenou vzdálenost)
  Serial.println("cm");

  display.showNumberDec(old_distance, false, 3,1);

  if (distance <= 150 && distance >= 100) {
    digitalWrite(7, HIGH);  // zapínání/vypínání LED
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    NewTone(BUZPIN, 200, 100); 
    delay(10*(distance-10)); //nastavení bzučáku jak často bude pípat
    noNewTone(BUZPIN); 

  }
  else if (distance <= 100 && distance >= 51) {
    digitalWrite(7, LOW);  
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    NewTone(BUZPIN, 200, 100); 
    delay(10*(distance-10)); //nastavení bzučáku jak často bude pípat
    noNewTone(BUZPIN); 

  }
  else if (distance <= 50 && distance >= 11) {
    digitalWrite(7, LOW); 
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH); 
    NewTone(BUZPIN, 200, 100); 
    delay(10*(distance-10)); //nastavení bzučáku jak často bude pípat
    noNewTone(BUZPIN); 
  }
  else if (distance <= 10 && distance >= 0) {
    digitalWrite(7, LOW); 
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
    NewTone(BUZPIN, 200, 100);   
  }
  else{
      digitalWrite(7, LOW); 
      digitalWrite(6, LOW);
      digitalWrite(5, LOW);  
  }
}