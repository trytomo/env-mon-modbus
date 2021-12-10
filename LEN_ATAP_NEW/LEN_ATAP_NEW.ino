//RS-485 Modbus Slave (Arduino UNO)
//Circuit Digest


#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT21   // DHT 21 (AM2301)

//#define SMOKEPIN A7
//#define HEATPIN A5
#define DOORPIN A5
#define RADARPIN A2
//#define alarm1 5
//#define alarm2 6
#define indicator 7

#include<ModbusRtu.h>       //Library for using Modbus in Arduino
#include "DHT.h"

DHT dht(DHTPIN, DHTTYPE);
Modbus bus;                          //Define Object bus for class modbus
uint16_t modbus_array[] = {0, 0, 0, 0, 0, 0, 0}; //Array initilized with three 0 values

int door, hum, tem, radar;
float h, t;

void setup()
{


  //  pinMode(alarm1, OUTPUT);          //Led1 set as OUTPUT
  //  pinMode(alarm2, OUTPUT);
  pinMode(indicator, OUTPUT);
  //  pinMode(SMOKEPIN, INPUT);
  //  pinMode(HEATPIN, INPUT);
  pinMode(DOORPIN, INPUT);
  pinMode(RADARPIN, INPUT);
  //
  //  digitalWrite(HEATPIN, LOW);
  digitalWrite(RADARPIN, LOW);
  //  digitalWrite(SMOKEPIN, LOW);
  digitalWrite(DOORPIN, LOW);

  bus = Modbus(5, 1, 4);          //Modbus slave ID as 1 and 1 connected via RS-485 and 4 connected to DE & RE pin of RS-485 Module
  bus.begin(9600);                //Modbus slave baudrate at 9600
  dht.begin();
  //Serial.begin(9600);

}

void loop()
{
  //  smoke = analogRead(SMOKEPIN);
  //  if (smoke >= 512) {
  //    smoke = 1;
  //  } else {
  //    smoke = 0;
  //  }

  //  heat = digitalRead(HEATPIN);

  door = digitalRead(DOORPIN);
  radar = digitalRead(RADARPIN);
  h = dht.readHumidity() * 100;
  // Read temperature as Celsius (the default)
  t = dht.readTemperature() * 100;


  if (isnan(t)) {
    digitalWrite(indicator, HIGH);
  }
  else {
    digitalWrite(indicator, LOW);
  }

  hum = h;
  tem = t;
  modbus_array[2] = hum;
  modbus_array[3] = tem;
  modbus_array[4] = door; //Door
  modbus_array[5] = radar; //Smoke
  modbus_array[6] = 0;

  //  Serial.println(hum);
  //  Serial.println(tem);
  //  Serial.println(door);

  bus.poll(modbus_array, sizeof(modbus_array) / sizeof(modbus_array[0]));    //Used to receive or write value from Master
  //
  //  if (modbus_array[1] == 1)  //Depends upon value in modubus_array[0] written by Master Modbus
  //  {
  //    //Serial.println("masuk");
  //    digitalWrite(alarm1, HIGH);   //LED OFF if 0
  //    digitalWrite(alarm2, HIGH);
  //    //digitalWrite(indicator, HIGH);
  //    //    lcd.setCursor(0,0);
  //    //    lcd.print("L1:OFF");
  //  }
  //  else if (modbus_array[1] == 0)
  //  {
  //    //Serial.println("kelar");
  //    digitalWrite(alarm1, LOW);
  //    digitalWrite(alarm2, LOW);//LED ON if value other than 0
  //    //digitalWrite(indicator, LOW);
  //    //     lcd.setCursor(0,0);
  //    //     lcd.print("L1:ON");
  //  }
}
