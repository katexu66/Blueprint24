#include <NewPing.h>
#include 

#define BUZZER_PWM_PIN 9

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 50 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pinMode(BUZZER_PWM_PIN, OUTPUT);
  analogWrite(BUZZER_PWM_PIN, 512);
}

void setBuzzer(int val, int min, int max) {
  int scaled = getTone(map(val, min, max, 0, 12));
  Serial.println(scaled);
  //tone(BUZZER_PWM_PIN, scaled);
}

int getTone(int value) {
  return 440 * pow(1.0594631, value);
}

void loop() {
  delay(5);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  int distance = sonar.ping_cm();
  Serial.print(distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("carcm");
  setBuzzer(distance, 0, MAX_DISTANCE);
}