// wiring diagram 
/*

         \/ the arduino \/
        __________________
       [------------------]13 
       [------------------]12 distance sensor TRIG
    RST[------------------]11 distance sensor ECHO
    3.3[------------------]10
     5V[------------------]9 buzzer (1)
    GND[------------------]8
    GND[------------------]7
    Vin[------------------]6
     A0[------------------]5
     A1[------------------]4
     A2[------------------]3
     A3[------------------]2
     A4[------------------]1
     A5[------------------]0



*/

// imports
#include <NewPing.h>

// tone library hack
#include <TonePlayer.h>

// ports
#define BUZZER_PWM_PIN 9
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 48 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define LED 11
#define BUTTON 2
#define POT_PIN A5
#define JOYSTICK_X A4
#define JOYSTICK_Y A3
#define JOYSTICK_BTN 3
#define ANA_BUZZER A2
#define DIGI_BUZZER 4

// library objects
TonePlayer tone1 (TCCR1A, TCCR1B, OCR1AH, OCR1AL, TCNT1H, TCNT1L);  // pin D9
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {

  // pin modes
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_BTN, INPUT_PULLUP);
  pinMode(ANA_BUZZER, OUTPUT);
  pinMode(DIGI_BUZZER, OUTPUT);

}

// LED
void lightOn() {
  digitalWrite(LED, HIGH);
}
void lightOff() {
  digitalWrite(LED, LOW);
}

// buzzers
void digiBuzzerOn() {
  digitalWrite(DIGI_BUZZER, HIGH);
}
void digiBuzzerOff() {
  digitalWrite(DIGI_BUZZER, LOW);
}
void anaBuzzerOff() {
  noTone(ANA_BUZZER);
}

void button() {

  if (digitalRead(BUTTON) == 0) {
    pot();
  } else {
    anaBuzzerOff();
  }

  // if (digitalRead(BUTTON) == 0) {
  //   digiBuzzerOn();
  // } else {
  //   digiBuzzerOff();
  // }
}

void pot() {
  int pot_value = analogRead(POT_PIN);
  int ana_value = map(pot_value, 0, 1023, 262, 523);
  tone(ANA_BUZZER, ana_value);

  // if (pot_value > 512) {
  //   lightOn();
  // } else {
  //   lightOff();
  // }
}

void loop() {

  button();

}
