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
#include <Adafruit_SSD1306.h>

// tone library hack
#include <TonePlayer.h>

// ports
#define BUZZER_PWM_PIN 9
#define TRIGGER_PIN 12   // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 11      // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 48  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define BUTTON 2
#define POT_PIN A3
#define JOYSTICK_X A1
#define JOYSTICK_BTN 7
#define ANA_BUZZER A2
#define BUZZER 3

// screen display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// library objects
TonePlayer tone1(TCCR1A, TCCR1B, OCR1AH, OCR1AL, TCNT1H, TCNT1L);  // pin D9
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);                // NewPing setup of pins and maximum distance.

// options
#define bpm 2000 // max: 2000 for "smooth" sampling
int sampleRate = 60000 / bpm;
#define INERTIA_MODE true
#define INERTIA_AMOUNT 10 // number of previous values remembered

// var for metronome
int bpm = 100;

void setup() {
  Serial.begin(115200);  // Open serial monitor at 115200 baud

  // pin modes
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(ANA_BUZZER, OUTPUT);
  pinMode(BUZZER_PWM_PIN, OUTPUT);
  analogWrite(BUZZER_PWM_PIN, 512);

  // screen display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display_bpm();
}

// processing 
int getTone(int value) {
  return 440 * pow(1.0594631, value);
}

int prevDists[INERTIA_AMOUNT]; // store last twenty samplings to average out and make changes smoother
// remove oldest sample and add new, then return new average
int nextSample(int newValue) {
  for (int i = 1; i < INERTIA_AMOUNT; i++) {
    prevDists[i-1] = prevDists[i];
  }
  prevDists[INERTIA_AMOUNT-1] = newValue;
  int sum = 0;
  for (int i = 0; i < INERTIA_AMOUNT; i++) {
    sum += prevDists[i];
  }
  return sum/INERTIA_AMOUNT;
}

// buzzers
void anaBuzzerOff() {
  noTone(ANA_BUZZER);
}

void setDistanceBuzzer(int val, int min, int max) {
  int scaled = getTone(map(val, min, max, 0, 12));
  Serial.println(scaled);
  tone1.tone(scaled);
}

void distSensorUpdate() {
  delay(sampleRate);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
<<<<<<< HEAD
  int distance = sonar.ping_cm();
  Serial.print(distance);  // Send ping, get distance in cm and print result (0 = outside set distance range)
=======
  int newDistance = sonar.ping_cm();
  Serial.print(newDistance); // Send ping, get distance in cm and print result (0 = outside set distance range)
>>>>>>> c9d9f4000ebc6122c5338ebf39bba8c5abcb55e3
  Serial.println("cm");
  if (newDistance != 0) {
    #ifdef INERTIA_MODE
    int newAverage = nextSample(newDistance);
    for (int i = 0; i < 20; i++) {
      Serial.print(prevDists[i]); 
      Serial.print(", ");
    }
    Serial.println();
    Serial.println(newAverage);
    setDistanceBuzzer(newAverage, 0, MAX_DISTANCE);
    #else 
    setDistanceBuzzer(newDistance, 0, MAX_DISTANCE);
    #endif
  }
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
  int pot_value = analogRead(POT_PIN);
  int ana_value = map(pot_value, 0, 1023, 262, 523);
  tone(ANA_BUZZER, ana_value);
}

<<<<<<< HEAD
// processing
int getTone(int value) {
  return 440 * pow(1.0594631, value);
}


// function to change bpm
void metronome_bpm() {
  int joystick_value_x = analogRead(JOYSTICK_X);
  if (joystick_value_x >= 700) {
    // moving joystick to the right raises bpm
    bpm += 1;
    display_bpm();
  } else if (joystick_value_x <= 300) {
    // moving joystick to the left lowers bpm
    bpm -= 1;
    display_bpm();
  }
}

// function to play metronome at the correct bpm
void metronome_sound() {

  digitalWrite(BUZZER, HIGH);
  delay(10);
  digitalWrite(BUZZER, LOW);
  delay(60000 / bpm);
}

// function to display current bpm
void display_bpm() {
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(28, 22);
  display.println(bpm);
  display.display();
}

=======
>>>>>>> c9d9f4000ebc6122c5338ebf39bba8c5abcb55e3
void loop() {
  distSensorUpdate();
  button();

  // play and calculate metronome
  metronome_bpm();
  metronome_sound();
}
