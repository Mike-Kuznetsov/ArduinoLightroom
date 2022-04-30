/*  Lightroom (Arduino Smart Lamp)
 *  Programm is written by Mautoz mikemautos@gmail.com
 *  Tested on Arduino Uno and Arduino Nano
 *  
 *  Pins:  
 *    IR-receiver - digital 8
 *    Wi-Fi module PWM signal - digital 2
 *    Relay 1/2/3 - digital 3/4/5
 *    
 *  Mautoz Tech YouTube Channel: https://www.youtube.com/c/mautoztech
*/
 
volatile int length = 0;
volatile int start_time = 0;
short int last=0;
#include "IRremote.h"
IRrecv irrecv(8); // IR-receiver pin
decode_results results;
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  attachInterrupt(0, rising, RISING); // This interrupt will put PWM value into lenght variable
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
}

void loop() { 
  Serial.println(length);
  if (length<30){ // Off mode
    if (last!=0){
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      last=0;
      Serial.println("0");
    }  
  }
  else if (length<333){ // 1sf mode, , it changes the states of the relays only if you changed state in your phone
    if (last!=1){
      digitalWrite(3,HIGH);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      last=1;
      Serial.println("1");
    }  
  }
  else if (length<667){ // 2nd mode, it changes the states of the relays only if you changed state in your phone
    if (last!=2){
      digitalWrite(3,HIGH);
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
      last=2;
      Serial.println("2");
    }  
  }
  else if (last!=3){ // 3rd mode, , it changes the states of the relays only if you changed state in your phone
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    last=3;
    Serial.println("3");
  }
  if ( irrecv.decode( &results )) { // If programm gets data from IR it changes the states of relays
    Serial.println(results.value);
    if (results.value==16738455){
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        Serial.println("0");
  
    }
    else if (results.value==16724175){
        digitalWrite(3,HIGH);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        Serial.println("1");

    }
    else if (results.value==16718055){
        digitalWrite(3,HIGH);
        digitalWrite(4,HIGH);
        digitalWrite(5,LOW);
        Serial.println("2");
    }
    else if (results.value==16743045){
      digitalWrite(3,HIGH);
      digitalWrite(4,HIGH);
      digitalWrite(5,HIGH);
      Serial.println("3");
    }
    irrecv.resume();
  }
  delay(100);
}

void rising() {
  attachInterrupt(0, falling, FALLING);
  start_time = micros();
}

void falling() {
  attachInterrupt(0, rising, RISING);
  length = micros() - start_time;
}
