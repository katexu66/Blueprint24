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
#define bpm 6000
int sampleRate = 60000 / bpm;

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
  Serial.print(distance);  // Send ping, get distance in cm and print result (0 = outside set distance range)
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
  int pot_value = analogRead(POT_PIN);
  int ana_value = map(pot_value, 0, 1023, 262, 523);
  tone(ANA_BUZZER, ana_value);
}

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

void loop() {
  distSensorUpdate();
  button();

  // play and calculate metronome
  metronome_bpm();
  metronome_sound();
}
