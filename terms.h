/*
 * classes for multiplicand, multiplier, operator and product
 */
#ifndef Terms_h
#define Terms_h

#include "maths.h"
#include "graphic.h"

/*
 * class Multi
 *
 * Can be a multiplicand or a multiplier.
 */
class Multi : Graphic
{
    int16_t value_;

public:
    //constructor
    Multi(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
        : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
    {
        Multi::newValue();
        Multi::drawValue();
    }
    //end of constructor

public:
    void newValue()
    {
        value_ = random(MIN_MULTIPLICAND, MAX_MULTIPLICAND + 1); //random number between min and max-1.
    }

    void drawValue()
    {
        drawBg(BG_COLOUR); //overwrite background to clear previous text
        lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
        lcd_->setTextColor(fgColour_);
        char buf[MAX_DATA_LEN + 1]; //add 1 to allow for null
        sprintf(buf, "%3d", value_);
        lcd_->print(buf);
    }

    int16_t getValue()
    {
        return value_;
    }

    void setValue(int16_t v)
    {
        value_ = v;
    }
};

/*
 * class Operator
 *
 * genarates and displays one of the operators "+', '-', '*' ,'/', %.
 */
class Operator : Graphic
{
    char oper_;

public:
    //constructor
    Operator(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
        : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
    {
        newOper();
        Operator::drawValue();
    }
    //end of constructor

    void newOper()
    {
        oper_ = operators[random(0, 5)];
    }

    char getOper()
    {
        return oper_;
    }

    void drawValue()
    {
        drawBg(BG_COLOUR_OPER); //overwrite background to clear previous text
        lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
        lcd_->setTextColor(fgColour_);
        lcd_->print(oper_);
    }
};

/*
 * class Equal
 *
 * displays an equal sign
 */
class Equals : Graphic
{
public:
    //constructor
    Equals(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
        : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
    {
        Equals::drawValue();
    }
    //end of constructor

private:
    void drawValue()
    {
        lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
        lcd_->setTextColor(fgColour_);
        lcd_->print('=');
    }
};

/*
 * class Product
 *
 * Displays the entered value
 * In conbstructor does not display the value as nothing enetred on startup
 */
class Product : public Graphic
{
    int16_t value_ = 0;

public:
    //constructor
    Product(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
        : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
    {
    }
    //end of constructor

    /*
     * drawValue()
     */
    void drawValue()
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
    void drawValue(char *buf)
    {
        lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
        lcd_->setTextColor(fgColour_);
        lcd_->print(buf);
    }

    /*
     * get and put functions
     */

    int16_t getValue()
    {
        return value_;
    }

    void setValue(int16_t v)
    {
        value_ = v;
    }
};

#endif
