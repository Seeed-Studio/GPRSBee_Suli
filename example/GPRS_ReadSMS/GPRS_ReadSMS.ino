/*
GPRS Read SMS

This sketch is used to test GPRS Bee's reading SMS function.

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/
#include <GPRS_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Suli.h>

#define PIN_TX    8
#define PIN_RX    7
#define PIN_PWR   9
#define BAUDRATE  9600

char gprsBuffer[64];
int i = 0;
char *s = NULL;
int inComing = 0;

GPRS gprsTest(PIN_TX,PIN_RX,PIN_PWR,BAUDRATE);//RX,TX,PWR,BaudRate

void setup() {
  Serial.begin(9600);
  while(0 != gprsTest.init()) {
    delay(1000);
    Serial.print("init error\r\n");
  }
  delay(3000);  
  Serial.println("Init Success, please call or send SMS message to me!");
}

void loop() {
  if(gprsTest.readable()) {
    inComing = 1;
  }else{ delay(100);}
   
  if(inComing){
    sim800_read_buffer(gprsBuffer,32,DEFAULT_TIMEOUT);
    //Serial.print(gprsBuffer);
    if(NULL != (s = strstr(gprsBuffer,"+CMTI: \"SM\""))) { //SMS: $$+CMTI: "SM",24$$
      char message[MESSAGE_LENGTH];
      int messageIndex = atoi(s+12);
      gprsTest.readSMS(messageIndex, message,MESSAGE_LENGTH);
      Serial.print("Recv Message: ");
      Serial.println(message);
    }
    sim800_clean_buffer(gprsBuffer,32);  
    inComing = 0;
  }
}
