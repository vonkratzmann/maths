/*
 * Maths defines and constants
 */

#ifndef Maths_h
#define Maths_h

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
#define MAX_MULTIPLICAND 20
#define MIN_MULTIPLICAND 0

#define MAX_DATA_LEN 4 // max length of the data, ie 3 numbers plus a negative sign

//check to make sure it is 30 or less
#if MAX_MULTIPLICAND > 30
#error "Muliplicand must be less than or equal to 30"
#endif

#define PRODUCT_SELECTED 0
#define PRODUCT_UNSELECTED 1
#define PRODUCT_INCORRECT 2
#define PRODUCT_CORRECT 3

char operators[5] = {'+', '*', '-', '/', '%'};

/*
 * If "overRide" is true always indicates correct answer,
 * regardless of what was entred.
 * Just to make it a bit more fun.
 */
bool overRide = false;
#define OVER_RIDE_CODE "99-"
#define OVER_RIDE_CLEAR "9-"

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

#define NO_COMMANDS 3
const char *commandStrings[NO_COMMANDS] = {"Ent", "Bck", "New"};

#endif