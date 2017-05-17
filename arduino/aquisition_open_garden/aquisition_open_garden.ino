/*  
 *  OpenGarden sensor platform for Arduino from Cooking-hacks.
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           2.3
 *  Design:            David Gascón 
 *  Implementation:    Victor Boria, Luis Martin & Jorge Casanova
 *  Realtime : Gllm
 */

#include <OpenGarden.h>
#include <Wire.h> 

DateTime myTime;

int flag = 0; // auxiliar variable
#define millis_entre_lecture 1000
int millis_old =0;


#define calibration_point_4 2246  //Write here your measured value in mV of pH 4
#define calibration_point_7 2080  //Write here your measured value in mV of pH 7
#define calibration_point_10 1894 //Write here your measured value in mV of pH 10

#define point_1_cond 40000   // Write here your EC calibration value of the solution 1 in µS/cm
#define point_1_cal 40       // Write here your EC value measured in resistance with solution 1
#define point_2_cond 10500   // Write here your EC calibration value of the solution 2 in µS/cm
#define point_2_cal 120      // Write here your EC value measured in resistance with solution 2

//Power strip codes
unsigned long on1 = 1597145202;
unsigned long off1 = 1597145204;
unsigned long on2 = 1597145220;
unsigned long off2 = 1597145222;
unsigned long on3 = 1597145112;
unsigned long off3 = 1597145114;
unsigned long on4 = 1597144950;
unsigned long off4 = 1597144952;
unsigned long on5 = 1597144464;
unsigned long off5 = 1597144466;

int airTemperature;
int airHumidity;
int waterTemperature ;
int luminosity; 
float mvpH;
int pH; 
float resistanceEC;
int EC;

void setup() 
{
  Serial.begin(9600);
  OpenGarden.initSensors(); //Initialize sensors power    
  OpenGarden.sensorPowerON();//Turn On the sensors
  delay(500);
  OpenGarden.initRTC();
  OpenGarden.initIrrigation(1); //Initialize irrigation 1
  OpenGarden.initIrrigation(2); //Initialize irrigation 2
  OpenGarden.initIrrigation(3); //Initialize irrigation 3
  OpenGarden.calibratepH(calibration_point_4,calibration_point_7,calibration_point_10);
  OpenGarden.calibrateEC(point_1_cond,point_1_cal,point_2_cond,point_2_cal);
  delay(500);
}


void loop() 
{
  myTime = OpenGarden.getTime();
  int millis_actuel = millis()-millis_old;
  // lire et envoyer a chaque temps entre le
  if ( millis_actuel>millis_entre_lecture)
  { 
    get_sensors();
    send_sensors();
    // debug_sensors();
    millis_old =  millis();
  } 

}

void get_sensors()
{
  //Read DHT22
  //float f_air_temperature = OpenGarden.readAirTemperature()*100;
  airTemperature = OpenGarden.readAirTemperature()*100; 
  airHumidity = OpenGarden.readAirHumidity()*100; 

  //Read DS18B20
  waterTemperature = OpenGarden.readSoilTemperature()*100;

  //Read LDR
  luminosity = OpenGarden.readLuminosity()*100; 

  //Read the pH sensor
  mvpH = OpenGarden.readpH(); //Value in mV of pH
  pH = OpenGarden.pHConversion(mvpH)*100; //Calculate pH value


  //Read the conductivity sensor in µS/cm
  resistanceEC = OpenGarden.readResistanceEC(); //EC Value in resistance
  EC = OpenGarden.ECConversion(resistanceEC)*100; //EC Value in µS/cm

}

void send_sensors()

{
  
 
  //Serial Print all values
  char buf[50];
  sprintf(buf, "%4d %4d %4d %4d %4d %4d",airTemperature, airHumidity, waterTemperature, luminosity, pH, EC);
   
  Serial.println(buf);  
    
}  


void debug_sensors()
{
  //Serial Print all values
  Serial.println(F("***************************")); 
  OpenGarden.printTime(myTime);
  Serial.println("");
  Serial.print(F("Air Temperature: "));
  Serial.print(airTemperature,4);
  Serial.println("*C");
  Serial.print(F("Air Humidity: "));
  Serial.print(airHumidity,4);
  Serial.println(F("% RH"));
  Serial.print(F("Water Temperature: "));
  Serial.print(waterTemperature,4);
  Serial.println("*C");
  Serial.print(F("Luminosity: "));
  Serial.print(luminosity,4);
  Serial.println("%");
  Serial.print(F("pH Level: "));
  Serial.println(pH,4);
  Serial.print(F("Conductivity: "));
  Serial.print(EC,4);
  Serial.println(F("uS/cm"));
}  



