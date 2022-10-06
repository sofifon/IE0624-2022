#include <PCD8544.h>
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT22
PCD8544 lcd;
DHT dht(2, DHTTYPE);
int RED=13;
int BLUE=12;
int pot=0;
int potPin = A0;

void setup() {
   lcd.begin(84, 48);
   dht.begin();
  Serial.begin(9600);
  Serial.println("Here we go!");
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);   
}
void loop() {
    lcd.clear();
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();         //Reading the temperature in degrees
    float senal = 1;
    pot = analogRead(potPin)*12/1000 +30;    
    if (isnan(hum) || isnan(temp) || isnan(pot)) {      //Checking if the arduino have recieved the values or not
     lcd.println("Failed to read from DHT sensor!");
      Serial.println("Failed");
         
     return;
}
  
  lcd.setCursor(0, 0);
  lcd.print("T_o: ");
  lcd.print(pot);
  Serial.println(pot);
  lcd.print(" *C");
  lcd.setCursor(0,1);
  lcd.print("Senal: ");
  lcd.print(senal);
  Serial.println(senal);
  lcd.print(" H");  
  lcd.setCursor(0, 2);
  lcd.print("T_s: ");
  Serial.println(temp);
  lcd.print(temp);
  lcd.print(" *C "); 
  lcd.setCursor(0,3);   
  lcd.print("H_s: ");
  Serial.println(hum);
  lcd.print(hum);
  lcd.print(" %\t");

  if (temp<30){
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, HIGH);        
  }else if (temp>42){
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
  }else {
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
  }

  
  delay(2000);
 }