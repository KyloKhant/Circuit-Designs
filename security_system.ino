#include <LiquidCrystal_I2C.h>
#include <Adafruit_LEDBackpack.h>
//Initializing LCD
LiquidCrystal_I2C lcd(0x27,16,2);
//Iniializing Clock
Adafruit_7segment clock= Adafruit_7segment();
//Setting pins
const int trigPin = 3;
const int echoPin = 2;
const int led1= 5;
const int led2= 6;
const int buzzer= 9;
const int IR= 11;

float duration, distance;


void setup() {
  //Ultrasonic distance sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //Infra-red
  pinMode(IR,INPUT);
  //LED
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  //LCD
  lcd.init();
  lcd.backlight();
  //Timer Clock
  clock.begin(112);
  //Serial Monitor
  Serial.begin(9600);
}

void loop() {
 //Loop to run from 00 hrs to 23 hrs
  for(int i=000;i<=2300;i+=100){
    clock.println(i);//Display time on clock
    clock.writeDisplay();
    //Before 9am & after 8pm(exclusive)
    if (i<900 || i>2000){
      //Only operates Infra-red
      int infra_state= digitalRead(IR);
      if (infra_state==HIGH){//motion detected 
        BUZZER(buzzer);
        LED(led1,led2);
        LCD();
      }
      else{
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        noTone(buzzer);
        lcd.clear();
        }
     
    }
    //Between 9am and 8pm (inclusive)
   else if (i>=900 && i<=2000){
    //Only operates ultrasonic sound sensor
     // Turn off, on and off the trigger pin 
     // to release the ultrasound and recapture
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
	// record the echo time for ultrasound to return 
    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2; //Distance= speed of sound*echo/2 
    Serial.print("Distance: ");//Display on serial
    Serial.println(distance);
    delay(100);
	//trigger alarm base on distance
    if (distance<=150){
      BUZZER(buzzer);
      LED(led1,led2);
      LCD(); 

    }
    else{
      digitalWrite(led1,LOW);
      digitalWrite(led2,LOW);
      noTone(buzzer);
      lcd.clear();
    }
    
	
  }
 delay(1000);

}

}
//LED display pattern
void LED (int led1,int led2){
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  delay(100);
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  
}
//Buzzer buzzing pattern
void BUZZER(int buzzer){
     tone(buzzer,800,100);
     tone(buzzer,1200,200);
     delay(50);  
}
//LCD display 
void LCD(){
    lcd.setCursor(3,0);
    lcd.print("!!WARNING!!");
    lcd.setCursor(0,1);
    lcd.print("*Intruder Alert*");
    delay(400); 
  	lcd.clear(); 
}