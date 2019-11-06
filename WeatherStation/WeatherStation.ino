/*
 * Weather & Media Station
 * 
 * Displays the current state of the temperature and humidity in a room. With the buttons underneath the display
 * you can chose between this two states by pressing the "Up" or "Down" Button. Addtionally you can play the melody
 * of Jingle Bells by pressing the "Left" Button. The Red LED and the Green LED will tune up with the Jingle Bells
 * melody, switching each others side beat by beat.
 * 
 */
#include <DHT.h> // Include the humidity sensor library (Needs to be installed by searching DHT)
#include <LiquidCrystal.h> // Library used for the lcd screen
#include "pitches.h" // Library with all the notes


#define DHTPIN 2 // Data pin of the sensor
#define DHTTYPE DHT11 // Sensor-type used within the library

DHT dht(DHTPIN, DHTTYPE); // Makes that the arduino board identifies the sensor

//LCD pins used for the LCD-screen
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10;
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

//LEDs used
const int LED1 = A1;
const int LED2 = A2;

//Notes in the melody & used pins
int speakerpin = 3;
int GND = A3;
int JingleBells_melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

//note durations. 4 = quarter note / 8 = eigth note
int noteDurations[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

// Number of elements in the current Array
const int num_elementsArray = sizeof(noteDurations)/sizeof(noteDurations[0]);

void setup() {
  lcd.begin(16, 2); // Defines the number of Strings that can be used in the display
  lcd.setCursor(0,0); // On top of the screen the text will be printed
  lcd.print("Reading sensor");
  lcd.setCursor(0,1); // On the bottom of the screen the text will be printed
  lcd.print("Press: UP key");
  Serial.begin(9600); // Start a connection with a computer to view data in the console
  dht.begin();  // arduino asks dht sensor to start collecting data

  // Leds are getting powered
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Speaker is getting powered
  pinMode(speakerpin, OUTPUT);
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
}

void loop() {
  delay(15);
  int x;
  x = analogRead (0); // To read the buttons as an Input

  float humidity, temperature;
  
  humidity = dht.readHumidity(); // Starts reading the humidity value and pass it to the variable
  temperature = dht.readTemperature(); // Starts reading the temperature value and pass it to the variable
  delay(60);

  Serial.println("temperature is ="); 
  Serial.println(temperature); // To view the temperature value in the console
  Serial.println("humidity is =");
  Serial.println(humidity); // To view the humidity value in the console

  // arrays of characters
  char tempString[6]; 
  char humString[6]; 
  dtostrf(temperature, 5, 1, tempString); // converts the float value into a String
  dtostrf(humidity, 2, 0, humString); // converts the float value into a String

  // value for the up button
  if(x < 200) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature is =");
    lcd.setCursor(0,1);
    lcd.print(tempString);  // To view the temperature value on the display
    lcd.print((char)223); // value to create the degree sign
    lcd.print("C");

    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
  }

  // value for the down button
  else if(x < 400){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humidity is ="); 
    lcd.setCursor(0,1);
    lcd.print(humString); // To view the humidity value on the display
    lcd.print("%");

    digitalWrite(LED2, HIGH);
    digitalWrite(LED1, LOW);
  }

  // value for the left button
  else if(x < 600) {
    // Iterate over the notes in the melody
    for(int thisNote = 0; thisNote < num_elementsArray; thisNote++) {
      //to calculate the note duration, take one second divided by the note type
      int noteDuration = 1000/noteDurations[thisNote];
      tone(speakerpin, JingleBells_melody[thisNote], noteDuration);

      // time between the single notes
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing
      noTone(speakerpin);

      if(thisNote % 8 == 0) {
        NoteDescription1();
      }
      lightShow1();
    }
  }

  // value for the select button
  else if(x < 800) {
    // Info about Buttons
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Weather & Tune");
    lcd.setCursor(0,1);
    lcd.print("BTN: UP, DN, LFT");
  }
  
  // If it's not the right value or no data sent
  else if(isnan(humidity) || isnan(temperature)){
    Serial.print("Failed to read");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Fail:");
    lcd.setCursor(0,1);
    lcd.print("unable to read");
  }
}

void NoteDescription1() {
  // Prints the actual song playing
  Serial.print("Now playing...");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Now playing...");
  lcd.setCursor(0,1);
  lcd.print("Jingle Bells");
}

void lightShow1() {
    // LEDs blinking according to the melody
    digitalWrite(LED1, HIGH); 
    digitalWrite(LED2, LOW);
    delay(100);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
}
