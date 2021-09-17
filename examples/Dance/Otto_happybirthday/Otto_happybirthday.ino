//----------------------------------------------------------------
//-- Otto Happy Birthday
//-- With this code Otto will walk 5 steps and then sing the happy birthday
//-- April 2019: Designed to work with the basic Otto but could be compatible with PLUS or Humanoid or other biped robots
/******************************************************************************************************
  Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
  Otto DIY invests time and resources providing open source code and hardware, 
  please support by purchasing kits from (https://www.ottodiy.com)
  
  BSD license, all text above must be included in any redistribution
 *******************************************************************/
//-----------------------------------------------------------------
#include <Servo.h> 
#include <Oscillator.h>
#include <US.h>
#include <Otto9.h>
Otto9 Otto;  //This is Otto!
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)
int speakerPin = 13;
int length = 28; // the number of notes
char notes[] = "GGAGcB GGAGdc GGxecBA yyecdc";
int beats[] = { 2, 2, 8, 8, 8, 16, 1, 2, 2, 8, 8,8, 16, 1, 2,2,8,8,8,8,16, 1,2,2,8,8,8,16 };
int tempo = 150;

void playTone(int tone, int duration) {

for (long i = 0; i < duration * 1000L; i += tone * 2) {
   digitalWrite(speakerPin, HIGH);
   delayMicroseconds(tone);
   digitalWrite(speakerPin, LOW);
   delayMicroseconds(tone);
}
}
void playNote(char note, int duration) {

char names[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B',           

                 'c', 'd', 'e', 'f', 'g', 'a', 'b',

                 'x', 'y' };

int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014,

                 956,  834,  765,  593,  468,  346,  224,

                 655 , 715 };

int SPEE = 5; // play the tone corresponding to the note name

for (int i = 0; i < 17; i++) {
   if (names[i] == note) {
    int newduration = duration/SPEE;
     playTone(tones[i], newduration);
   }
}
}

void setup() {
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
pinMode(speakerPin, OUTPUT);
}

void loop() {
  Otto.walk(5,1300,1); 
  Otto.playGesture(OttoSuperHappy);
for (int i = 0; i < length; i++) {
   if (notes[i] == ' ') {
     delay(beats[i] * tempo); // rest
   } else {
     playNote(notes[i], beats[i] * tempo);
   }
   // pause between notes
   delay(tempo);   
}
}
