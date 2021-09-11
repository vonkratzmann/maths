/*
 * Maths Game
 *
 * Note using a 2.8 INCH TOUCHSCREEN LCD SCREEN SHIELD FOR ARDUINO UNO from Aus Electronics Direct.
 * The display is 240x320 with a resistive touchscreen.
 * The supplier specifications said ILI9341 controller,
 * ran test program and ID = 0x4747. From the web said it means Himax HX8347-D controller.
 * These are not very common controllers
 * and are not supported by default.
 * Had to edit MCUFRIEND_kbv.cpp: and uncomment #define SUPPORT_8347D.
 *
 * To use, touch the product field which will change colour,
 * touch the digits to make up the answer,
 * and then touch "Ent" or enter and the program will check the answer.
 * Characters in the product field can be deleted by touching "Back" or backspace.
 * A new game is started by touching "New".
 *
 * Supports '+', '-', '*', '/', '%' operators.
 * Maximum multiplicand and multiplier number is 30.
 * Minimum multiplicand and multiplier number is -30.
 * In the code the number of operators can be reduced,
 * and the max and min values can be reduced,
 * to make the calculations easier.
 *
 * All arithmetic operations use the same names for terms as multiply operation, ie
 * multiplicand, operator, multiplier, equals and product.
 * Only the product field is editable by the user,
 * the multiplicand, operator, and multiplier, are generated by a randomn number generator.
 * The maximum number of characters that can be entered in the product field is 4, ie "-999".
 * All arithmetic is integer based.
 * There is check to ensure the randomn number generator does not generate a divide by '0'.
 */

#include "maths.h"
#include "terms.h"
#include "equation.h"
#include "numbers_command.h"
#include "touch.h"
#include "battery.h"

MCUFRIEND_kbv lcdScreen;

Equation *equHorz1;
Equation *equHorz2;
Equation *equVert1;
Equation *equVert2;

//tracks which equation has been selected
Equation *selectedEquation;

Touch *touch;
Battery *battery;

Number *numbersOnScreen[12];                //-, 0, 1, ... 9.
Command *commandsOnScreen[NUMBER_COMMANDS]; //enter(e), backspace(b), new game(n)

char *commandStrings[NUMBER_COMMANDS] = {"Ent", "Bck", "New"};

/*
 * Can set number of operators in use, min is 1 max is 5,
 * so only displays that number of operators.
 * So 4 would give you '+', '*', '-', '/'.
 * So 3 would give you '+', '*', '-'.
 * etc etc .
 */
char operators[5] = {'+', '*', '-', '/', '%'};

/*
 * If "overRide" is true always indicates correct answer,
 * regardless of what was entered.
 * Just to make it a bit more fun.
 * Cleared at start of a new game.
 */
bool overRide = false;

bool volatile justWokenUp = false;
/* 
 * Interrupt used for wake up after being put into low power mode
 */
ISR(PCINT1_vect)
{
    justWokenUp = true;
}

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
#endif

    uint16_t ID = lcdScreen.readID();
    lcdScreen.begin(ID);
    lcdScreen.setRotation(ORIENTATION);

    lcdScreen.fillScreen(TFT_BLACK);
    lcdScreen.setFont(&FreeMono18pt7b);

    //As only a few equations generate manually
    equHorz1 = new Equation();
    equHorz2 = new Equation();
    equVert1 = new Equation();
    equVert2 = new Equation();
    touch = new Touch();
    battery = new Battery();

    newGame();
}

void loop()
{
    long timeLastTouched;
    if (touch->isThereATouch())
    {
        int16_t xTouch;
        int16_t yTouch;

        char numberTouched;
        char commandTouched;
        int8_t equationNumberTouched;

        timeLastTouched = micros();

        touch->getTouchPoint(&xTouch, &yTouch);

        if ((equationNumberTouched = isEquationTouched(xTouch, yTouch)) != 0)
        {
            processEquationTouched(equationNumberTouched);
        }

        else if ((numberTouched = isNumberTouched(xTouch, yTouch)) != '\0')
        {
            processNumberTouched(numberTouched);
        }

        else if ((commandTouched = isCommandTouched(xTouch, yTouch)) != '\0')
        {
            //only command processed here is a new game
            newGame();
        }
    }
    if (micros() - timeLastTouched > IDLE_TIME_BEFORE_SHUTDOWN_MSEC)
    {
        battery->shutDown();
    }
    if (justWokenUp)
    {
        justWokenUp = false;
        newGame();
    }
}
//end of main loop

/*
 * isEquationTouched()
 */
int8_t isEquationTouched(int16_t xTouch, int16_t yTouch)
{
    if (equHorz1->product_->touched(xTouch, yTouch))
    {
        return 1;
    }
    if (equHorz2->product_->touched(xTouch, yTouch))
    {
        return 2;
    }
    if (equVert1->product_->touched(xTouch, yTouch))
    {
        return 3;
    }
    if (equVert2->product_->touched(xTouch, yTouch))
    {
        return 4;
    }
    return 0;
}

/*
 *  isNumberTouched()
 */
char isNumberTouched(int16_t xTouch, int16_t yTouch)
{
    for (int i = 1; i < 11; i++)
    {
        if (numbersOnScreen[i]->touched(xTouch, yTouch))
        {
            return i - 1 + '0'; //convert to ascii character from '0' to '9'
        }
    }
    if (numbersOnScreen[0]->touched(xTouch, yTouch))
    {
        return '-';
    }
    if (commandsOnScreen[0]->touched(xTouch, yTouch)) //kk fix, instead of zero put label
    {
        return 'e';
    }
    if (commandsOnScreen[1]->touched(xTouch, yTouch)) //kk fix, instead of one put label
    {
        return 'b';
    }
    return '\0';
}

/*
 * isCommandTouched()
 *
 * Handles new game command.
 */
char isCommandTouched(int16_t xTouch, int16_t yTouch)
{
    if (commandsOnScreen[2]->touched(xTouch, yTouch)) //kk need to fix, instead of two put label
    {
        return 'n'; //new game
    }
    return '\0';
}

/*
 * processEquationTouched()
 *
 * Only one product field selected at a time,
 * so check if this product was selected successfully,
 * if so clear any other selected products.
 */
void processEquationTouched(int8_t equationNumber)
{
    selectedEquation = NULL;
    switch (equationNumber)
    {
    case 1:
        equHorz1->processProductTouch();
        if (equHorz1->getState() == PRODUCT_SELECTED)
        {
            clearOtherSelectedProducts(equHorz1);
            selectedEquation = equHorz1;
        }
        break;

    case 2:
        equHorz2->processProductTouch();
        if (equHorz2->getState() == PRODUCT_SELECTED)
        {
            clearOtherSelectedProducts(equHorz2);
            selectedEquation = equHorz2;
        }
        break;

    case 3:
        equVert1->processProductTouch();
        if (equVert1->getState() == PRODUCT_SELECTED)
        {
            clearOtherSelectedProducts(equVert1);
            selectedEquation = equVert1;
        }
        break;

    case 4:
        equVert2->processProductTouch();
        if (equVert2->getState() == PRODUCT_SELECTED)
        {
            clearOtherSelectedProducts(equVert2);
            selectedEquation = equVert2;
        }
        break;
    }
}

/*
 * processNumberTouched()
 *
 * only process number if equation has been selected
 */
void processNumberTouched(char c)
{
    if (selectedEquation != NULL && selectedEquation->getState() == PRODUCT_SELECTED)
    {
        selectedEquation->processNumber(c);
    }
}

/*
 * newGame()
 *
 * Deletes the current terms in each equation,
 * freeing up heap memory and
 * the generate new equations.
 * Numbers and commands are not updated as these do not change between games.
 *
 * Delete terms in the reverse order in which they were created.
 * Aim was to minimise heap fragmentation.
 *
 * Clears overRide code, which overrides any answer to always give the correct answer
 */
void newGame()
{
    deleteTermsVert(equVert2);
    deleteTermsVert(equVert1);
    deleteTermsHorz(equHorz2);
    deleteTermsHorz(equHorz1);
    generateEquations();
    displayNumbers();
    displayCommands();

    overRide = false;
}

/*
 * deleteTermsHorz()
 *
 * Delete the heap memory that has been allocated to the equation terms
 * by the earlier new() calls. This is to free up heap memory before
 * generating new equations using new() calls.
 * Check the pointers exist, before deleting,
 * this covers the case where just powered up and equations not generated.
 */
void deleteTermsHorz(Equation *equation)
{
    if (equation->product_ != NULL)
    {
        delete (equation->product_);
        delete (equation->equals_);
        delete (equation->multiplier_);
        delete (equation->operator_);
        delete (equation->multiplicand_);
    }
}

/*
 * deleteTermsVert()
 *
 * As the vertical multiplier and multiplicand are the same as the horizontal
 * only delete these in deleteTermsHorz().
 * Otherwise if you call delete() on the multiplier and multiplicand here,
 * after they have been deleted in deleteTermsHorz(),
 * the heap memory has already been deallocated and the pointer is no longer valid
 * and the program will crash.
 * Check the pointers exist, before deleting,
 * this covers the case where just powered up and equations not generated.
 */

void deleteTermsVert(Equation *equation)
{
    if (equation->product_ != NULL)
    {
        delete (equation->product_);
        delete (equation->equals_);
        delete (equation->operator_);
    }
}

/*
 * generateEquations()
 *
 * As only small number of equations, and as they are in different directions, generate manually.
 * The rectangles, border and background colours are diplayed by the constructor in the base class "Graphic".
 * For the multiplicand, operator and multiplier,
 * the values are generated by a randomn generator in the constructor in the respective class,
 * and then displayed on the screen also by the respective class constructor.
 * For an Equals class the constructor just displays the '='.
 * For the product no value is generated as this has to be entered by the user.
 * Checks if there is divide by zero in any equation, if so changes zero to a 1.
 *
 * The lines of code below, were initially laid out in vertical alignment, but the document auto format gave this:
 */
void generateEquations()
{
    equHorz1->multiplicand_ = new Multi(&lcdScreen, 0, 0, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz1->operator_ = new Operator(&lcdScreen, FONT_WIDTH * 3, 0, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equHorz1->multiplier_ = new Multi(&lcdScreen, FONT_WIDTH * 4, 0, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz1->equals_ = new Equals(&lcdScreen, FONT_WIDTH * 7, 0, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equHorz1->product_ = new Product(&lcdScreen, FONT_WIDTH * 8, 0, FONT_WIDTH * 4, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz1->setState(PRODUCT_UNSELECTED);
    checkDivideByZero(equHorz1);

    equHorz2->multiplicand_ = new Multi(&lcdScreen, 0, FONT_HEIGHT * 2, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz2->operator_ = new Operator(&lcdScreen, FONT_WIDTH * 3, FONT_HEIGHT * 2, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equHorz2->multiplier_ = new Multi(&lcdScreen, FONT_WIDTH * 4, FONT_HEIGHT * 2, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz2->equals_ = new Equals(&lcdScreen, FONT_WIDTH * 7, FONT_HEIGHT * 2, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equHorz2->product_ = new Product(&lcdScreen, FONT_WIDTH * 8, FONT_HEIGHT * 2, FONT_WIDTH * 4, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz2->setState(PRODUCT_UNSELECTED);
    checkDivideByZero(equHorz2);

    equVert1->multiplicand_ = equHorz1->multiplicand_;
    equVert1->operator_ = new Operator(&lcdScreen, FONT_WIDTH, FONT_HEIGHT, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equVert1->multiplier_ = equHorz2->multiplicand_;
    equVert1->equals_ = new Equals(&lcdScreen, FONT_WIDTH, FONT_HEIGHT * 3, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equVert1->product_ = new Product(&lcdScreen, 0, FONT_HEIGHT * 4, FONT_WIDTH * 4, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equVert1->setState(PRODUCT_UNSELECTED);
    checkDivideByZero(equVert1);

    equVert2->multiplicand_ = equHorz1->multiplier_;
    equVert2->operator_ = new Operator(&lcdScreen, FONT_WIDTH * 5, FONT_HEIGHT, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equVert2->multiplier_ = equHorz2->multiplier_;
    equVert2->equals_ = new Equals(&lcdScreen, FONT_WIDTH * 5, FONT_HEIGHT * 3, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equVert2->product_ = new Product(&lcdScreen, FONT_WIDTH * 4, FONT_HEIGHT * 4, FONT_WIDTH * 4, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equVert2->setState(PRODUCT_UNSELECTED);
    checkDivideByZero(equVert2);

    selectedEquation = NULL;
}

/*
 *  checkDivideByZero()
 *
 * Checks if there is divide by zero in any equation, if so changes zero to a 1.
 * Redisplays the new value;
 */
void checkDivideByZero(Equation *equation)
{
    if (equation->multiplicand_->getValue() == 0 && (equation->operator_->getOper() == '/' || equation->operator_->getOper() == '%'))
    {
        equation->multiplicand_->setValue(1);
        equation->multiplicand_->drawValue();
    }
}

/*
 * displayNumbers()
 *
 * These are displayed on the screen
 * '-', '0', '1', .. '9'
 */
void displayNumbers()
{
    int16_t x = 0;
    char c = '0';
    uint8_t i = 0;

    numbersOnScreen[i++] = new Number(&lcdScreen, x, FONT_HEIGHT * 5, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR, '-');

    x += FONT_WIDTH;

    //start loop at 1 as the '-' uses numberOnScreen[0]
    for (; i < 11; i++, x += FONT_WIDTH, c += 1)
    {
        numbersOnScreen[i] = new Number(&lcdScreen, x, FONT_HEIGHT * 5, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR, c);
    }
}

/*
 * displayCommands()
 *
 * These are displayed on the screen
 * "Enter", "Back", "New"
 *
 * Even though the three command definitions are in the one array,
 * processing of "Enter", "Back", is handled differently to "New".
 * "Enter", "Back" are handled by the isNumberTouched() and processNumberTouched().
 * "New" is handled by isCommandTouched() and then a call to NewGame().
 */
void displayCommands()
{
    int16_t x = 0;
    for (uint8_t i = 0; i < NUMBER_COMMANDS; i++, x += FONT_WIDTH * 3)
    {
        commandsOnScreen[i] = new Command(&lcdScreen, x, FONT_HEIGHT * 6, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR, commandStrings[i]);
    }
}

/*
 * clearOtherSelectedProducts()
 *
 * Can only have one product field selected for data entry at a time
 * parameter 'equ' is the equation which has just been selected.
 * Checks each equation and if it is not the one just selected,
 * ensure it is set to unselected.
 */
void clearOtherSelectedProducts(Equation *equ)
{
    if (equHorz1->getState() == PRODUCT_SELECTED && equ != equHorz1)
    {
        equHorz1->setState(PRODUCT_UNSELECTED);
        equHorz1->product_->drawBg(BG_COLOUR);
    }

    if (equHorz2->getState() == PRODUCT_SELECTED && equ != equHorz2)
    {
        equHorz2->setState(PRODUCT_UNSELECTED);
        equHorz2->product_->drawBg(BG_COLOUR);
    }

    if (equVert1->getState() == PRODUCT_SELECTED && equ != equVert1)
    {
        equVert1->setState(PRODUCT_UNSELECTED);
        equVert1->product_->drawBg(BG_COLOUR);
    }

    if (equVert2->getState() == PRODUCT_SELECTED && equ != equVert2)
    {
        equVert2->setState(PRODUCT_UNSELECTED);
        equVert2->product_->drawBg(BG_COLOUR);
    }
}
