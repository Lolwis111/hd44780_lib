#include "hd44780.h"

#include <stdio.h>

#include <wiringPi.h>

void hd44780_setDefaultPins(hd44780 *toDefault)
{
    if(!toDefault) 
        return;

    toDefault->D4 = 24;
    toDefault->D5 = 25;
    toDefault->D6 = 12;
    toDefault->D7 = 16;

    toDefault->registerSelect = 21;
    toDefault->enable = 20;

    toDefault->cols = 2;
    toDefault->rows = 16;

    toDefault->pulseDelay = 5;
}

void hd44780_initalizeDisplay(hd44780 *header, int gpioInit)
{
    if(!header)
        return;

    if(gpioInit != 0)
        wiringPiSetupGpio();

    pinMode(header->D4, OUTPUT);
    pinMode(header->D5, OUTPUT);
    pinMode(header->D6, OUTPUT);
    pinMode(header->D7, OUTPUT);

    pinMode(header->registerSelect, OUTPUT);
    pinMode(header->enable, OUTPUT);

    hd44780_writeBytes(header, 0x33, 0);
    hd44780_writeBytes(header, 0x32, 0);
    hd44780_writeBytes(header, 0x28, 0);
    hd44780_writeBytes(header, 0x0C, 0);
    hd44780_writeBytes(header, 0x0F, 0);
}

static void hd44780_pulse(hd44780 *header)
{
    delay(header->pulseDelay);

    digitalWrite(header->enable, LOW);

    delay(header->pulseDelay);

    digitalWrite(header->enable, HIGH);

    delay(header->pulseDelay);

    digitalWrite(header->enable, LOW);

    delay(header->pulseDelay);
}

void hd44780_writeBytes(hd44780 *header, int byte, int mode)
{
    if(!header)
        return;

    digitalWrite(header->registerSelect, mode);

    delay(header->pulseDelay);

    digitalWrite(header->D4, LOW);
    digitalWrite(header->D5, LOW);
    digitalWrite(header->D6, LOW);
    digitalWrite(header->D7, LOW);

    if(byte & 0x10) 
        digitalWrite(header->D4, HIGH);
    
    if(byte & 0x20) 
        digitalWrite(header->D5, HIGH);
    
    if(byte & 0x40) 
        digitalWrite(header->D6, HIGH);
    
    if(byte & 0x80) 
        digitalWrite(header->D7, HIGH);

    hd44780_pulse(header);

    digitalWrite(header->D4, LOW);
    digitalWrite(header->D5, LOW);
    digitalWrite(header->D6, LOW);
    digitalWrite(header->D7, LOW);

    if(byte & 0x01) 
        digitalWrite(header->D4, HIGH);
    
    if(byte & 0x02) 
        digitalWrite(header->D5, HIGH);
    
    if(byte & 0x04) 
        digitalWrite(header->D6, HIGH);
    
    if(byte & 0x08) 
        digitalWrite(header->D7, HIGH);

    hd44780_pulse(header);
}

void hd44780_moveCursor(hd44780 *header, cursorMovement movement)
{
    if(!header) 
        return;

    if(movement == CURSOR_RIGHT) 
        hd44780_writeBytes(header, 0x14, 0);
    else if(movement == CURSOR_LEFT) 
        hd44780_writeBytes(header, 0x10, 0);
    else 
        hd44780_writeBytes(header, 0x02, 0);
}

void hd44780_printString(hd44780 *header, char *string)
{
    if(!header || !string || !*string) 
        return;

    int positionInLine = 0;
    int i;
    for(i = 0; string[i] != '\0'; i++)
    {
        if(positionInLine == header->rows)
        {
            hd44780_writeBytes(header, HD44780_DDRAM_ADDRESS | 0xC0, COMMAND_MODE);
            positionInLine = 0;
        }

        if(string[i] == '\n')
        {
            hd44780_writeBytes(header, HD44780_DDRAM_ADDRESS | 0xC0, COMMAND_MODE);
            positionInLine = 0;
        }
        else
        {
            hd44780_writeBytes(header, string[i], CHARACTER_MODE);
            positionInLine = positionInLine + 1;
        }
    }
}

void hd44780_scrollDisplay(hd44780 *header, displayScroll scroll)
{
    if(!header) 
        return;

    if(scroll == DISPLAY_SCROLL_LEFT)
    {
        hd44780_writeBytes(header, HD44780_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_LEFT, 
                COMMAND_MODE);
    }
    else if(scroll == DISPLAY_SCROLL_RIGHT)
    {
        hd44780_writeBytes(header, HD44780_CURSOR_SHIFT | HD44780_SHIFT_DISPLAY | HD44780_RIGHT, 
                COMMAND_MODE);
    }
}

void hd44780_clearDisplay(hd44780 *header)
{
    if(!header) 
        return;

    hd44780_writeBytes(header, 0x01, 0);
}

void hd44780_printInt32(hd44780 *header, int val, int base)
{
    if(!header) 
        return;

    if(base == 2)
    {
        int i;
        for(i = 0; i < 32; i++)
        {
            if(i == header->rows)
            {
                hd44780_writeBytes(header, HD44780_DDRAM_ADDRESS | 0xC0, COMMAND_MODE);

                if((val >> i) & 0x01)
                    hd44780_writeBytes(header, '1', 1);
                else
                    hd44780_writeBytes(header, '0', 1);
            }
       }
    }
    else if(base == 10)
    {
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", val);

        hd44780_printString(header, buffer);
    }
    else if(base == 16)
    {
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "0x%X", val);

        hd44780_printString(header, buffer);
    }
}
