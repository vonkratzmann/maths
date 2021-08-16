/*
 * classes for multiplicand, multiplier, operator and product
 */
#ifndef Terms_h
#define Terms_h

#include <arduino.h>
#include "maths.h"
#include "graphic.h"

extern char operators[5];

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
    Multi(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol);

    void newValue();

    void drawValue();

    int16_t getValue();

    void setValue(int16_t v);
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
    Operator(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol);

    void newOper();

    char getOper();

    void drawValue();
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
    Equals(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol);

private:
    void drawValue();
};

/*
 * class Product
 *
 * Displays the entered value
 * In Product constructor does not display the value as nothing entered on startup
 */
class Product : public Graphic
{
    int16_t value_ = 0;

public:
    //constructor
    Product(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol);

    /*
     * drawValue()
     */
    void drawValue();

    /*
     * drawValue(*char buf)
     */
    void drawValue(char *buf);

    /*
     * get and put functions
     */

    int16_t getValue();

    void setValue(int16_t v);
};

#endif
