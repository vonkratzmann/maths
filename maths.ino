/*
 * Maths Game
 *
 * Note using a 2.8 INCH TOUCHSCREEN LCD SCREEN SHIELD FOR ARDUINO UNO from Aus Electronics Direct.
 * Ran test program and ID = 0x4747. From the web said it means Himax HX8347-D controller. These are not very common controllers
 * and are not supported by default.
 * Had to edit MCUFRIEND_kbv.cpp: and uncomment #define SUPPORT_8347D.
 * 
 * To use touch the product field which will change colour,
 * touch the digits to make up the answer, 
 * and then touch "Ent" or enter and the program will check the answer.
 * Characters in the product field can be deleted by touching "Back" or backspace.
 * A new game is started by touching "New".
 */

#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

#include <Fonts/FreeMono18pt7b.h>
#include "maths.h"
#include "terms.h"
#include "equation.h"
#include "numbers_command.h"
#include "touch.h"

//#define DEBUG //comment out if not debugging

Equation *equHorz1;
Equation *equHorz2;
Equation *equVert1;
Equation *equVert2;

//tracks which equation has been selected
Equation *selectedEquation;

Touch *touch;

Number *numbersOnScreen[12];            //0, 1, ... 9, +, -,
Command *commandsOnScreen[NO_COMMANDS]; //enter(e), backsapce(b), new game(n)

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
#endif

    randomSeed(analogRead(A5)); //Seed generator with noise from unused AIN

    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(ORIENTATION);

    tft.fillScreen(TFT_BLACK);
    tft.setFont(&FreeMono18pt7b);

    //As only a few equations generate manually
    equHorz1 = new Equation();
    equHorz2 = new Equation();
    equVert1 = new Equation();
    equVert2 = new Equation();
    touch = new Touch();

    generateEquations();
    displayNumbers();
    displayCommands();
}

void loop()
{
    int16_t xTouch;
    int16_t yTouch;

    char numberTouched;
    char commandTouched;
    int8_t equationNumberTouched;

    if (touch->isThereATouch())
    {
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
            return i - 1 + '0'; //convert to ascci character from '0' to '9'
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
        equHorz1->processTouch();
        if (equHorz1->getState() == PRODUCT_SELECTED)
        {
            clearOtherSelectedProducts(equHorz1);
            selectedEquation = equHorz1;
        }
        break;

    case 2:
        equHorz2->processTouch();
        if (equHorz2->getState() == PRODUCT_SELECTED)
        {
            clearOtherSelectedProducts(equHorz2);
            selectedEquation = equHorz2;
        }
        break;

    case 3:
        equVert1->processTouch();
        if (equVert1->getState() == PRODUCT_SELECTED)
        {
            clearOtherSelectedProducts(equVert1);
            selectedEquation = equVert1;
        }
        break;

    case 4:
        equVert2->processTouch();
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
 * Generate new multiplicand, operator, multiplier vlaues for each equation,
 * overwrites the old values, and display the new values.
 * For the product, the field is cleared and no value is generated as this
 * has to be entered by the user.
 * Ensure all equations are UNSELECTED at start of the game.
 */
void newGame()
{
    equHorz1->multiplicand_->newValue();
    equHorz1->multiplicand_->drawValue();
    equHorz1->multiplier_->newValue();
    equHorz1->multiplier_->drawValue();
    equHorz1->operator_->newOper();
    equHorz1->operator_->drawValue();
    equHorz1->product_->drawBg(BG_COLOUR);
    equHorz1->setState(PRODUCT_UNSELECTED);

    equHorz2->multiplicand_->newValue();
    equHorz2->multiplicand_->drawValue();
    equHorz2->multiplier_->newValue();
    equHorz2->multiplier_->drawValue();
    equHorz2->operator_->newOper();
    equHorz2->operator_->drawValue();
    equHorz2->product_->drawBg(BG_COLOUR);
    equHorz2->setState(PRODUCT_UNSELECTED);

    equVert1->multiplicand_->newValue();
    equVert1->multiplicand_->drawValue();
    equVert1->multiplier_->newValue();
    equVert1->multiplier_->drawValue();
    equVert1->operator_->newOper();
    equVert1->operator_->drawValue();
    equVert1->product_->drawBg(BG_COLOUR);
    equVert1->setState(PRODUCT_UNSELECTED);

    equVert2->multiplicand_->newValue();
    equVert2->multiplicand_->drawValue();
    equVert2->multiplier_->newValue();
    equVert2->multiplier_->drawValue();
    equVert2->operator_->newOper();
    equVert2->operator_->drawValue();
    equVert2->product_->drawBg(BG_COLOUR);
    equVert2->setState(PRODUCT_UNSELECTED);
}

/*
 * generateEquations()
 *
 * As only small number of equations, and as they are in different directions, generate manually.
 * The rectangles, border and background colours are diplayed by the constructor in the base class "Graphic".
 * For the multiplic and, operator and multiplier, 
 * the values are generated by a randomn generator in the constructor in the respective class,
 * and then displayed on the screen also by the respective class constructor.
 * For an equation the constructor just displays the '='.
 * For the product no value is generated as this has to be entered by the user.
 * Checks if there is divide by zero in any equation, if so changes zero to a 1.
 */
void generateEquations()
{
    equHorz1->multiplicand_ = new Multi(&tft, 0, 0, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz1->operator_ = new Operator(&tft, FONT_WIDTH * 3, 0, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equHorz1->multiplier_ = new Multi(&tft, FONT_WIDTH * 4, 0, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz1->equals_ = new Equals(&tft, FONT_WIDTH * 7, 0, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equHorz1->product_ = new Product(&tft, FONT_WIDTH * 8, 0, FONT_WIDTH * 4, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz1->setState(PRODUCT_UNSELECTED);
    checkDivideByZero(equHorz1);

    equHorz2->multiplicand_ = new Multi(&tft, 0, FONT_HEIGHT * 2, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz2->operator_ = new Operator(&tft, FONT_WIDTH * 3, FONT_HEIGHT * 2, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equHorz2->multiplier_ = new Multi(&tft, FONT_WIDTH * 4, FONT_HEIGHT * 2, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz2->equals_ = new Equals(&tft, FONT_WIDTH * 7, FONT_HEIGHT * 2, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equHorz2->product_ = new Product(&tft, FONT_WIDTH * 8, FONT_HEIGHT * 2, FONT_WIDTH * 4, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equHorz2->setState(PRODUCT_UNSELECTED);
    checkDivideByZero(equHorz2);

    equVert1->multiplicand_ = equHorz1->multiplicand_;
    equVert1->operator_ = new Operator(&tft, FONT_WIDTH, FONT_HEIGHT, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equVert1->multiplier_ = equHorz2->multiplicand_;
    equVert1->equals_ = new Equals(&tft, FONT_WIDTH, FONT_HEIGHT * 3, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equVert1->product_ = new Product(&tft, 0, FONT_HEIGHT * 4, FONT_WIDTH * 4, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
    equVert1->setState(PRODUCT_UNSELECTED);
    checkDivideByZero(equVert1);

    equVert2->multiplicand_ = equHorz1->multiplier_;
    equVert2->operator_ = new Operator(&tft, FONT_WIDTH * 5, FONT_HEIGHT, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equVert2->multiplier_ = equHorz2->multiplier_;
    equVert2->equals_ = new Equals(&tft, FONT_WIDTH * 5, FONT_HEIGHT * 3, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR);
    equVert2->product_ = new Product(&tft, FONT_WIDTH * 4, FONT_HEIGHT * 4, FONT_WIDTH * 4, FONT_HEIGHT, FG_COLOUR, BG_COLOUR, BORDER_COLOUR);
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

    numbersOnScreen[i++] = new Number(&tft, x, FONT_HEIGHT * 5, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR, '-');

    x += FONT_WIDTH;

    //start loop at 1 as the '-' uses numberOnScreen[0]
    for (; i < 11; i++, x += FONT_WIDTH, c += 1)
    {
        numbersOnScreen[i] = new Number(&tft, x, FONT_HEIGHT * 5, FONT_WIDTH, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR, c);
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
 */
void displayCommands()
{
    int16_t x = 0;
    for (uint8_t i = 0; i < NO_COMMANDS; i++, x += FONT_WIDTH * 3)
    {
        commandsOnScreen[i] = new Command(&tft, x, FONT_HEIGHT * 6, FONT_WIDTH * 3, FONT_HEIGHT, FG_COLOUR, BG_COLOUR_OPER, BORDER_COLOUR, commandStrings[i]);
    }
}

/*
 * clearOtherSelectedProducts()
 *
 * can only have one product field slected for data entry at a time
 * parameter 'equ' is the equation which has just been selected
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
