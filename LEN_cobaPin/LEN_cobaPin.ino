//RS-485 Modbus Slave (Arduino UNO)
//Circuit Digest


#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define SMOKEPIN A7
#define HEATPIN A5
#define DOORPIN1 50
#define DOORPIN2 A2
#define PLNPIN A1
#define GENPIN A0

#include<ModbusRtu.h>       //Library for using Modbus in Arduino
#include "DHT.h"

//#include<LiquidCrystal.h>   //Library for using 16x2 LCD display
//#include <Servo.h>          //Library for using Servo Motor

#define alarm1 5              //Define as 2 led1 //mosfet rada ngaco
//#define alarm2 6 
//#define led2 5              //Define as 5 led2

#define indicator 7

//LiquidCrystal lcd(8,9,10,11,12,13);   //initizlize lcd object with pins (RS,E,D4,D5,D6,D7) for class liquid crystal
//Servo servo;                          //Initilize servo object for class Servo

DHT dht(DHTPIN, DHTTYPE);
Modbus bus;                          //Define Object bus for class modbus
uint16_t modbus_array[] = {0, 0, 0, 0, 0, 0, 0, 0}; //Array initilized with three 0 values

int smoke, heat, pln, genset, door1, door2;

void setup()
{
  //  lcd.begin(16,2);                //Lcd set in 16x2 mode
  //  lcd.print("RS-485 Modbus");     //Welcome Message
  //  lcd.setCursor(0,1);
  //  lcd.print("Arduino Slave");
  //  delay(5000);
  //  lcd.clear();

  pinMode(alarm1, OUTPUT);
  //pinMode(alarm2, OUTPUT);
  pinMode(indicator, OUTPUT); //alarm set as OUTPUT

  
  pinMode(SMOKEPIN, INPUT);
  pinMode(HEATPIN, INPUT);
  pinMode(DOORPIN1, INPUT);
  pinMode(DOORPIN2, INPUT);
  pinMode(PLNPIN, INPUT);
  pinMode(GENPIN, INPUT);
  //bus = Modbus(1, 1, 4);          //Modbus slave ID as 1 and 1 connected via RS-485 and 4 connected to DE & RE pin of RS-485 Module
  //bus.begin(9600);                //Modbus slave baudrate at 9600
  dht.begin();

  digitalWrite(HEATPIN, LOW);
  digitalWrite(PLNPIN, LOW);
  digitalWrite(GENPIN,LOW);
  digitalWrite(SMOKEPIN, LOW);
  digitalWrite(DOORPIN1,LOW);
  digitalWrite(DOORPIN2, LOW);
  Serial.begin(9600);

}

void loop()
{
  smoke = analogRead(SMOKEPIN);
  if (smoke >= 512){
    smoke= 1;
  } else { smoke =0;}
  
  heat = digitalRead(HEATPIN);
  //pln = digitalRead(PLNPIN);
  door1=digitalRead(DOORPIN1);
  door2=digitalRead(DOORPIN2);
  Serial.println("Data Read");
  Serial.println(door1);
  Serial.println(door2);
  //Serial.println(smoke);
  int genset = digitalRead(GENPIN);
  //int door=digitalRead(DOORPIN);
  float h = dht.readHumidity() * 100;
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature() * 100;
  Serial.println(t);
  Serial.println(h);

  if (isnan(t)) {
    digitalWrite(indicator, HIGH);
    }
  else{
    digitalWrite(indicator, LOW);
  }
  

  int hum = h;
  int tem = t;
  modbus_array[2] = hum;
  modbus_array[3] = tem;
  modbus_array[4] = pln; //Door
  modbus_array[5] = genset; //Smoke
  modbus_array[6] = heat;
  modbus_array[7] = smoke;

//  int i;
//  Serial.println("data =");
//  for (i=0; i<=7;i++){
//    Serial.println(modbus_array[i]);
//  }
  //bus.poll(modbus_array, sizeof(modbus_array) / sizeof(modbus_array[0]));    //Used to receive or write value from Master

   if (modbus_array[1] == 1 || heat == 1 || smoke==1)    //Depends upon value in modubus_array[0] written by Master Modbus
  {
    Serial.println("masuk");
    digitalWrite(alarm1, HIGH);   //LED OFF if 0
    //digitalWrite(alarm2, HIGH);
    //digitalWrite(indicator, HIGH);
    //    lcd.setCursor(0,0);
    //    lcd.print("L1:OFF");
  }
  else if (modbus_array[1]==0 || heat == 0 || smoke==0)
  {
    Serial.println("kelar");
    digitalWrite(alarm1, LOW);
    //digitalWrite(alarm2, LOW);//LED ON if value other than 0
    //digitalWrite(indicator, LOW);
    //     lcd.setCursor(0,0);
    //     lcd.print("L1:ON");
  }

  delay(1000);

}
