//RS-485 Modbus Slave (Arduino UNO)
//Circuit Digest


#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define SMOKEPIN1 A0
#define SMOKEPIN2 A1

#define RADARPIN1 A2
#define RADARPIN2 A3

#define HEATPIN A4
#define DOORPIN A5

#define alarm1 5
#define alarm2 6

#define relaysmoke1 3 //asalnya a6 diganti d3
#define relaysmoke2 13 //asalnya a7 diganti d13

#define indicator 7

#include<ModbusRtu.h>       //Library for using Modbus in Arduino
#include "DHT.h"

DHT dht(DHTPIN, DHTTYPE);
Modbus bus;                          //Define Object bus for class modbus
uint16_t modbus_array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; //Array initilized with three 0 values
int smoke, heat, door, radar;
float h, t;



void setup()
{
  pinMode(alarm1, OUTPUT);
  pinMode(indicator, OUTPUT);
  pinMode(SMOKEPIN1, INPUT);
  pinMode(HEATPIN, INPUT);
  pinMode(DOORPIN, INPUT);
  pinMode(RADARPIN1, INPUT);

  digitalWrite(HEATPIN, LOW);
  digitalWrite(DOORPIN, LOW);
  digitalWrite(SMOKEPIN1, LOW);
  digitalWrite(RADARPIN1, LOW);

  digitalWrite(relaysmoke1, LOW);
  digitalWrite(relaysmoke2, LOW);

  bus = Modbus(4, 1, 4);          //Modbus slave ID as 1 and 1 connected via RS-485 and 4 connected to DE & RE pin of RS-485 Module
  bus.begin(9600);                //Modbus slave baudrate at 9600
  dht.begin();

}

void loop()
{
  smoke = digitalRead(SMOKEPIN1);
  heat = digitalRead(HEATPIN);
  door = digitalRead(DOORPIN);
  radar = digitalRead(RADARPIN1);

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
  modbus_array[2] = hum;
  modbus_array[3] = tem;
  modbus_array[4] = door; //Door
  modbus_array[5] = radar; //Smoke
  modbus_array[6] = heat;
  modbus_array[7] = smoke;

  bus.poll(modbus_array, sizeof(modbus_array) / sizeof(modbus_array[0]));    //Used to receive or write value from Master

  if (modbus_array[1] == 1 || heat == 1 || smoke == 1)  //Depends upon value in modubus_array[0] written by Master Modbus
  {
    //Serial.println("masuk");
    digitalWrite(alarm1, HIGH);

  }
  else if (modbus_array[1] == 0 || heat == 0 || smoke == 0)
  {
    //Serial.println("kelar");
    digitalWrite(alarm1, LOW);

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
