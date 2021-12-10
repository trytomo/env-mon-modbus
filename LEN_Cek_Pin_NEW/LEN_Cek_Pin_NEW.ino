//// BAGIAN INPUT
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT21   // DHT 21 (AM2301)


#define SMOKEPIN1 A0
#define SMOKEPIN2 A1
#define PLNPIN A2
#define GENPIN A3
#define HEATPIN A4
#define DOORPIN1 A5


#include<ModbusRtu.h>       //Library for using Modbus in Arduino
#include "DHT.h"

// BAGIAN OUTPUT

#define alarm1 5              //Define as 2 led1 //mosfet rada ngaco
#define alarm2 6
#define relaysmoke1 3
#define relaysmoke2 13

#define indicator 7


DHT dht(DHTPIN, DHTTYPE);
Modbus bus;                          //Define Object bus for class modbus
uint16_t modbus_array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; //Array initilized with three 0 values


void setup() {
  // put your setup code here, to run once:
  pinMode(alarm1, OUTPUT);
  pinMode(alarm2, OUTPUT);
  pinMode(indicator, OUTPUT); //alarm set as OUTPUT
  pinMode(relaysmoke1, OUTPUT);
  pinMode(relaysmoke2, OUTPUT);

  pinMode(SMOKEPIN1, INPUT);
  pinMode(SMOKEPIN2, INPUT);
  pinMode(PLNPIN, INPUT);
  pinMode(GENPIN, INPUT);
  pinMode(HEATPIN, INPUT);
  pinMode(DOORPIN1, INPUT);


  digitalWrite(SMOKEPIN1, LOW);
  digitalWrite(SMOKEPIN2, LOW);
  digitalWrite(PLNPIN, LOW);
  digitalWrite(GENPIN, LOW);
  digitalWrite(HEATPIN, LOW);
  digitalWrite(DOORPIN1, LOW);

  bus = Modbus(3, 1, 4);          //Modbus slave ID as 1 and 1 connected via RS-485 and 4 connected to DE & RE pin of RS-485 Module
  bus.begin(9600);
  dht.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  //  digitalWrite(alarm1, HIGH);
  //  digitalWrite(alarm2, HIGH);
  //  digitalWrite(indicator, HIGH);
  //  //digitalWrite(relaysmoke1, 255);
  //  //digitalWrite(relaysmoke2, 255);
  //  delay(1000);
  //  digitalWrite(alarm1, LOW);
  //  digitalWrite(alarm2, LOW);
  //  digitalWrite(indicator, LOW);
  //  analogWrite(relaysmoke1, 0);
  //  analogWrite(relaysmoke2, 0);
  //  delay(1000);


  float h = dht.readHumidity() * 100;
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature() * 100;


  if (isnan(t)) {
    digitalWrite(indicator, HIGH);
  }
  else {
    digitalWrite(indicator, LOW);
  }

  int hum = h;
  int tem = t;

  //  Serial.print("Humidity  ");
  //  Serial.println(h);
  //  Serial.print("Temp  ");
  //  Serial.println(t);

  int smoke = digitalRead(SMOKEPIN1);
  //  Serial.print("Smoke :"); Serial.println(smoke);
  int smoke2 = digitalRead(SMOKEPIN2);
  //  Serial.print("Smoke 2 :"); Serial.println(smoke2);
  int heat = digitalRead(HEATPIN);
  //  Serial.print("Heat :"); Serial.println(heat);
  int pln = digitalRead(PLNPIN);
  //  Serial.print("PLN :"); Serial.println(pln);
  int gen = digitalRead(GENPIN);
  int door = digitalRead(DOORPIN1);

  //  Serial.print("Gen :"); Serial.println(gen);

  modbus_array[2] = h;
  modbus_array[3] = t;
  modbus_array[4] = door; //Door
  modbus_array[5] = gen; //Smoke
  modbus_array[6] = heat;
  modbus_array[7] = smoke;

  bus.poll(modbus_array, sizeof(modbus_array) / sizeof(modbus_array[0]));    //Used to receive or write value from Master

  if (modbus_array[1] == 1 || heat == 1 || smoke == 1)  //Depends upon value in modubus_array[0] written by Master Modbus
  {
    digitalWrite(alarm1, HIGH);   //LED OFF if 0
    digitalWrite(alarm2, HIGH);
    //digitalWrite(indicator, HIGH);
    //    lcd.setCursor(0,0);
    //    lcd.print("L1:OFF");
  }
  else if (modbus_array[1] == 0 || heat == 0 || smoke == 0)
  {
    digitalWrite(alarm1, LOW);
    digitalWrite(alarm2, LOW);
  }

  if (modbus_array[8] == 1)
  {
    digitalWrite(relaysmoke1, HIGH);
    digitalWrite(relaysmoke2, HIGH);
  }
  else if (modbus_array[8] == 0)
  {
    digitalWrite(relaysmoke1, LOW);
    digitalWrite(relaysmoke2, LOW);

  }


}
