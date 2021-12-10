//RS-485 Modbus Slave (Arduino UNO)
//Circuit Digest


#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT21   // DHT 21 (AM2301)

#include<ModbusRtu.h>       //Library for using Modbus in Arduino
#include "DHT.h"

//#include<LiquidCrystal.h>   //Library for using 16x2 LCD display
//#include <Servo.h>          //Library for using Servo Motor

#define led1 LED_BUILTIN              //Define as 2 led1 
//#define led2 5              //Define as 5 led2

//LiquidCrystal lcd(8,9,10,11,12,13);   //initizlize lcd object with pins (RS,E,D4,D5,D6,D7) for class liquid crystal
//Servo servo;                          //Initilize servo object for class Servo

DHT dht(DHTPIN, DHTTYPE);
Modbus bus;                          //Define Object bus for class modbus 
uint16_t modbus_array[] = {0,0,0,0,0,0};    //Array initilized with three 0 values


                      
void setup()
{
//  lcd.begin(16,2);                //Lcd set in 16x2 mode
//  lcd.print("RS-485 Modbus");     //Welcome Message
//  lcd.setCursor(0,1); 
//  lcd.print("Arduino Slave");
//  delay(5000);
//  lcd.clear();
  
  pinMode(led1,OUTPUT);           //Led1 set as OUTPUT
//  pinMode(led2,OUTPUT);           //Led2 set as OUTPUT
//  servo.attach(6);                //Servo PWM pin 6
  bus = Modbus(2,1,4);            //Modbus slave ID as 1 and 1 connected via RS-485 and 4 connected to DE & RE pin of RS-485 Module 
  bus.begin(9600);                //Modbus slave baudrate at 9600
  dht.begin();

}

void loop()
{
  float h = dht.readHumidity()*100;
    // Read temperature as Celsius (the default)
  float t = dht.readTemperature()*100;

  int hum=h;
  int tem=t;
  modbus_array[2]=hum;
  modbus_array[3]=tem;
  modbus_array[4]=1; //status PLN
  modbus_array[5]=0; //status Genset
  
  bus.poll(modbus_array,sizeof(modbus_array)/sizeof(modbus_array[0]));       //Used to receive or write value from Master 
  
  if (modbus_array[1] == 0)    //Depends upon value in modubus_array[0] written by Master Modbus
  {
    digitalWrite(led1,LOW);    //LED OFF if 0
//    lcd.setCursor(0,0);
//    lcd.print("L1:OFF");
  }
  else
  {
     digitalWrite(led1,HIGH);  //LED ON if value other than 0
//     lcd.setCursor(0,0);
//     lcd.print("L1:ON");        
  } 

}
