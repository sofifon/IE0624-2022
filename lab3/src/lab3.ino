#include <PCD8544.h>
#include "ArduPID.h"
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT22
PCD8544 lcd;
DHT dht(2, DHTTYPE);
int RED=13;
int BLUE=12;
float pot=0;
float pot1=0;
int potPin = A0;
int npnTrans = 9;

ArduPID myController;
double input;
double output;

// Arbitrary setpoint and gains - adjust these as fit for your project:
double setpoint = 35;
double p = 10;
double i = 1;
double d = 0.5;

void setup() {
   lcd.begin(84, 48);
   dht.begin();
  Serial.begin(9600);
  Serial.println("Here we go!");
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(npnTrans, OUTPUT);   

  myController.begin(&input, &output, &setpoint, p, i, d);

  // myController.reverse()               // Uncomment if controller output is "reversed"
  // myController.setSampleTime(10);      // OPTIONAL - will ensure at least 10ms have past between successful compute() calls
  myController.setOutputLimits(0, 255);
  myController.setBias(255.0 / 2.0);
  myController.setWindUpLimits(-5, 5); // Groth bounds for the integral term to prevent integral wind-up
  
  myController.start();
}
void loop() {
    lcd.clear();
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();         //Reading the temperature in degrees
    float senal = 1;
    pot = analogRead(potPin)*12/1000 +30;
    pot1= (pot-30)/12*255;        
    if (isnan(hum) || isnan(temp) || isnan(pot)) {      //Checking if the arduino have recieved the values or not
     lcd.println("Failed to read from DHT sensor!");
      Serial.println("Failed");
         
     return;
    }
  input=temp;
  if (setpoint != pot){
    setpoint=pot;
    myController.begin(&input, &output, &setpoint, p, i, d);
    myController.setOutputLimits(0, 255);
    myController.setBias(255.0 / 2.0);
    myController.setWindUpLimits(-5, 5); // Groth bounds for the integral term to prevent integral wind-up
  
    myController.start();        
  }
  
  myController.compute();

  senal=output/255*80;
  analogWrite(npnTrans, output);
  lcd.setCursor(0, 0);
  lcd.print("T_o: ");
  lcd.print(pot);
  lcd.print(" *C");
  lcd.setCursor(0,1);
  lcd.print("Senal: ");
  lcd.print(senal);
  lcd.print("*C");  
  lcd.setCursor(0, 2);
  lcd.print("T_s: ");
  lcd.print(temp);
  lcd.print(" *C "); 
  lcd.setCursor(0,3);   
  lcd.print("H_s: ");
  lcd.print(hum);
  lcd.print(" %\t");

  Serial.println(temp);
  Serial.println(hum);
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