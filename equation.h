/*
 * Equation class
 *
 * Holder for all the terms of an equation ie:
 * multiplicand, operator, multiplier, equals, product.
 * Handles selection of an equation product field, calculation of the result,
 * and displaying the results.
 */

#ifndef Equation_h
#define Equation_h

#include <arduino.h>
#include "maths.h"
#include "terms.h"

extern bool overRide;
extern char operators[5];

class Equation
{
    int8_t state_ = PRODUCT_UNSELECTED; //valid states PRODUCT_SELECTED, PRODUCT_UNSELECTED, PRODUCT_INCORRECT, PRODUCT_CORRECT.
    String buf_;

public:
    Multi *multiplicand_;
    Operator *operator_;
    Multi *multiplier_;
    Equals *equals_;
    Product *product_;

    /*
     * calculateAnswer()
     */
    int16_t calculateAnswer();

    /*
     * processProductTouch()
     *
     * product term has been touched
     * select the field so numbers can be entered
     * if already selected or a correct number has been entered, ignore
     */
    void processProductTouch();

    /*
     * processNumber()
     *
     * Adds what just been touched, to previously numerics stored in buf_,
     * handles enter case, 'e' to process numbers previously entered ,
     * handles backspace case 'b',
     * anything else, assumes it is numeric and adds to the buffer.
     */
    void processNumber(char c);

    /*
     * get and set functions
     */

    int8_t getState();

    void setState(int8_t s);
};

#endif
