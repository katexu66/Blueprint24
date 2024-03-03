#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUZZER 3

#define JOYSTICK_X A1
#define JOYSTICK_Y A2
#define JOYSTICK_BTN 7

int bpm = 100;


void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  Serial.begin(9600);
  display_bpm();

  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_BTN, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
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
  // put your main code here, to run repeatedly:
  metronome_bpm();
  metronome_sound();
}
