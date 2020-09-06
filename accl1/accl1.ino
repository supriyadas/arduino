
    int IN[8]  = {2,3,4,5,8,9,10,11};
    int Steps = 0, i=0;
    boolean Direction = true;
    unsigned long currentMillis ;
    int steps_left=1000;

    void setup()
    {
      
    Serial.begin(115200);
    
      for(i=0;i<8;i++){
        pinMode(IN[i], OUTPUT); 
      }

    }
    void loop()
    {
      while(steps_left>0){

  
        stepper(1,1);
        stepper(1,2);
        steps_left--;
        delayMicroseconds(2000);
      }
      //delay(500);
    }

    void stepper(int xw, int m){
      for (int x=0;x<xw;x++){
        switch(Steps){
           case 0:
             digitalWrite(IN[1*m-1], LOW); 
             digitalWrite(IN[2*m-1], LOW);
             digitalWrite(IN[3*m-1], LOW);
             digitalWrite(IN[4*m-1], HIGH);
           break; 
           case 1:
             digitalWrite(IN[1*m-1], LOW); 
             digitalWrite(IN[2*m-1], LOW);
             digitalWrite(IN[3*m-1], HIGH);
             digitalWrite(IN[4*m-1], HIGH);
           break; 
           case 2:
             digitalWrite(IN[1*m-1], LOW); 
             digitalWrite(IN[2*m-1], LOW);
             digitalWrite(IN[3*m-1], HIGH);
             digitalWrite(IN[4*m-1], LOW);
           break; 
           case 3:
             digitalWrite(IN[1*m-1], LOW); 
             digitalWrite(IN[2*m-1], HIGH);
             digitalWrite(IN[3*m-1], HIGH);
             digitalWrite(IN[4*m-1], LOW);
           break; 
           case 4:
             digitalWrite(IN[1*m-1], LOW); 
             digitalWrite(IN[2*m-1], HIGH);
             digitalWrite(IN[3*m-1], LOW);
             digitalWrite(IN[4*m-1], LOW);
           break; 
           case 5:
             digitalWrite(IN[1*m-1], HIGH); 
             digitalWrite(IN[2*m-1], HIGH);
             digitalWrite(IN[3*m-1], LOW);
             digitalWrite(IN[4*m-1], LOW);
           break; 
             case 6:
             digitalWrite(IN[1*m-1], HIGH); 
             digitalWrite(IN[2*m-1], LOW);
             digitalWrite(IN[3*m-1], LOW);
             digitalWrite(IN[4*m-1], LOW);
           break; 
           case 7:
             digitalWrite(IN[1*m-1], HIGH); 
             digitalWrite(IN[2*m-1], LOW);
             digitalWrite(IN[3*m-1], LOW);
             digitalWrite(IN[4*m-1], HIGH);
           break; 
           default:
             digitalWrite(IN[1*m-1], LOW); 
             digitalWrite(IN[2*m-1], LOW);
             digitalWrite(IN[3*m-1], LOW);
             digitalWrite(IN[4*m-1], LOW);
           break; 
        }
        SetDirection();
      }
    } 
    void SetDirection(){
      if(Direction==1){ Steps++;}
      if(Direction==0){ Steps--; }
      if(Steps>7){Steps=0;}
      if(Steps<0){Steps=7; }
    }
