
/*
 * Maths defines and constants
 */

#ifndef Maths_h
#define Maths_h

#include "MCUFRIEND_kbv.h"
#include "Fonts\FreeMono18pt7b.h"

//comment out define below if not debugging
#define DEBUG

//display text message, variable name and value
#ifdef DEBUG
#define dbg(myFixedText, variableName)                \
    Serial.print(#myFixedText " " #variableName "="); \
    Serial.println(variableName);

//#define DEBUG_TOUCHED
#endif

#define BG_COLOUR TFT_ORANGE
#define BG_COLOUR_OPER TFT_DARKGREY
#define FG_COLOUR TFT_WHITE
#define BORDER_COLOUR TFT_BLUE
#define PRODUCT_SELECTED_COLOUR TFT_LIGHTGREY
#define PRODUCT_CORRECT_COLOUR TFT_GREEN
#define PRODUCT_INCORRECT_COLOUR TFT_RED

//fonts sit lower when drawn, so move up by 7 points
#define FONT_VERT_CORRECTION 7
#define FONT_WIDTH 22
#define FONT_HEIGHT 34

/*
 * Designed for multiplicand and multipliers from -30 to +30,
 * although the maximum can be set to lower than 30 for easier problems.
 * So for the multiplicand and multiplier the field for data entry
 * is 3 characters wide,  ie -30.
 * The product field for data entry is 4 charaters wide, ie -900.
 */
#define MAX_MULTIPLICAND 12
#define MIN_MULTIPLICAND 0

#define MAX_DATA_LEN 4 // max length of the data, ie 3 numbers plus a negative sign

//check to make sure it is 30 or less
#if MAX_MULTIPLICAND > 30
#error "MAX_MULTIPLICAND must be less than or equal to 30"
#endif

#if MIN_MULTIPLICAND < -30
#error "Multiplicand must be greater than or equal to -30"
#endif

#define PRODUCT_SELECTED 0
#define PRODUCT_UNSELECTED 1
#define PRODUCT_INCORRECT 2
#define PRODUCT_CORRECT 3

#define OVER_RIDE_CODE "99-"

/*
 * Orientation:
 * 0 sets the display to portrait (tall) mode, with the USB jack at the top
 * 1 sets the display to landscape (wide) mode, with the USB jack at the left
 */
#define PORTRAIT 0
#define LANDSCAPE 1
#define ORIENTATION LANDSCAPE

#if ORIENTATION == PORTRAIT
#define WIDTH 240
#define HEIGHT 320
#else
#define WIDTH 320
#define HEIGHT 240
#endif

#define TERMINATOR_CHAR ('\r')

/*
 * Number of command is 3, ie enter(e), backspace(b), new game(n)
 */
#define NUMBER_COMMANDS 3

/*
 * Can set number of operators in use, min is 1 max is 5,
 * so only displays that number of operators.
 * So 5 would give you '+', '*', '-', '/', '%'
 * So 4 would give you '+', '*', '-', '/'.
 * So 3 would give you '+', '*', '-'.
 * etc etc .
 */
#define NUMBER_OPERATORS_IN_USE 5

// kk #define IDLE_TIME_BEFORE_SHUTDOWN_MSEC 5 * 60 * 1000
#define IDLE_TIME_BEFORE_SHUTDOWN_MSEC  20 * 1000
//use PORTC bit 0 for wakeup pin, when in low power mode,
//pin 23 on ATMega328P, A0 in datasheet, PCINT8,
//called A5 on Arduino Uno board.
#define WAKE_UP_PIN PORTC0
#define WAKE_UP_PORT PORTC
#define WAKE_UP_DDR DDRC

/*
 * On boot ie a power up, define equations in heap.
 * When user starts a new game or coming out of "power down" mode,
 * the old equation defintions in the heap are deleted and
 * new equations defined. 
 */
enum powerUp
{
    Boot,
    Reset
};

#endif
