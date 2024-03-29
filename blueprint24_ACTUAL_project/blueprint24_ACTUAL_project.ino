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
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define BUTTON 2
#define POT_PIN A3
#define JOYSTICK_X A1
#define JOYSTICK_BTN 7
#define ANA_BUZZER A2
#define BUZZER 3
#define ONE 5
#define TWO 6
#define FOUR 8
#define EIGHT 10

// screen display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// options
#define bpm 2000 // max: 2000 for "smooth" sampling
int sampleRate = 60000 / bpm;
#define INERTIA_MODE false
#define INERTIA_AMOUNT 10 // number of previous values remembered
#define MAX_DISTANCE 96 // Maximum distance to ping at (cm). Maximum sensor distance is rated at 400-500cm.

// library objects
TonePlayer tone1 (TCCR1A, TCCR1B, OCR1AH, OCR1AL, TCNT1H, TCNT1L);  // pin D9
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// var for metronome
int tempo = 100;

// processing 

/*
0 - "free": half steps
1 - "maj/major": c major scale
3 - "min/minor": a minor scale
2 - "pen/pentatonic" pentatonic scale
4 - "arp/arpeggio" c arpeggio
*/
int getTone(int note, int option) {
  switch (option) {
    case 0:
      return 440 * pow(1.0594631, note);
    case 1:
      switch (note) {
        case 0:
          return 523; // C5
        case 1:
          return 587; // D5
        case 2:
          return 659; // E5
        case 3:
          return 698; // F5
        case 4:
          return 784; // G5
        case 5:
          return 880; // A5
        case 6:
          return 988; // B5
        case 7:
          return 1047; // C6
        case 8:
          return 1175; // D6
        case 9:
          return 1319; // E6
        case 10:
          return 1397; // F6
        case 11:
          return 1568; // G6
        case 12:
          return 1760; // A6
        case 13:
          return 1976; // B6
        case 14:
          return 2093; // C7
        default:
          return 262; // C4
      }
    case 2:
      switch (note) {
        case 0:
          return 523; // C5
        case 1:
          return 587; // D5
        case 2:
          return 659; // E5
        case 3:
          return 784; // G5
        case 4:
          return 880; // A5
        case 5:
          return 1047; //C6
      }
    default:
     return 440;
  }
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
  //noTone(ANA_BUZZER);
}

int setDistanceBuzzer(int val, int min, int max, int scaleOption) {
  int scaleMax = 0;
  switch (scaleOption) {
    case 0:
      scaleMax = 12;
      break;
    case 1:
      scaleMax = 14;
      break;
    case 2:
      scaleMax = 5;
      break;
    default:
      scaleMax = 0;
  }
  int note = getTone(map(val, min, max, 0, scaleMax), scaleOption);
  Serial.println(note);
  tone1.tone(note);
  return note;
}

int ping() {
  Serial.print("Ping: ");
  int ping = sonar.ping_cm();
  Serial.print(ping); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  return ping;
}

int buzzerUpdate(int pingDistance, int scaleOption) {
  #ifdef INERTIA_MODE
  int newAverage = nextSample(pingDistance);
  for (int i = 0; i < 20; i++) {
    Serial.print(prevDists[i]); 
    Serial.print(", ");
  }
  Serial.println();
  Serial.println(pingDistance);
  return setDistanceBuzzer(newAverage, 0, MAX_DISTANCE, scaleOption);
  #else 
  return setDistanceBuzzer(pingDistance, 0, MAX_DISTANCE, scaleOption);
  #endif
}

// speaker module
void writeSpeaker(uint8_t note) {
  bool eight = (note >> 3) & (uint8_t)1;
  bool four = (note >> 2) & (uint8_t)1;
  bool two = (note >> 1) & (uint8_t)1;
  bool one = (note >> 0) & (uint8_t)1;
  Serial.print("original number: ");
  Serial.println(note);
  Serial.print("bits: ");
  Serial.print(eight); Serial.print(" ");
  Serial.print(four); Serial.print(" ");
  Serial.print(two); Serial.print(" ");
  Serial.print(one); Serial.println();
  digitalWrite(EIGHT, eight);
  digitalWrite(FOUR, four);
  digitalWrite(TWO, two);
  digitalWrite(ONE, one);
}

// input 
void button(int note) {
  if (digitalRead(BUTTON) == 0) {
    pot(note);
  } else {
    anaBuzzerOff();
  }
}

void pot(int note) {
  int pot_value = analogRead(POT_PIN);
  //int scaled = getTone(map(val, min, max, 0, 12), 1);
  int ana_value = map(pot_value, 0, 1023, 262, 523);
  if (analogRead(POT_PIN) > 512) {
    int ana_value = note*3.1785;
    //tone(ANA_BUZZER, ana_value);
  } else {
    int ana_value = note/3.1785;
    // tone(ANA_BUZZER, ana_value);
  }
}

// processing
int getTone(int value) {
  return 440 * pow(1.0594631, value);
}


// function to change bpm
void metronome_bpm() {
  int joystick_value_x = analogRead(JOYSTICK_X);
  Serial.print("joystick: ");
  Serial.println(joystick_value_x);
  if (joystick_value_x >= 700) {
    // moving joystick to the right raises bpm
    tempo += 1;
    display_bpm();
  } else if (joystick_value_x <= 300) {
    // moving joystick to the left lowers bpm
    tempo -= 1;
    display_bpm();
  }
}

// function to play metronome at the correct tempo
void metronome_sound() {

  digitalWrite(BUZZER, HIGH);
  delay(10);
  digitalWrite(BUZZER, LOW);
  //delay(60000 / tempo);
}

// function to display current bpm
void display_bpm() {
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(28, 22);
  display.println(tempo);
  display.display();
}
  
int buttonPresses = 0;

void buttonPress() {
  if (digitalRead(BUTTON) == 0) {
    if (buttonPresses == 5) {
      buttonPresses = 0;
      delay(500);
    } else {
      buttonPresses = buttonPresses + 1;
      delay(500);
    }
  }
}

void setup() {
  Serial.begin(115200);  // Open serial monitor at 115200 baud
  while(!Serial);
  Serial.println("begin");

  // pin modes
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(ANA_BUZZER, OUTPUT);
  pinMode(BUZZER_PWM_PIN, OUTPUT);
  analogWrite(BUZZER_PWM_PIN, 512);
  pinMode(ONE, OUTPUT);
  pinMode(TWO, OUTPUT);
  pinMode(FOUR, OUTPUT);
  pinMode(EIGHT, OUTPUT);

  // screen display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display_bpm();
  Serial.println("end init");
}

int metct = 0;
void loop() {
  delay(sampleRate);
  buttonPress();
  int distCm = ping();
  int note = 0;
  if (distCm != 0) {
    note = buzzerUpdate(distCm, buttonPresses);
  }

  // play and calculate metronome
  if (metct == 0) {
    metronome_bpm();
    metronome_sound();
  }
  metct++;
  metct = metct % 20;

  writeSpeaker(note+4);
}
