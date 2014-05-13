/*
SIM800 Serial Debug 

This sketch is used to communicate with SIM800 with AT commands.

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/

#include <sim800_Suli.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Suli.h>

const int pin_tx = 8;
const int pin_rx = 7;
const int pin_power = 9;

SoftwareSerial gprs(pin_tx,pin_rx);//TX,RX

void setup(){
  Serial.begin(9600);
  sim800_init(&gprs, -1, 9600);
  sim800_power_on(pin_power);
}

void loop(){
  if(gprs.available()){
    Serial.write(gprs.read());
  }
  if(Serial.available()){     
    gprs.write(Serial.read()); 
  }
}



