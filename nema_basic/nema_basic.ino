// defines pins numbers
const int stepPin1 = 2; 
const int dirPin1 = 3; 

const int stepPin2 = 4; 
const int dirPin2 = 5; 

const int stepPin3 = 6; 
const int dirPin3 = 7; 

const int stepPin4 = 8; 
const int dirPin4 = 9; 


String readString;
boolean move = false;

boolean move1 = false;
boolean move2 = false;
boolean move3 = false;
boolean move4 = false;


int del = 50 ;

void setup() {
  
  // Sets the two pins as Outputs
  pinMode(stepPin1,OUTPUT); 
  pinMode(dirPin1,OUTPUT);
  
  pinMode(stepPin2,OUTPUT); 
  pinMode(dirPin2,OUTPUT);
  
  pinMode(stepPin3,OUTPUT); 
  pinMode(dirPin3,OUTPUT);
  
  pinMode(stepPin4,OUTPUT); 
  pinMode(dirPin4,OUTPUT);
  
  digitalWrite(dirPin1,HIGH); 
  digitalWrite(dirPin2,HIGH); 
  digitalWrite(dirPin3,HIGH); 
  digitalWrite(dirPin4,HIGH); 
  
  Serial.begin(9600);
}
void loop() {

 while(Serial.available()){ // the code here is for bluetooth
  delay(3);
  char c = Serial.read();
  readString += c;
  }
  if(readString.length() > 0){

    if(readString == "A")  {

      move1 = true;
      move2 = false;
      move3 = false;
      move4 = false;
      
    }else if(readString == "B"){

      move1 = false;
      move2 = true;
      move3 = false;
      move4 = false;
    
    }else if(readString == "C"){

      move1 = false;
      move2 = false;
      move3 = true;
      move4 = false;
    
    }else if(readString == "D"){

      move1 = false;
      move2 = false;
      move3 = false;
      move4 = true;
    
    }else if(readString == "AB"){

      move1 = true;
      move2 = true;
      move3 = false;
      move4 = false;
    
    }else if(readString == "CD"){

      move1 = false;
      move2 = false;
      move3 = true;
      move4 = true;
    
    }else if(readString == "AD"){

      move1 = true;
      move2 = false;
      move3 = false;
      move4 = true;
    
    }else if(readString == "BC"){

      move1 = false;
      move2 = true;
      move3 = true;
      move4 = false;
      
    }else if(readString == "ABCD"){

      move1 = true;
      move2 = true;
      move3 = true;
      move4 = true;
      
    }else if(readString == "up")  {
      
      digitalWrite(dirPin1,HIGH); 
      digitalWrite(dirPin2,HIGH);
      digitalWrite(dirPin3,HIGH);
      digitalWrite(dirPin4,HIGH);
      
      move = move ? false : true;
      
    }else if(readString == "down"){
      
      digitalWrite(dirPin1,LOW);
      digitalWrite(dirPin2,LOW);
      digitalWrite(dirPin3,LOW);
      digitalWrite(dirPin4,LOW);
      
      move = move ? false : true;
      
    }else if(readString == "su"){
      
      del = del + 5;
      Serial.println(del);
      
    }else if(readString == "sd"){
      
      del = del - 5;
      Serial.println(del);
      
    }

    Serial.println(readString);
  }

  if(move){
     if(move1){
      digitalWrite(stepPin1,HIGH);
     } 
     if(move2){
      digitalWrite(stepPin2,HIGH);
     }
     if(move3){
      digitalWrite(stepPin3,HIGH);
     }
     if(move4){
      digitalWrite(stepPin4,HIGH);
     }
     
     delayMicroseconds(del); 

     if(move1){
      digitalWrite(stepPin1,LOW);
     } 
     if(move2){
      digitalWrite(stepPin2,LOW);
     }
     if(move3){
      digitalWrite(stepPin3,LOW);
     }
     if(move4){
      digitalWrite(stepPin4,LOW);
     } 
     
     delayMicroseconds(del); 
  }
  readString="";
}
