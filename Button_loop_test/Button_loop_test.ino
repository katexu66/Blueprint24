#define BUTTON 2

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
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

void loop() {
  // put your main code here, to run repeatedly:
  buttonPress();
  Serial.println(buttonPresses);
}
