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
                    GND[------------------]7 Joystick button
                    Vin[------------------]6
                     A0[------------------]5
          joystick x A1[------------------]4
potentiometer buzzer A2[------------------]3 metronnome buzzer
       potentiometer A3[------------------]2 button for potentiometer
                 SDA A4[------------------]1
                 SCL A5[------------------]0



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
#define BUTTON 2
#define POT_PIN A3
#define JOYSTICK_X A4
#define JOYSTICK_BTN 3
#define ANA_BUZZER A2

// library objects
TonePlayer tone1 (TCCR1A, TCCR1B, OCR1AH, OCR1AL, TCNT1H, TCNT1L);  // pin D9
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// options
#define bpm 6000
int sampleRate = 60000 / bpm;

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud

  // pin modes
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_BTN, INPUT_PULLUP);
  pinMode(ANA_BUZZER, OUTPUT);
  pinMode(BUZZER_PWM_PIN, OUTPUT);
  analogWrite(BUZZER_PWM_PIN, 512);

}

// buzzers
void anaBuzzerOff() {
  noTone(ANA_BUZZER);
}

void setDistanceBuzzer(int val, int min, int max) {
  if (val == 0) {
    return;
  }
  int scaled = getTone(map(val, min, max, 0, 12));
  Serial.println(scaled);
  tone1.tone(scaled);
}

void distSensorUpdate() {
  delay(sampleRate);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  int distance = sonar.ping_cm();
  Serial.print(distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  setDistanceBuzzer(distance, 0, MAX_DISTANCE);
}

// control
void button() {
  if (digitalRead(BUTTON) == 0) {
    pot();
  } else {
    anaBuzzerOff();
  }
}

void pot() {
  if (analogRead(POT_PIN) > 512) {
      int ana_value = scaled*1.0595*3;
      tone(ANA_BUZZER, ana_value);
  }
}

// processing 
int getTone(int value) {
  return 440 * pow(1.0594631, value);
}

void loop() {
  distSensorUpdate();
  button();
}
