#ifndef HD44780_H_
#define HD44780_H_

#define HD44780_CLEAR_DISPLAY       0x01
#define HD44780_RETURN_HOME         0x02
#define HD44780_ENTRY_MODE_SET      0x04
#define HD44780_DISPLAY_CONTROL     0x08
#define HD44780_CURSOR_SHIFT        0x10
#define HD44780_FUNCTION_SET        0x20
#define HD44780_CGRAM_ADDRESS       0x40
#define HD44780_DDRAM_ADDRESS       0x80

#define HD44780_SHIFT_ENTRY         0x01
#define HD44780_NOSHIFT_ENTRY       0x00
#define HD44780_ENTRY_DECREMENT     0x00
#define HD44780_ENTRY_INCREMENT     0x02

#define HD44780_DISPLAY_ON          0x04
#define HD44780_DISPLAY_OFF         0x00
#define HD44780_HIDE_CURSOR         0x00
#define HD44780_SHOW_CURSOR         0x02
#define HD44780_CURSOR_BLINK_ON     0x01
#define HD44780_CURSOR_BLINK_OFF    0x00

#define HD44780_MOVE_CURSOR         0x00
#define HD44780_SHIFT_DISPLAY       0x08
#define HD44780_LEFT                0x00
#define HD44780_RIGHT               0x04

#define HD44780_5x10FONT            0x04
#define HD44780_5x8FONT             0x00
#define HD44780_LINES               0x08
#define HD44780_LINE                0x00
#define HD44780_8BIT                0x10
#define HD44780_4BIT                0x00

typedef struct
{
    int registerSelect;
    int enable;

    int D4;
    int D5;
    int D6;
    int D7;

    int cols;
    int rows;

    int pulseDelay;

} hd44780;

typedef enum 
{ 
    CURSOR_LEFT, 
    CURSOR_RIGHT, 
    CURSOR_HOME 
} cursorMovement;

typedef enum 
{ 
    DISPLAY_SCROLL_LEFT, 
    DISPLAY_SCROLL_RIGHT 
} displayScroll;

typedef enum 
{ 
    COMMAND_MODE, 
    CHARACTER_MODE 
} modes;

void hd44780_setDefaultPins(hd44780 *toDefault);

void hd44780_initalizeDisplay(hd44780 *header, int initGpio);

void hd44780_writeBytes(hd44780 *header, int byte, int mode);

void hd44780_moveCursor(hd44780 *header, cursorMovement movement);

void hd44780_scrollDisplay(hd44780 *header, displayScroll scroll);

void hd44780_printString(hd44780 *header, char *string);

void hd44780_clearDisplay(hd44780 *header);

void hd44780_printInt32(hd44780 *header, int val, int base);


#endif // HD44780_H_
