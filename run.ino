#include "Arduino.h"
#include <Keypad.h>
#include <SD.h>
#include <TMRpcm.h>    

//using the MEGA
#define SD_ChipSelectPin 53

TMRpcm sound;   

const byte ROWS = 4;
const byte COLS = 4;

const byte ROW_PINS[ROWS] = { 34, 5, 6, 2 };
const byte COL_PINS[COLS] = { 35, 4, 7, 3 };

// Since the phone has 4 column pins butonly 3 columns, the X's represet non-connections
// Keypad lib expects 4x4 or 3x4
char BUTTONS[ROWS][COLS] = {
  { '1', '2', 'X', '3' },  // 1st row
  { '4', '5', 'X', '6' },  // 2nd row
  { '7', 'X', '8', '9' },  // 3rd row
  { '*', 'X', '0', '#' }   // 4th row
};

Keypad phonePad = Keypad(makeKeymap(BUTTONS), ROW_PINS, COL_PINS, ROWS, COLS);

const unsigned int TONES[ROWS][COLS] = {
  { "tone1.wav", "tone2.wav", "off", "tone3.wav" },
  { "tone4.wav", "tone5.wav", "off", "tone6.wav" },
  { "tone7.wav", "off", "tone8.wav", "tone9.wav" },
  { "tone10.wav", "off", "off", "tone11.wav" }  
};

const byte speaker_pin = 46; 
char* tone_file = "off";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }                                       
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_ChipSelectPin)) { 
    Serial.println("Initialization failed...");
    return;
  }
  else {
      Serial.println("Initialization done...");
  }

  sound.speakerPin = speaker_pin; 
  sound.quality(1); 
  sound.setVolume(5); 
}

void loop() {
  char button_character = phonePad.waitForKey(); 


  for (byte i = 0; i < ROWS; i++) {
    for (byte j = 0; j < COLS; j++) {
      if (button_character == BUTTONS[i][j]) {
        tone_file = TONES[i][j];
        sound.stopPlayback();
        sound.play(tone_file); 
      }
    }
  };


  if (tone_file != "off") {
    Serial.println(tone_file);
    sound.play(tone_file); 
  } else {
    Serial.println("Stop tone");
    noTone(speaker_pin); 
  }
}
