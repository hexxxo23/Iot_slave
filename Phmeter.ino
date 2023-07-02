#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A2
GravityTDS gravityTds;
float tdsValue = 0;
float temperature = 25; 

const int ph_pin =A0;
float Po = 0;
float PH_step;
int phValue;
float volt;
double PhVolt;

SoftwareSerial ph(6,7) ;//RX,TX
SoftwareSerial turbidity(4,5) ;//rx,tx
SoftwareSerial tds(2,3) ;

const int turbiditysensorPin = A1;
int read_adc;
int ntu = 0;

// ph cal
float PH4 = 3.66;
float PH7 = 3.31;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ph.begin(9600);
  turbidity.begin(9600);
  tds.begin(9600);

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);       //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin(); 

  pinMode(ph_pin, INPUT);
  // pinMode(turbiditysensorPin, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  phRead();
  turbidityRead();
  readTds();
  Serial.println("++++++++++++++");
  delay(51000);
}


void phRead(){
  phValue = analogRead(ph_pin);
  volt = phValue*(5/1023.0);
  Serial.println(volt);
  // ph.println(volt);
  PH_step = (PH4-PH7) /3;
  Po = 7.00+((PH7-volt)/PH_step);
  Serial.print("PH value : "); Serial.println(Po,2);
  ph.println(Po,2);
}
void turbidityRead() {
  
  read_adc = analogRead(turbiditysensorPin);
  Serial.println(read_adc);
  // if(read_adc>1310){
  //   read_adc = 1220;
  // }
  ntu = map(read_adc, 0, 971, 250, 0);

  Serial.print("Turbidity: ");Serial.print(ntu);Serial.println(" NTU");
  turbidity.print(ntu);
  
  if (ntu <= 35) {
    Serial.println("( Normal )");
  } else if (35 < ntu && ntu < 60) {
    Serial.println("( Agak Keruh )");
  } else if (60 <= ntu && ntu < 100) {
    Serial.println("( Keruh )");
  } else if (ntu > 100) {
    Serial.println("( Sangat Keruh )");
  }
}

void readTds() {
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();                     //sample and calculate
  tdsValue = gravityTds.getTdsValue();     // then get the value

  Serial.print("TDS: ");Serial.print(tdsValue, 0);Serial.println(" ppm");
  Serial.print("temperature: ");Serial.println(temperature);
  tds.print(tdsValue, 0);
  // sensor.print("temperature: ");sensor.println(temperature);
  
  // return tdsValue;
}
