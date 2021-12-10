
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
#define relaysmoke1 3 //asalnya a6 diganti d3
#define relaysmoke2 13 //asalnya a7 diganti d13

#define indicator 7

//LiquidCrystal lcd(8,9,10,11,12,13);   //initizlize lcd object with pins (RS,E,D4,D5,D6,D7) for class liquid crystal
//Servo servo;                          //Initilize servo object for class Servo

DHT dht(DHTPIN, DHTTYPE);
Modbus bus;                          //Define Object bus for class modbus
uint16_t modbus_array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; //Array initilized with three 0 values

int smoke1,smoke2, heat, pln, genset;
float h, t;


void setup()
{
  //  lcd.begin(16,2);                //Lcd set in 16x2 mode
  //  lcd.print("RS-485 Modbus");     //Welcome Message
  //  lcd.setCursor(0,1);
  //  lcd.print("Arduino Slave");
  //  delay(5000);
  //  lcd.clear();

  pinMode(alarm1, OUTPUT);
  pinMode(alarm2, OUTPUT);
  pinMode(indicator, OUTPUT); //alarm set as OUTPUT


  pinMode(SMOKEPIN1, INPUT);
  pinMode(SMOKEPIN2, INPUT);
  pinMode(HEATPIN, INPUT);
  pinMode(DOORPIN1, INPUT);
  pinMode(PLNPIN, INPUT);
  pinMode(GENPIN, INPUT);
  bus = Modbus(2, 1, 4);          //Modbus slave ID as 1 and 1 connected via RS-485 and 4 connected to DE & RE pin of RS-485 Module
  bus.begin(9600);                //Modbus slave baudrate at 9600
  dht.begin();

  digitalWrite(HEATPIN, LOW);
  digitalWrite(PLNPIN, LOW);
  digitalWrite(SMOKEPIN1, LOW);
  digitalWrite(SMOKEPIN2, LOW);

  digitalWrite(GENPIN, LOW);
  digitalWrite(relaysmoke1, LOW);
  digitalWrite(relaysmoke2, LOW);



}

void loop()
{
  smoke1 = digitalRead(SMOKEPIN1);
  smoke2 = digitalRead(SMOKEPIN2);
  heat = digitalRead(HEATPIN);
  pln = digitalRead(PLNPIN);
  genset = digitalRead(GENPIN);
  //int door=digitalRead(DOORPIN);

  h = dht.readHumidity() * 100;
  // Read temperature as Celsius (the default)
  t = dht.readTemperature() * 100;

  if (isnan(t)) {
    digitalWrite(indicator, HIGH);
  }
  else {
    digitalWrite(indicator, LOW);
  }


  int hum = h;
  int tem = t;
  //uint16_t modbus_array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; //Array initilized with three 0 values
  //{null, alarm status, hum, temp, pln, genset, heat, smoke, smokecut

  modbus_array[2] = hum;
  modbus_array[3] = tem;
  modbus_array[4] = pln; //Door
  modbus_array[5] = genset; //Smoke
  modbus_array[6] = heat;
  modbus_array[7] = smoke1;

  bus.poll(modbus_array, sizeof(modbus_array) / sizeof(modbus_array[0]));    //Used to receive or write value from Master

  if (modbus_array[1] == 1 || heat == 1 || smoke1 == 1)  //Depends upon value in modubus_array[0] written by Master Modbus
  {
    digitalWrite(alarm1, HIGH);   //LED OFF if 0
    digitalWrite(alarm2, HIGH);
    //digitalWrite(indicator, HIGH);
    //    lcd.setCursor(0,0);
    //    lcd.print("L1:OFF");
  }
  else if (modbus_array[1] == 0 || heat == 0 || smoke1 == 0)
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
