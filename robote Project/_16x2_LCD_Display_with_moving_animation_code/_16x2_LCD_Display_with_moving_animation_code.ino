#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x23, 16, 2);
int loadingCount = 1;

// setting animation speed (delay between frames)
int x = 400;
// setting time for Man waiting
int y = 5000;

// START setting all the pixel-art
//setting pixels for standing man:
//step 1 of 1:
  

byte bigR0[] = {B00000,B01111,B01111,B01111,B01111,B01111,B01111,B01111};
byte bigR1[] = {B00000,B11111,B11111,B00001,B00000,B00000,B00001,B11111};
byte bigR2[] = {B00000,B00000,B10000,B11000,B11100,B11100,B11000,B00000};
byte bigR3[] = {B01111,B01111,B01111,B01111,B01111,B01111,B01111,B00000};
byte bigR4[] = {B11111,B00111,B00011,B00001,B00000,B00000,B00000,B00000};
byte bigR5[] = {B10000,B10000,B11000,B11100,B11110,B01110,B01110,B00000};

byte lodingBox[]  = {B00000,B11111,B11111, B11111, B11111,B11111,B00000,B00000};


//setting up basics
void setup() {
 // set up the LCD's number of columns and rows:
 lcd.begin();
 // clear the LCD screen:
 lcd.clear();

//displayIcon();

//delay(1000);
//displayTitle();

//delay(1000);
//displayResetSystem();
//control();
configMode ();
//displayIP("192.168.256.135");
//delay(1000);
//displayAP("Supriya_Home");
         
}
    

//putting all the pieces together in a loop:
void loop() {
loadingv2();
delay(200);
}

// START putting al the pixel-art we defined before in his place for each animation part
// standing man animation part
void displayIcon(){

  lcd.clear();
  lcd.createChar(1, bigR0);
  lcd.createChar(2, bigR1);
  lcd.createChar(3, bigR2);
  lcd.createChar(4, bigR3);
  lcd.createChar(5, bigR4);
  lcd.createChar(6, bigR5); 
  
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.write(2);
  lcd.setCursor(2,0);
  lcd.write(3);
  lcd.setCursor(0,1);
  lcd.write(4);
  lcd.setCursor(1,1);
  lcd.write(5);
  lcd.setCursor(2,1);
  lcd.write(6);
}

void displayTitle(){

  byte smallO0[] = {B00000,B00000,B00000,B01110,B11011,B11011,B11011,B01110};
  byte smallB0[] = {B00000,B11000,B11000,B11110,B11011,B11011,B11011,B11110};
  byte smallR0[] = {B00000,B00000,B00000,B01101,B01111,B01100,B01100,B01100};

  lcd.clear();
  lcd.createChar(1, smallO0);
  lcd.createChar(2, smallB0);
  lcd.createChar(3, smallR0);

  lcd.setCursor(3,0);
  lcd.write(3);
  lcd.setCursor(4,0);
  lcd.write(1);
  lcd.setCursor(5,0);
  lcd.write(2);
  lcd.setCursor(6,0);
  lcd.write(1);
  
  lcd.setCursor(8,0);
  lcd.write(3);
  lcd.setCursor(9,0);
  lcd.write(1);
  lcd.setCursor(10,0);
  lcd.write(2);
  lcd.setCursor(2,1);
  lcd.print("smart robot");
}

void configMode (){
  byte settings0[] = {B00000,B00000,B00100,B01100,B10100,B10100,B10011,B01000};
  byte settings1[] = {B00000,B00000,B01000,B01100,B01010,B01010,B10010,B00100};
  byte settings2[] = {B00100,B00100,B00100,B00100,B11111,B11111,B00000,B00000};
  byte settings3[] = {B01000,B01000,B01000,B01000,B11110,B11110,B00000,B00000};

  lcd.clear();
  lcd.createChar(1, settings0);
  lcd.createChar(2, settings1);
  lcd.createChar(3, settings2);
  lcd.createChar(4, settings3);
  
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.write(2);
  lcd.setCursor(0,1);
  lcd.write(3);
  lcd.setCursor(1,1);
  lcd.write(4);

  lcd.setCursor(4,0);
  lcd.print("config mode");
}

void loading(){
  lcd.createChar(5, lodingBox);

  int dly = 200;
  for(int i=5; i<=13 ;i++){  
    lcd.setCursor(i,1);
    lcd.write(5);
    delay(dly);
    lcd.setCursor(i-1,1);
    lcd.print(" ");
  }
  for(int i=13; i>=5 ;i--) { 
    lcd.setCursor(i,1);
    lcd.write(5);
    delay(dly);
    lcd.print(" ");
  }
}

void loadingv2(){
  lcd.createChar(5, lodingBox);

  loadingCount = loadingCount>18 ? 1 : loadingCount;
                      
  if(loadingCount<10){
    int i = loadingCount+4;
    if(i>5){
      lcd.setCursor(i-2,1);
      lcd.print(" ");
    }
    lcd.setCursor(i,1);
    lcd.write(5);
    
  }else{
    int i = 19 - loadingCount+4;
    if(i<13){
      lcd.setCursor(i+1,1);
      lcd.print(" ");
    }
    
    lcd.setCursor(i,1);
    lcd.write(5);
  }
  loadingCount++;
}

void displayIP(String ip){
  
  byte small_I[] = {B00000,B01110,B00100,B00100,B00100,B01110,B00000,B00000};
  byte small_P[] = {B00000,B01110,B01010,B01110,B01000,B01000,B00000,B00000};

  lcd.clear();
  lcd.createChar(1, small_I);
  lcd.createChar(2, small_P);

  lcd.setCursor(0,0);
  lcd.print("-------");
  lcd.setCursor(7,0);
  lcd.write(1);
  lcd.setCursor(8,0);
  lcd.write(2);
  lcd.setCursor(9,0);
  lcd.print("-------");
  
  //lcd.setCursor(1,0);
  //lcd.print(ap.substring(0, 15));
  lcd.setCursor((16%ip.length())/2,1);
  lcd.print(ip);
}

void displayAP(String ap){

  byte small_A[] = {B00000,B01110,B01010,B01110,B01010,B01010,B00000,B00000};
  byte small_P[] = {B00000,B01110,B01010,B01110,B01000,B01000,B00000,B00000};

  lcd.clear();
  lcd.createChar(1, small_A);
  lcd.createChar(2, small_P);

  lcd.setCursor(0,0);
  lcd.print("-------");
  lcd.setCursor(7,0);
  lcd.write(1);
  lcd.setCursor(8,0);
  lcd.write(2);
  lcd.setCursor(9,0);
  lcd.print("-------");
  
  lcd.setCursor(ap.length() > 16 ?  0 : (16%ap.length())/2,1);
  lcd.print(ap.substring(0, 16));
}



void control(){

  byte dir_f[]={B00000,B00100,B01110,B11111,B00100,B00100,B00000,B00000};
  byte dir_b[]={B00000,B00100,B00100,B11111,B01110,B00100,B00000,B00000};
  byte dir_l[]={B00000,B00100,B01100,B11111,B01100,B00100,B00000,B00000};
  byte dir_r[]={B00000,B00100,B00110,B11111,B00110,B00100,B00000,B00000};
  byte dir_f_na[] = {B11111,B00100,B01110,B11111,B00100,B00100,B00100,B00000};
  byte dir_b_na[] = {B00000,B00100,B00100,B00100,B11111,B01110,B00100,B11111};
  
  byte small_i[] = {B01110,B00100,B00100,B00100,B01110,B00000,B00000,B00000};
  byte small_n[] = {B01110,B01010,B01010,B01010,B01010,B00000,B00000,B00000};
  byte small_f[] = {B01110,B01000,B01100,B01000,B01000,B00000,B00000,B00000};
  byte small_o[] = {B01110,B01010,B01010,B01010,B01110,B00000,B00000,B00000};
  
  lcd.clear();
  lcd.createChar(1, small_i);
  lcd.createChar(2, small_n);
  lcd.createChar(3, small_f);
  lcd.createChar(4, small_o);
  
  lcd.createChar(6, dir_f);
  lcd.createChar(7, dir_f_na);
  lcd.createChar(8, dir_b_na);

  lcd.setCursor(6,0);
  lcd.write(1);
  lcd.setCursor(7,0);
  lcd.write(2);
  lcd.setCursor(8,0);
  lcd.write(3);
  lcd.setCursor(9,0);
  lcd.write(4);
  lcd.setCursor(0,0);
  lcd.print("------");
  lcd.setCursor(10,0);
  lcd.print("------");

  lcd.setCursor(0,1);
  lcd.print("D:");
  lcd.setCursor(2,1);
  lcd.write(6);
  
  lcd.setCursor(6,1);
  lcd.print("S:");
  lcd.setCursor(8,1);
  lcd.print("5");

  lcd.setCursor(12,1);
  lcd.print("O:");
  lcd.setCursor(14,1);
  lcd.write(7);
  lcd.setCursor(15,1);
  lcd.write(8);

}
void displayResetSystem(){
  
  byte reset0[] = {B00011,B11111,B11000,B01010,B01010,B01010,B01000,B01111};
  byte reset1[] = {B11000,B11111,B00011,B10010,B10010,B10010,B00010,B11110};

  lcd.clear();
  lcd.createChar(1, reset0);
  lcd.createChar(2, reset1);

  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.write(2);

  lcd.setCursor(4,0);
  lcd.print("Resetting...");
}


