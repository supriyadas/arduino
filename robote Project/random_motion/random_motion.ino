#include<time.h> 

// Ultrasonic Front Pins
const int trigPin1 = 3;
const int echoPin1 = 2;

// Motor Pins
const int M1_IN_1 = 6;
const int M1_IN_2 = 7;
const int M2_IN_1 = 8;
const int M2_IN_2 = 9;

// Ultrasolic LED indicator
const int BI_LED_PIN = 13;
const int ledPinRare = 12;

unsigned long lastUpdated;
long thresholdMove = 3500;
long threshold = 8;
double lastTheta = 80.0;

void setup() {
 
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(M1_IN_1, OUTPUT); 
  pinMode(M1_IN_2, OUTPUT);
  pinMode(M2_IN_1, OUTPUT);
  pinMode(M2_IN_2, OUTPUT);

  pinMode(BI_LED_PIN, OUTPUT);
  pinMode(ledPinRare, OUTPUT);
  lastUpdated=millis();
  srand(time(0)); 
  Serial.begin(115200);
}


void loop() {

  int distance = SonarSensor(trigPin1, echoPin1);
  //Serial.println(distance);
  if(distance<threshold){
    digitalWrite(BI_LED_PIN, HIGH);
  }else{
    digitalWrite(BI_LED_PIN, LOW);
  }
//  if(distance<threshold){
//    long startTime = millis();
//    stop_motors();
//    backward();
//    delay(1000);
//    stop_motors();
//    
//    if(lastTheta<0){
//      turn_left(); 
//      delay(6.944*(-1*lastTheta));
//    }else{
//      turn_right();        
//      delay(6.944*(lastTheta));
//    }
//    stop_motors();
//    long endTime = millis();
//    thresholdMove = thresholdMove+(endTime-startTime);
//    forward();
//  }
  
  if(distance<threshold || (millis()-lastUpdated)>thresholdMove){

    double gain=1.8;
    double xtime = gain*2.8;
    double ytime = gain*3.2;
    boolean skip = false;
    if(distance<threshold){
      stop_motors();
      backward();
      delay(1300);
      stop_motors();
      turn_left();
      delay(6.944*180);
      stop_motors();
      forward();
      thresholdMove = xtime*1000;
      lastUpdated=millis();
      skip = true;
    }
    if(!skip){
      stop_motors();
  
      double x = xtime*sampleNormal();
      double y = ytime*sampleNormal();
      double r = sqrt(x*x + y*y);
      double theta = 180/3.14*atan2(y,x);
      if(theta>0){
        turn_right();
        delay(6.944*theta);
        stop_motors();
      }else{
        turn_left(); 
        delay(6.944*(-1*theta));
        stop_motors();
      }
      forward();
      thresholdMove = r*1000;
      lastTheta= theta;
      lastUpdated=millis();
      Serial.print(thresholdMove);
      Serial.print("=>");
      Serial.println(theta);
    }
  }
}

double sampleNormal() {
    double u = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double v = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double r = u * u + v * v;
    if (r == 0 || r > 1) return sampleNormal();
    double c = sqrt(-2 * log(r) / r);
    return u * c;
}

int SonarSensor(int trigPin,int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = (duration-10) * 0.034 / 2;
  return distance;
}


void forward() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
}

void backward() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, HIGH);
  digitalWrite(M2_IN_2, LOW);
}

void turn_right() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M2_IN_1, HIGH);
  digitalWrite(M2_IN_2, LOW);
}

void turn_left() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
}

void stop_motors() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
}
