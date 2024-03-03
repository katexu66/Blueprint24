/*
5 - 1s
6 - 2s
8 - 4s
10 - 8s

*/

#define ONE 5
#define TWO 6
#define FOUR 8
#define EIGHT 10
#define BUZZER 3

void setup () {
  // set up computer and master serial
  Serial.begin(115200);
  while(!Serial);
  Serial.println("slave start");
  pinMode(ONE, INPUT);
  pinMode(TWO, INPUT);
  pinMode(FOUR, INPUT);
  pinMode(EIGHT, INPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  delay(100);
  int one = digitalRead(ONE);
  int two = digitalRead(TWO);
  int four = digitalRead(FOUR);
  int eight = digitalRead(EIGHT);

  Serial.print(eight); Serial.print(" ");
  Serial.print(four); Serial.print(" ");
  Serial.print(two); Serial.print(" ");
  Serial.print(one); Serial.println();

  Serial.print("sum: ");
  int sum = 8*eight+4*four+2*two+one;
  Serial.println(sum);
  tone(BUZZER, (440 * pow(1.0594631, sum)));
}