/*
 * classes for multiplicand, multiplier, operator and product
 */

#include "terms.h"

/*
 * class Multi
 *
 * Can be a multiplicand or a multiplier.
 */

//constructor
Multi::Multi(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
    : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
{
    Multi::newValue();
    Multi::drawValue();
}
//end of constructor

void Multi::newValue()
{
    value_ = random(MIN_MULTIPLICAND, MAX_MULTIPLICAND + 1); //random number between min and max-1.
}

void Multi::drawValue()
{
    drawBg(BG_COLOUR); //overwrite background to clear previous text
    lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
    lcd_->setTextColor(fgColour_);
    char buf[MAX_DATA_LEN + 1]; //add 1 to allow for null
    sprintf(buf, "%3d", value_);
    lcd_->print(buf);
}

int16_t Multi::getValue()
{
    return value_;
}

void Multi::setValue(int16_t v)
{
    value_ = v;
}

/*
 * class Operator
 *
 * genarates and displays one of the operators "+', '-', '*' ,'/', %.
 */

//constructor
Operator::Operator(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
    : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
{
    newOper();
    Operator::drawValue();
}
//end of constructor

void Operator::newOper()
{
    oper_ = operators[random(0, NUMBER_OPERATORS_IN_USE)];
}

char Operator::getOper()
{
    return oper_;
}

void Operator::drawValue()
{
    drawBg(BG_COLOUR_OPER); //overwrite background to clear previous text
    lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
    lcd_->setTextColor(fgColour_);
    lcd_->print(oper_);
}

/*
 * class Equal
 *
 * displays an equal sign
 */

//constructor
Equals::Equals(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
    : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
{
    Equals::drawValue();
}
//end of constructor

void Equals::drawValue()
{
    lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
    lcd_->setTextColor(fgColour_);
    lcd_->print('=');
}

/*
 * class Product
 *
 * Displays the entered value
 * In Product constructor does not display the value as nothing entered on startup
 */

//constructor
Product::Product(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
    : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
{
}
//end of constructor

/*
 * drawValue()
 */
void Product::drawValue()
{
    lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
    lcd_->setTextColor(fgColour_);
    char buf[MAX_DATA_LEN + 1]; //add 1 to allow for null
    sprintf(buf, "%4d", value_);
    lcd_->print(buf);
}

/*
 * drawValue(*char buf)
 */
void Product::drawValue(char *buf)
{
    lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
    lcd_->setTextColor(fgColour_);
    lcd_->print(buf);
}

/*
 * get and put functions
 */

int16_t Product::getValue()
{
    return value_;
}

void Product::setValue(int16_t v)
{
    value_ = v;
}
