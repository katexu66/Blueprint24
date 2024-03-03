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
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println(bpm);
  display.display();

  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_BTN, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);

  Serial.print(analogRead(JOYSTICK_X));
}

void metronome_bpm() {
  int joystick_value_x = analogRead(JOYSTICK_X);
  if (joystick_value_x >= 700) {
    bpm += 1;
    display.println(bpm);
    Serial.println(bpm);
  } else if (joystick_value_x <= 300) {
    bpm -= 1;
    display.println(bpm);
    Serial.println(bpm);
  }
}

void metronome_sound() {
  int joystick_btn_value = digitalRead(JOYSTICK_BTN);
  Serial.print(joystick_btn_value);
  if (joystick_btn_value == LOW) {
    digitalWrite(BUZZER, LOW);
  } else {
    digitalWrite(BUZZER, HIGH);
    delay(10);
    digitalWrite(BUZZER, LOW);
    delay(60000 / bpm);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  metronome_bpm();
  metronome_sound();
}
