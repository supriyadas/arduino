/*
 * HC-SR04 example sketch
 *
 * https://create.arduino.cc/projecthub/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-036380
 *
 * by Isaac100
 */

const int trigPin = 9;
const int echoPin1 = 10;
const int echoPin2 = 11;
const int echoPin3 = 12;

float duration, distance1, distance2, distance3;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin1, HIGH);
  distance1 = (duration*.0343)/2;

  duration = pulseIn(echoPin2, HIGH);
  distance2 = (duration*.0343)/2;

  duration = pulseIn(echoPin3, HIGH);
  distance3 = (duration*.0343)/2;

  
  Serial.print("Distance: ");
  Serial.print(distance1);
  Serial.print(" ");
  Serial.print(distance2);
  Serial.print(" ");
  Serial.println(distance3);
  delay(100);
}
