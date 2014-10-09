/*
HELLO TRAVELLER 
An Arduino artefact created for DIGF6L01.

An ultrasonic sensor triggers LCD messages. A button triggers a servo motor.
A traveller receives a fright.

8 October 2014 
by Sachiko Murakami

*/


// include libraries
#include <Button.h>
#include <LiquidCrystal.h>
#include <Servo.h>


// announcing LCD and servo exist
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
Servo myservo;

// setting pins
#define trigPin 5
#define echoPin 4
#define buttonPin 2
#define ledPin 13

// defining variables
int pos  = 15;        // sets initial position for servo       
int runxtime = 0;    // to control what LCD displays depending on stage of process
int reminder = 0;    // to determine whether second message has been displayed
unsigned long time;       // time counter
unsigned long timestamp;  // to count after the second message is displayed to make sure it doesn't stay inactive for too long.
Button button = Button(2, PULLDOWN); // using button library

void setup() {
  lcd.begin(16, 1);            // sets up LCD screen
  lcd.print("Initializing.");  // lets me know what's happening
  Serial.begin(9600);          // begin serial monitor
  pinMode(trigPin, OUTPUT);    // sets trigpin of ultrasonic as output
  pinMode(echoPin, INPUT);     // sets echopin of ultrasonic as input

  myservo.attach(6);           // attaches servo to pin 6 
  myservo.write(15);           // set servo initial position
}


void(* resetFunc) (void) = 0; // reset function if button isn't pushed in time

void moveServo() {            // servo movement
  int y = 0; 
  for (y = 0; y < 1; y++) 
  {
    for(pos = 15; pos <= 90; pos += 1)  // move foreward fast
    {                                 
      myservo.write(pos);              
      delay(5);                     
    }
    delay(2000); 
    for(pos = 90; pos>=15; pos-=1)      // move back a little slower
    {                                
      myservo.write(pos);             
      delay(15);
    }
  }
}

void flashlcd() {                       //  display for 1st narrative
  int i = 0;  
  for (i = 0; i < 2; i++) {
    lcd.noDisplay();                     
    delay(200);
    lcd.display();
    delay(200);
    i++;
  }
  delay(1000); 
  lcd.clear();  
}

void flashlcdfast() {                   // blink the display fast for second narrative
  int i = 0;  
  for (i = 0; i < 6; i++) {
    lcd.noDisplay();                      
    delay(100);
    lcd.display();
    delay(100);
    i++;
  }
  delay(100); 
  lcd.clear();  
}

void buttonAction() {
  Serial.println("buttonpushed");
  lcd.clear(); 
  lcd.print("HA HA HA HA HA HA HA HA HA HA");
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) 
  {
    lcd.scrollDisplayLeft(); 
    delay(150);
  }
  moveServo();
} 

void helloTraveller() {                 // First narrative
  lcd.clear();                          // clear lcd screen
  lcd.print("HELLO TRAVELLER");         // write message to screen
  flashlcd();
  lcd.print("WELCOME.");
  flashlcd();
  lcd.print("YOU'RE SAFE NOW.");
  flashlcd();
  lcd.print("ENJOY YOURSELF.");   
  flashlcd();
  lcd.print("BUT");    
  flashlcd();
  lcd.print("WHATEVER YOU DO");
  flashlcd();
  lcd.print("DON'T");
  flashlcdfast();
  lcd.print("PUSH");
  flashlcdfast();
  lcd.print("THE");
  flashlcdfast();
  lcd.print("BUTTON");
  flashlcdfast();
  delay(1000);
  lcd.clear();
}

void pushTheButton() {                      // second narrative
  lcd.clear();
  lcd.print("     I SAID PUSH THE BUTTON");
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) 
  {
    lcd.scrollDisplayLeft(); 
    delay(175); 
  }
  lcd.clear();
  delay(1000);
}

long duration, distance;            // variables to convert time to distance for ultrasonic sensor

void runUltrasonic() {              // turning on the ultrasonic sensor
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  //Serial.println(distance);
}

void disableUltrasonic() {          // disable ultrasonic sensor after 2nd narrative to make button pushable
  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
} 

void loop(){

  if (button.isPressed()) {         // button trigger
    buttonAction();
    reminder = 0;
    runxtime = 0;
    delay(1000);

  }

  if (reminder == 0) {              // keep ultrasonic going
    runUltrasonic();
  }
  else {
    disableUltrasonic();            // disable ultrasonic so button is pushable
    lcd.clear();
    lcd.print("Systems normal.");
  }


  if (distance < 30 && distance > 0 && runxtime==0 && reminder==0) // if motion
  {  
    helloTraveller();              // run first narrative
    runxtime = 1;                  // acts as a switch so second narrative runs
  } 
  else if (distance < 30 && distance > 0 && runxtime==1 && reminder==0)// if motion, AND the first narrative has occurred
  { 
    pushTheButton();              // run second narrative
    reminder = 1;                // acts as a switch so ultrasonic disabled and button pushable
    timestamp = millis()/1000;
  }
  else {
    lcd.clear();                  // default display
    lcd.print("Systems normal."); // 

  }
  time = millis()/1000;
  Serial.println(time);                          // time counter for my reference
  if (reminder == 1 && (time - timestamp) > 15) { // resents program if there is too long of a delay.
    resetFunc();
  }

}



