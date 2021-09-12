#include <Otto9.h>
Otto9 Otto;
#include <SerialCommand.h>//-- Library to manage serial commands
SoftwareSerial BTserial = SoftwareSerial(11,12); //  TX  RX of the Bluetooth
SerialCommand SCmd(BTserial);  //The SerialCommand object

#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

Servo rightServo;
Servo leftServo;
Servo gripServo;  

int gripPos;
int rightSpeed = 0;
int leftSpeed = 0;

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl2(int rightSpeed, int leftSpeed, int stepDelay) {   // -90 - full speed backwards, 90 - full speed forwards, 0 - stopped

  //Set all of the servo positions
  rightServo.write(90 + rightSpeed);  //compensate for the 0-180, -90-90 relationship
  leftServo.write(90 - leftSpeed);
  delay(stepDelay*1000);
}
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
int T=1000;              //Initial duration of movement
int moveId=0;            //Number of movement
int moveSize=15;         //Asociated with the height of some movements
unsigned long int matrix;
#define DIN_PIN A3
#define CS_PIN A2
#define CLK_PIN A1
#define LED_DIRECTION 1
#define PIN_YL 2 // left leg, servo[0]
#define PIN_YR 3 // right leg, servo[1]
#define PIN_RL 4 // left foot, servo[2]
#define PIN_RR 5 // right foot, servo[3]
#define PIN_Trigger 8 // ultrasound
#define PIN_Echo 9 // ultrasound
#define PIN_Buzzer  13 //buzzer

void setup(){
  Serial.begin(9600);  
  BTserial.begin(9600);   
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  //Setup callbacks for SerialCommand commands 
  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  SCmd.addCommand("M", receiveMovement);  //  sendAck & sendFinalAck
  SCmd.addCommand("H", receiveGesture);   //  sendAck & sendFinalAck
  SCmd.addCommand("K", receiveSing);      //  sendAck & sendFinalAck
  SCmd.addDefaultHandler(receiveStop);
  //Otto wake up!
  Otto.sing(S_connection);
  Otto.home();
  Otto.sing(S_happy);
  delay(200);
    //Set the Intial Positions of Everything
  rightServo.write(90);
  leftServo.write(90);
  gripServo.write(50);   
  delay(1000);
  //Initialize the positions of everything
  gripServo.attach(4);
  rightServo.attach(3);
  leftServo.attach(2);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
        SCmd.readSerial();    //If Otto is moving yet
        if (Otto.getRestState()==false){  
          move(moveId);
        }  
    }
///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////
//-- Function to receive Stop command.
void receiveStop(){
    sendAck();
    Otto.home();
    sendFinalAck();
}

void receiveMovement(){

    sendAck();

    if (Otto.getRestState()==true){
        Otto.setRestState(false);
    }
    //Definition of Movement Bluetooth commands
    //M  MoveID  T   MoveSize  
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {moveId=atoi(arg);}
    else{
      Otto.putMouth(xMouth);
      delay(2000);
      Otto.clearMouth();
      moveId=0; //stop
    }
    
    arg = SCmd.next(); 
    if (arg != NULL) {T=atoi(arg);}
    else{
      T=1000;
    }

    arg = SCmd.next(); 
    if (arg != NULL) {moveSize=atoi(arg);}
    else{
      moveSize =15;
    }
}

//-- Function to execute the right movement according the movement command received.
void move(int moveId){

  bool manualMode = false;

  switch (moveId) {
    case 0:
      motorControl2 (0, 0, 0 ); // stop
      break;
    case 1: //M 1 1000 
      motorControl2 (-45, -45, 0.2 ); // front 
      break;
    case 2: //M 2 1000 
      motorControl2 (45, 45, 0.2 ); // back 
      break;
    case 3: //M 3 1000 
      motorControl2 (-45, 0, 0.4 ); // turn left 
      break;
    case 4: //M 4 1000 
      motorControl2 (0, -45, 0.4 ); // turn right 
      break;
    case 5: //M 5 1000 30 
      gripServo.write(90);
      break;
    case 6: //M 6 1000 30
      gripServo.write(70);
      break;
    case 7: //M 7 1000 30
      gripServo.write(60);
      break;
    case 8: //M 8 1000 30
      gripServo.write(40);
      break;
    case 9: //M 9 1000 30 
      gripServo.write(30);
      break;
    case 10: //M 10 1000 30 
      gripServo.write(110);
      break;
    case 11: //M 11 1000 
      gripServo.write(130);
      break;
    case 12: //M 12 1000 30 
      gripServo.write(150);
      break;
    case 13: //M 13 1000 30
      gripServo.write(155);
      break;
    case 14: //M 14 1000 20
      gripServo.write(120);
      break;
    case 15: //M 15 500 
      gripServo.write(80);
      break;
    case 16: //M 16 500 
      gripServo.write(60);
      break;
    case 17: //M 17 500 
      gripServo.write(140);
      break;
    case 18: //M 18 500 
      gripServo.write(40);
      break;
    case 19: //M 19 500 20
      gripServo.write(145);
      break;
    case 20: //M 20 500 15
     gripServo.write(85);
      break;
    default:
        manualMode = true;
      break;
  }

  if(!manualMode){
    sendFinalAck();
  }    
}

//-- Function to receive gesture commands
void receiveGesture(){

    //sendAck & stop if necessary
    sendAck();
    Otto.home(); 

    //Definition of Gesture Bluetooth commands
    //H  GestureID  
    int gesture = 0;
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {gesture=atoi(arg);}
    else 
    {
      //Otto.putMouth(xMouth);
      delay(2000);
      //Otto.clearMouth();
    }

    switch (gesture) {
      case 1: //H 1 
        Otto.playGesture(OttoHappy);
        break;
      case 2: //H 2 
        Otto.playGesture(OttoSuperHappy);
        break;
      case 3: //H 3 
        Otto.playGesture(OttoSad);
        break;
      case 4: //H 4 
        Otto.playGesture(OttoSleeping);
        break;
      case 5: //H 5  
        Otto.playGesture(OttoFart);
        break;
      case 6: //H 6 
        Otto.playGesture(OttoConfused);
        break;
      case 7: //H 7 
        Otto.playGesture(OttoLove);
        break;
      case 8: //H 8 
        Otto.playGesture(OttoAngry);
        break;
      case 9: //H 9  
        Otto.playGesture(OttoFretful);
        break;
      case 10: //H 10
        Otto.playGesture(OttoMagic);
        break;  
      case 11: //H 11
        Otto.playGesture(OttoWave);
        break;   
      case 12: //H 12
        Otto.playGesture(OttoVictory);
        break; 
      case 13: //H 13
        Otto.playGesture(OttoFail);
        break;         
      default:
        break;
    }
    sendFinalAck();
}

//-- Function to receive sing commands
void receiveSing(){

    //sendAck & stop if necessary
    sendAck();
    Otto.home(); 

    //Definition of Sing Bluetooth commands
    //K  SingID    
    int sing = 0;
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {sing=atoi(arg);}
    else 
    {
     // Otto.putMouth(xMouth);
      delay(2000);
      //Otto.clearMouth();
    }

    switch (sing) {
      case 1: //K 1 
        Otto.sing(S_connection);
        break;
      case 2: //K 2 
        Otto.sing(S_disconnection);
        break;
      case 3: //K 3 
        Otto.sing(S_surprise);
        break;
      case 4: //K 4 
        Otto.sing(S_OhOoh);
        break;
      case 5: //K 5  
        Otto.sing(S_OhOoh2);
        break;
      case 6: //K 6 
        Otto.sing(S_cuddly);
        break;
      case 7: //K 7 
        Otto.sing(S_sleeping);
        break;
      case 8: //K 8 
        Otto.sing(S_happy);
        break;
      case 9: //K 9  
        Otto.sing(S_superHappy);
        break;
      case 10: //K 10
        Otto.sing(S_happy_short);
        break;  
      case 11: //K 11
        Otto.sing(S_sad);
        break;   
      case 12: //K 12
        Otto.sing(S_confused);
        break; 
      case 13: //K 13
        Otto.sing(S_fart1);
        break;
      case 14: //K 14
        Otto.sing(S_fart2);
        break;
      case 15: //K 15
        Otto.sing(S_fart3);
        break;    
      case 16: //K 16
        Otto.sing(S_mode1);
        break; 
      case 17: //K 17
        Otto.sing(S_mode2);
        break; 
      case 18: //K 18
        Otto.sing(S_mode3);
        break;   
      case 19: //K 19
        Otto.sing(S_buttonPushed);
        break;                      
      default:
        break;
    }
    sendFinalAck();
}

//-- Function to send Ack comand (A)
void sendAck(){
  delay(30);
  Serial.print(F("&&"));
  Serial.print(F("A"));
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send final Ack comand (F)
void sendFinalAck(){
  delay(30);
  Serial.print(F("&&"));
  Serial.print(F("F"));
  Serial.println(F("%%"));
  Serial.flush();
}
