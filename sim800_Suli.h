/*
 * SIM800_Suli.h 
 * A library for SeeedStudio seeeduino GPRS shield 
 *  
 * Copyright (c) 2014 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : lawliet zou
 * Create Time: April 2014
 * Change Log :
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

#ifndef __SIM800_SULI_H__
#define __SIM800_SULI_H__

#include "Suli.h"
#include <SoftwareSerial.h>

#define TRUE                1
#define FALSE               0
#define DEFAULT_TIMEOUT     5

enum DataType {
    CMD     = 0,
    DATA    = 1,
};

void  sim800_init(void * uart_device, int16 uart_num, uint32 baud);
void  sim800_power_on(PIN_T powerPin);
int   sim800_check_readable();
int   sim800_wait_readable(int wait_time);
void  sim800_flush_serial();
char  sim800_read_byte(void);
int   sim800_read_buffer(char* buffer,int count, unsigned int timeOut);
void  sim800_clean_buffer(char* buffer, int count);
void  sim800_send_cmd(const char* cmd);
void  sim800_send_AT(void);
void  sim800_send_End_Mark(void);
bool  sim800_response_cmp(uint8_t* resp, unsigned int len, unsigned int timeout);
int   sim800_wait_for_resp(const char* resp, unsigned int timeout, DataType type);
int   sim800_check_with_cmd(const char* cmd, const char *resp, unsigned timeout, DataType type);
#endif