#include <avr/sleep.h>
#ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//Attiny Core:
//https://github.com/SpenceKonde/ATTinyCore
//Board: Attiny 24/44/84
//Pin Mapping: Counter-Clockwise
//Chip: Attiny 84
//Clock: 8 MHz (internal)

// ATMEL ATTINY84 / ARDUINO
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//             (D  0)  PB0  2|    |13  AREF (D 10)
//             (D  1)  PB1  3|    |12  PA1  (D  9) 
//                     PB3  4|    |11  PA2  (D  8) 
//  PWM  INT0  (D  2)  PB2  5|    |10  PA3  (D  7) 
//  PWM        (D  3)  PA7  6|    |9   PA4  (D  6) 
//  PWM        (D  4)  PA6  7|    |8   PA5  (D  5)        PWM

const int charHeight = 8;
const int charWidth = 5;
int rows= 8;                     // Total LED's in a row
bool STATES[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
int LEDS[] = {10, 9, 8, 7, 6, 5, 4, 3};  // Port A
byte magPin = 2;                 // Hall effect sensor, pulled-up, goes low when magnet passes      
byte buttonPin = 0;        
int length=(sizeof(LEDS)/sizeof(int));
int mode;                                // current operating mode, stored in EEPROM
int modes = 7;             
void setup() {
  // put your setup code here, to run once:
  // setup inputs
  pinMode(buttonPin, INPUT_PULLUP);
  //digitalWrite(buttonPin, HIGH);
  pinMode(magPin, INPUT_PULLUP);
  //digitalWrite(magPin, HIGH);
  
  // setup other LEDs
  for(int LED=0; LED<length; LED++){
    pinMode(LEDS[LED], OUTPUT);
    digitalWrite(LEDS[LED], STATES[LED]);
  }

}

void loop() {
  checkButton();
  
  if(mode==0){
    for(int LED=0; LED<length; LED++){
    if(LED%2!=0){
       digitalWrite(LEDS[LED], HIGH);
       delay(1);
    }
    else
      digitalWrite(LEDS[LED], LOW);
      delay(1);
 }
 
  delay(100);}
  
  else if(mode==1){
  // put your main code here, to run repeatedly:
    for(int LED=0; LED<length; LED++){
       digitalWrite(LEDS[LED], HIGH); 
       delay(1); 
       digitalWrite(LEDS[LED], LOW); 
    } 
    for(int LED=length-1; LED >= 0; LED--){
       digitalWrite(LEDS[LED], HIGH); 
       delay(1); 
       digitalWrite(LEDS[LED], LOW); 
    }  
  }
  else if(mode==2){
    for(int LED=0; LED<length; LED++){
       digitalWrite(LEDS[LED], HIGH); 
       delay(1); 
    } 
    for(int LED=length-1; LED >= 0; LED--){
       delay(1); 
       digitalWrite(LEDS[LED], LOW); 
    }
  }
  else if(mode==3){
    for(int LED=0; LED<length; LED++){
       digitalWrite(LEDS[LED], HIGH); 
       delay(3); 

    } 
    for(int LED=0; LED<length; LED++){
       digitalWrite(LEDS[LED], LOW); 
       delay(3); 

    }   
  }
  else if(mode==4){
    for(int LED=0; LED<length; LED++){
      if(LED<4){
        digitalWrite(LEDS[LED], HIGH); 
        delay(1);
  }
      else{    
        digitalWrite(LEDS[LED], LOW); 
        delay(1);
      }
  }
  delay(10); 
    for(int LED=0; LED<length; LED++){
      if(LED<4){
        digitalWrite(LEDS[LED], LOW); 
        delay(1);
      }
      else{   
        digitalWrite(LEDS[LED], HIGH); 
        delay(1);
      }
   }
  }
  else if(mode==5){
    for(int LED=0; LED<length/2; LED++){
       digitalWrite(LEDS[LED], LOW); 
       delay(1); 
       digitalWrite(LEDS[LED], HIGH); 
    } 
    for(int LED=length-1; LED >=length/2; LED--){
       digitalWrite(LEDS[LED], HIGH); 
       delay(1); 
       digitalWrite(LEDS[LED], LOW); 
    }
  }
  else if(mode==6){
    for(int LED=0; LED<length; LED++){
      if(LED%2==0){
        digitalWrite(LEDS[LED], HIGH); 
        delay(1);
    }
      else{
      if(LED==7){
        digitalWrite(LEDS[LED], HIGH); 
      }
      else  
        digitalWrite(LEDS[LED], LOW); 
        delay(1);
    }
   }
   delay(1); 
    for(int LED=0; LED<length; LED++){
      if(LED%2==0){
        digitalWrite(LEDS[LED], LOW); 
        delay(1);
    }
      else{   
       digitalWrite(LEDS[LED], HIGH); 
       delay(1);
      }
    }
}
}

void system_sleep() {
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}

void checkButton(){
  // check button for mode change and display current mode
  if (touched()){
    mode += 1;
    if (mode >= modes){
      mode = 0;
    }
  }
}

bool touched(){
  // returns true if touched, false if not.  Light LED until touch released
  bool touchVal = digitalRead(buttonPin);
  if (!touchVal){
    while(!digitalRead(buttonPin)){ // wait till touch release
      delay(10);
      digitalWrite(LEDS[mode], LOW);
    }
    //digitalWrite(LEDS[0], LOW);
    return (true);
  }
  else{
    return (false);
  }
}
