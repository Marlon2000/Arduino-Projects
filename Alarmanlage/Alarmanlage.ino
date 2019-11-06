/*
   Alarmanlage

  
   In the normal state, the yellow LED lights up to indicate that the alarm system is active.
   If someone runs into the alarm system or plays with fire in front of the infrared sensor,
   the alarm goes off and the red LED flashes quickly!
   In order to deactivate the alarm system, the white button must be pressed so that the green LED lights up and a melody sounds.
*/
// add note library
#include "pitches.h"

// Global Variables:

// IR-Sensor GND + Signalf
int GND = A0;
int Signal = A1;

// Create the button
int SchalterZustand;
int InputPin = 13;

// Creation of the LEDs
int LED1 = 7;
int LED2 = 6;
int LED3 = 5;

// Musical notes in the melody
int Melodie[] = {NOTE_C1, NOTE_G1, NOTE_C3, NOTE_D7, NOTE_C1, NOTE_G1, NOTE_C3, NOTE_C3};

// Lenght of the notes. 4 = quarter / 8 = eighth note
int noteDauern[] = {8, 8, 8, 8, 4, 4, 4, 4};

void setup() {
  // Put your setup code here, to run once:

  // GND pin of the IR-Sensors
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
  // Receive signal from IR sensor
  pinMode(Signal, INPUT);

  // LEDs are supplied with power
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // Establishing a serial connection with the pc
  Serial.begin(9600);

  // Declare switch as input
  pinMode(InputPin, INPUT);
}

void loop() {
  // Put your main code here, to run repeatedly:

  SchalterZustand == 0;

  // Initial status of the Alarm system
  
  digitalWrite(LED1, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED2, HIGH);
  delay(1100);
  digitalWrite(LED2, LOW);
  delay(900);

  // Reading out the status of the sensor
  Serial.println((Signal));
  // If a object is near the sensor
  if (analogRead(Signal) < 500)
  {
    // Reading out the status of the switch
    SchalterZustand = digitalRead(InputPin);
    Serial.println(SchalterZustand, DEC);

    while (SchalterZustand == 0)
    {
      // Iteration over the notes in the melody
      for(int aktuelleNote = 0; aktuelleNote < 48; aktuelleNote++)
      {
        digitalWrite(LED3, HIGH);
        // To calculate the duration of a note, one second is calculated by the note type
        int noteDauer = 1000 / noteDauern [aktuelleNote];
        // Buzzer on Pin8
        tone(8, Melodie [aktuelleNote], noteDauer);
        
        int pauseZwischenNoten = noteDauer * 1.40;
        delay(pauseZwischenNoten);
        digitalWrite(LED3, LOW); 
        noTone(8);
        break;
      }
      SchalterZustand = digitalRead(InputPin);
    }

    if (SchalterZustand == 1)
    {
      // If the switch is pressed, switch off the red LED + switch on the green LED/ buzzer.
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED1, HIGH);
      delay(1080);
      digitalWrite(LED1, LOW);
      delay(900);
    }
  }
}
