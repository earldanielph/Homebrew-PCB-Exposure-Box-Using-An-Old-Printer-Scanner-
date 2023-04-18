// Homebrew PCB Exposure Box Using An Old Printer Scanner
// Evtech | @earldanielph
// beacons.page/evtech

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LEDs 12          
#define buzzer 11       
#define button A3        
#define durationPot A0   
#define brightnessPot A1 

int 
maxTime = 1200,                
buzzerFreq = 1000,      
glowDelay = 10;          

float 
timeLeftBarPercent = 0.00,
durationSecFloat = 0.00,
durationSecSelect = 0.00;
int
whitePWM = 0,
duration = 0,
durationSec = 0,
brightness = 0,
brightnessPercent = 0,
brightnessPWM = 0,
seconds = 0,
minutes = 0,
exposureLoop = 0,
startupLoop = 0;
unsigned long previousMillis = 0;    
const long interval = 1000;  
char timeMinSec[16];

void setup(){
  Wire.begin();
  pinMode(LEDs, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();  
  
  brightnessPWM = map(analogRead(brightnessPot),0,1023,0,255);
  
  while(digitalRead(button)==LOW){
    lcd.setCursor(0,0);lcd.print("  Homebrew PCB  ");
    lcd.setCursor(0,1);lcd.print("  Exposure Box  ");
    for (int i=0; i <= brightnessPWM; i++){ 
      analogWrite(LEDs,i);
      delay(glowDelay);
    }
    lcd.clear();
    lcd.setCursor(0, 0);lcd.print("Firmware Version");delay(500);
    lcd.setCursor(3, 1);lcd.print("Evtech v.1");delay(500);
    break;
  }
  lcd.clear();
}

void loop(){
  duration = analogRead(durationPot);
  durationSec = map(duration,0,1023,0, maxTime);

  brightness = analogRead(brightnessPot);
  brightnessPercent = map(brightness,0,1023,0,100);
  brightnessPWM = map(brightness,0,1023,0,255);
  
  analogWrite(LEDs,brightnessPWM);
      
  minutes = durationSec/60;
  seconds = durationSec%60;
  
  lcd.setCursor(0,0);lcd.print("Duration: ");
  sprintf(timeMinSec,"%0.2d:%0.2d",minutes,seconds);
  lcd.setCursor(11,0);lcd.print(timeMinSec); 
  
  if(brightnessPercent<100){  
    lcd.setCursor(14, 1);
    lcd.print(" ");
  }
      
  lcd.setCursor(0, 1);
  lcd.print("Brightness:     ");
  lcd.setCursor(12, 1);
  lcd.print(brightnessPercent);
  lcd.setCursor(15, 1);
  lcd.print("%");  
  
  if (digitalRead(button)==HIGH){
    durationSecSelect = durationSec;
    durationSecFloat = durationSec;
    
    lcd.clear();
    lcd.setCursor(0,0);lcd.print("    STARTING    ");
    lcd.setCursor(0, 1);
    for (int i=0; i <= 15; i++){ 
      lcd.print((char) 255);delay(35);
      tone(buzzer, buzzerFreq*2, 50);
    }
    lcd.print((char) 255);
    delay(300);
    analogWrite(LEDs,brightnessPWM);    
 
    lcd.clear();  
    lcd.setCursor(0, 0);
    lcd.print("Time Left: ");

    while(exposureLoop==0){

      timeLeftBarPercent = 100*(durationSecFloat/durationSecSelect);
      
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval){
        previousMillis = currentMillis;

        minutes = durationSec/60;
        seconds = durationSec%60;

        sprintf(timeMinSec,"%0.2d:%0.2d", minutes, seconds);
        lcd.setCursor(11, 0);
        lcd.print(timeMinSec); 
         
        if(timeLeftBarPercent<2)        {for (int v=0; v <= 15; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}}      
        else if(timeLeftBarPercent<6.25){for(int v=0; v <= 14; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}} 
        else if(timeLeftBarPercent<12.5){for(int v=0; v <= 13; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}}  
        else if(timeLeftBarPercent<18.75){for(int v=0; v <= 12; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}} 
        else if(timeLeftBarPercent<25)   {for (int v=0; v <= 11; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}} 
        else if(timeLeftBarPercent<31.25){for(int v=0; v <= 10; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}}  
        else if(timeLeftBarPercent<37.5) {for(int v=0; v <= 9; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}}   
        else if(timeLeftBarPercent<43.75){for(int v=0; v <= 8; v++){  lcd.setCursor(v, 1);lcd.print((char) 255);}}   
        else if(timeLeftBarPercent<50)   {for(int v=0; v <= 7; v++){  lcd.setCursor(v, 1);lcd.print((char) 255);}}      
        else if(timeLeftBarPercent<56.25){for(int v=0; v <= 6; v++){  lcd.setCursor(v, 1);lcd.print((char) 255);}}   
        else if(timeLeftBarPercent<62.5) {for(int v=0; v <= 5; v++){  lcd.setCursor(v, 1);lcd.print((char) 255);}}   
        else if(timeLeftBarPercent<68.75){for (int v=0; v <= 4; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}}    
        else if(timeLeftBarPercent<75)   {for (int v=0; v <= 3; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}}   
        else if(timeLeftBarPercent<81.25){for (int v=0; v <= 2; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}} 
        else if(timeLeftBarPercent<87.5) {for (int v=0; v <= 1; v++){lcd.setCursor(v, 1);lcd.print((char) 255);}}   
        else if(timeLeftBarPercent<93.75){lcd.setCursor(0, 1);lcd.print((char) 255);}
        durationSec--;
        durationSecFloat--;
      }

      if(durationSec<=0){
        digitalWrite(LEDs,LOW);          
        tone(buzzer,3000,500);         
        lcd.setCursor(0,0);lcd.print("    Exposure    ");
        lcd.setCursor(0,1);lcd.print("    Finished    ");
        delay(500);lcd.clear();
        lcd.setCursor(0,0);lcd.print("  Press Button  ");
        lcd.setCursor(0,1);lcd.print("    To Start    ");
        delay(500);
        while(digitalRead(button)==LOW){}  
        for (int i=0; i <= 255; i++){      
          analogWrite(LEDs,i);
          delay(glowDelay);
        }
        lcd.clear(); 
        exposureLoop=1;
      }
      else if(digitalRead(button)==HIGH){
        digitalWrite(LEDs,LOW);
        tone(buzzer,3000,500);
        lcd.setCursor(0,0);lcd.print("   Cancelling   ");
        lcd.setCursor(0,1);lcd.print("    Process!    ");
        delay(750);       
        lcd.setCursor(0,0);lcd.print("    Exposure    ");
        lcd.setCursor(0,1);lcd.print("    Finished    ");
        delay(500);lcd.clear();
        lcd.setCursor(0,0);lcd.print("  Press Button  ");
        lcd.setCursor(0,1);lcd.print("    To Start    ");
        
        while(digitalRead(button)==LOW){}
        for (int i=0;i<=brightnessPWM;i++){ 
          analogWrite(LEDs,i);
          delay(glowDelay);
        }
        lcd.clear();  
        exposureLoop=1;
      }
    } 
    exposureLoop=0; 
  } 
  delay(20);
}

// Homebrew PCB Exposure Box Using An Old Printer Scanner
// Evtech | @earldanielph
// beacons.page/evtech
