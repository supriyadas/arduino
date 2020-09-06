// defines pins numbers
int motor1[2] = {2,3};//Pulse,Direction
int motor2[2] = {4,5};
int motor3[2] = {6,7};
int motor4[2] = {8,9};

const int multi = 6400;

const int del1 = 0;
const int del = 80;

void setup() {
  
  pinMode(motor1[0],OUTPUT); 
  pinMode(motor1[1],OUTPUT);
  
  pinMode(motor2[0],OUTPUT); 
  pinMode(motor2[1],OUTPUT);
  
  pinMode(motor3[0],OUTPUT); 
  pinMode(motor3[1],OUTPUT);
  
  pinMode(motor4[0],OUTPUT); 
  pinMode(motor4[1],OUTPUT);

   Serial.begin(9600);

}
void loop() {

  set_motors_direction('U','U','U','U');
  move_motor(1,1,1,1);
  delay(del1);
  
  //part 1
  set_motors_direction('U','U','D','D');
  move_motor(1,1,1,1);
  delay(del1);
 //part 2
  set_motors_direction('D','D','U','U');
  move_motor(2,2,2,2);
  delay(del1); 
  //part 3
   set_motors_direction('U','U','D','D');
  move_motor(2,2,2,2);
  delay(del1); 
  //part 4
  set_motors_direction('D','D','U','U');
  move_motor(1,1,1,1);
delay(del1); 
  //====Balance====//

  //part 5
  set_motors_direction('U','S','S','D');
  move_motor(1,0,0,1);
  delay(del1); 
  //part 6
  set_motors_direction('D','D','U','U');
  move_motor(1,1,1,1);
  delay(del1);   
 
  //part 7
  set_motors_direction('D','U','D','U');
  move_motor(1,1,1,1);
delay(del1); 
  //part 8
  set_motors_direction('U','U','D','D');
  move_motor(1,1,1,1);
delay(del1); 
  //part 9
  set_motors_direction('S','D','U','S');
  move_motor(0,1,1,0);
delay(del1); 
  //====Balance====//

  //part 10
  set_motors_direction('U','D','U','D');
  move_motor(1,1,1,1);
  delay(del1); 
  //part 11
  set_motors_direction('D','U','D','U');
  move_motor(1,1,1,1);
  delay(del1); 
  
  //part 12
  set_motors_direction('U','D','U','D');
  move_motor(1,1,1,1);
  delay(del1); 
  //part 13
  set_motors_direction('D','U','D','U');
  move_motor(1,1,1,1);
delay(del1);

set_motors_direction('D','D','D','D');
  move_motor(1,1,1,1);
  delay(del1);
}

void set_motors_direction(char a1, char a2, char a3, char a4){

  set_direction(1, a1);
  set_direction(2, a2);
  set_direction(3, a3);
  set_direction(4, a4);

}

void set_direction(int motor_no, char direc) {
  switch (direc) {
  case 'U':
    switch (motor_no) {
      case 1:
        digitalWrite(motor1[1], HIGH);
        break;
      case 2:
        digitalWrite(motor2[1], HIGH);
        break;
      case 3:
        digitalWrite(motor3[1], HIGH);
        break;
      case 4:
        digitalWrite(motor4[1], HIGH);
        break;
    }
    break;
  case 'D':
    switch (motor_no) {
      case 1:
        digitalWrite(motor1[1], LOW);
        break;
      case 2:
        digitalWrite(motor2[1], LOW);
        break;
      case 3:
        digitalWrite(motor3[1], LOW);
        break;
      case 4:
        digitalWrite(motor4[1], LOW);
        break;
    }
    break;
  }

}

void move_motor(int b1, int b2, int b3, int b4) {

  int m = get_max(b1, b2, b3, b4)*multi;

  int m1 = b1 * multi;
  int m2 = b2 * multi;
  int m3 = b3 * multi;
  int m4 = b4 * multi; 
  boolean f1, f2, f3, f4;
  
   Serial.println(m1);
   
  for(int x = 0; x < m; x++) {

    f1 = x < m1 ? true : false;
    f2 = x < m2 ? true : false;
    f3 = x < m3 ? true : false;
    f4 = x < m4 ? true : false;


    if(f1)
      digitalWrite(motor1[0],HIGH); 
    if(f2)
      digitalWrite(motor2[0],HIGH);
    if(f3)
      digitalWrite(motor3[0],HIGH);
    if(f4)
      digitalWrite(motor4[0],HIGH);
      
    //delayMicroseconds(del); 
    
    if(f1)
      digitalWrite(motor1[0],LOW); 
    if(f2)
      digitalWrite(motor2[0],LOW);
    if(f3)
      digitalWrite(motor3[0],LOW);
    if(f4)
      digitalWrite(motor4[0],LOW);

    delayMicroseconds(del); 
  }
}

int get_max(int a, int b, int c, int d){

   int max = (a > b? a : b);  
   max = (c > d? (c > max? c : max) : (d > max? d : max));

   return max;
}
