#include "hd44780.h"

#include <wiringPi.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    hd44780 header;

    hd44780_setDefaultPins(&header);

    hd44780_initalizeDisplay(&header, 1);

    hd44780_writeBytes(&header, 0x33, 0);
    hd44780_writeBytes(&header, 0x32, 0);
    hd44780_writeBytes(&header, 0x28, 0);
    hd44780_writeBytes(&header, 0x0C, 0);
    hd44780_writeBytes(&header, 0x0F, 0);

    while(1)
    {
        hd44780_clearDisplay(&header);

        hd44780_moveCursor(&header, CURSOR_HOME);

        int i =  rand();
        int g = i ^ (i >> 1);

        hd44780_moveCursor(&header, CURSOR_HOME);
        hd44780_printInt32(&header, i, 16);
        hd44780_writeBytes(&header, HD44780_DDRAM_ADDRESS | 0xC0, COMMAND_MODE);
        hd44780_printInt32(&header, g, 16);

        delay(2000);
    }
}
