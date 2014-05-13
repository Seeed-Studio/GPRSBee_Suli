/*
* gprs.cpp 
* A library for SeeedStudio seeeduino GPRS shield 
*  
* Copyright (c) 2013 seeed technology inc. 
* Author  		: 	lawliet.zou(lawliet.zou@gmail.com)
* Create Time	: 	Dec 23, 2013 
* Change Log 	: 
*
* The MIT License (MIT)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "sim800_Suli.h"

SoftwareSerial *serialSIM800 = NULL;
IO_T _pinPower;

void  sim800_init(void * uart_device, int16 uart_num, uint32 baud)
{
	serialSIM800 = (SoftwareSerial*)uart_device;
	suli_uart_init(uart_device, uart_num, baud);
}

void  sim800_power_on(PIN_T powerPin)
{
	suli_pin_init(&_pinPower, powerPin);
	suli_pin_dir(&_pinPower,HAL_PIN_OUTPUT);
	suli_pin_write(&_pinPower,HAL_PIN_LOW);
	sim800_flush_serial();
	if(0 != sim800_check_with_cmd("AT\r\n","OK",2,CMD)) {
        suli_pin_write(&_pinPower,HAL_PIN_HIGH);
        suli_delay_ms(2000);
        suli_pin_write(&_pinPower,HAL_PIN_LOW);
    }	
}

char sim800_read_byte(void)
{
	return suli_uart_read_byte(serialSIM800,-1);
}

int sim800_check_readable()
{
	return suli_uart_readable(serialSIM800,-1);
}

int sim800_wait_readable(int wait_time)
{
	unsigned long timerStart,timerEnd;
	int dataLen = 0;
	timerStart = suli_millis();
    timerEnd = 1000*wait_time + timerStart;
	while(suli_millis() < timerEnd) {
		suli_delay_ms(500);
		dataLen = suli_uart_readable(serialSIM800,-1);
		if(dataLen > 0){
			break;
		}
    }
    return dataLen;
}

void sim800_flush_serial()
{
	while(suli_uart_readable(serialSIM800,-1)){
		char c = suli_uart_read_byte(serialSIM800,-1);
	}
}

int sim800_read_buffer(char *buffer,int count, unsigned int timeOut)
{
    int i = 0;
	unsigned long timerStart,timerEnd;
	timerStart = suli_millis();
    timerEnd = 1000*timeOut + timerStart;
    while(1) {
#if 0
        while (suli_uart_readable(serialSIM800,-1)) {
            char c = suli_uart_read_byte(serialSIM800,-1);
            buffer[i++] = c;
            if(i >= count)break;
        }
        if(i >= count)break;
		if(suli_millis() > timerEnd) {
            break;
        }
#else
/** Anyway, Suli is not quickly enough to fetch data from buffer, so 
	we have to fetch our data like this.*/
		while (serialSIM800->available()) {
			char c = serialSIM800->read();
			buffer[i++] = c;
			if(i >= count)break;
		}
		if(i >= count)break;
		if(millis() > timerEnd) {
			break;
		}
#endif
    }
    return 0;	
}

void sim800_clean_buffer(char *buffer, int count)
{
    for(int i=0; i < count; i++) {
        buffer[i] = '\0';
    }
}

void sim800_send_cmd(const char* cmd)
{
	suli_uart_send(serialSIM800, -1, (uint8_t*)cmd, strlen(cmd));
}

void sim800_send_AT(void)
{
	sim800_check_with_cmd("AT\r\n","OK",DEFAULT_TIMEOUT,CMD);
}

bool sim800_response_cmp(uint8_t* resp, unsigned int len, unsigned int timeout)
{
    int sum=0;
	unsigned long timerStart,timerEnd;
	timerStart = suli_millis();
    timerEnd = 1000*timeout + timerStart;
    while(1) {
        if(suli_uart_readable(serialSIM800,-1)) {
            char c = suli_uart_read_byte(serialSIM800,-1);
            sum = (c==resp[sum]) ? sum+1 : 0;
            if(sum == len)break;
        }
		if(suli_millis() > timerEnd) {
            return false;
        }
    }
    return true;	
}

int sim800_wait_for_resp(const char* resp, unsigned int timeout,DataType type)
{
    int len = strlen(resp);
    int sum = 0;
	unsigned long timerStart,timerEnd;
	timerStart = suli_millis();
    timerEnd = 1000*timeout + timerStart;
    while(1) {
        if(suli_uart_readable(serialSIM800,-1)) {
            char c = suli_uart_read_byte(serialSIM800,-1);
            sum = (c==resp[sum]) ? sum+1 : 0;
            if(sum == len)break;
        }
        if(suli_millis() > timerEnd) {
            return -1;
        }
    }

    if(type == CMD) {
        while(suli_uart_readable(serialSIM800,-1)) {
            char c = suli_uart_read_byte(serialSIM800,-1);
        }
    }

    return 0;	

}

void sim800_send_End_Mark(void)
{
	suli_uart_send_byte(serialSIM800,-1,(char)26);
}


int sim800_check_with_cmd(const char* cmd, const char *resp, unsigned timeout, DataType type)
{
    sim800_send_cmd(cmd);
    return sim800_wait_for_resp(resp,timeout,type);
}


