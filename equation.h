/*
 * Equation class
 *
 * Holder for all the terms of an eauation ie:
 * multiplicand, operator, multiplier, equals, product.
 * Handles selction of an equation product field, calculation of the result,
 * and displaying the results.
 */

#ifndef Equation_h
#define Equation_h

#include "maths.h"

class Equation
{
    int8_t state_ = PRODUCT_UNSELECTED; //ie product is correct, incorrect, selected
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
    int16_t calculateAnswer()
    {
        int16_t multiplicand = multiplicand_->getValue();
        int16_t multiplier = multiplier_->getValue();
        int16_t result;
        char oper = operator_->getOper();

        if (oper == '+')
        {
            result = multiplicand + multiplier;
        }
        else if (oper == '-')
        {
            result = multiplicand - multiplier;
        }
        else if (oper == '*')
        {
            result = multiplicand * multiplier;
        }
        else if (oper == '/')
        {
            result = multiplicand / multiplier;
        }
        else if (oper == '%')
        {
            result = multiplicand % multiplier;
        }
        return result;
    }

    /*
     * processTouch()
     *
     * product term has been touched
     * select the field so numbers can be entered
     * if already selected or a correct number has been entered, ignore
     */
    void processTouch()
    {
        switch (state_)
        {
        case PRODUCT_UNSELECTED:
            state_ = PRODUCT_SELECTED;
            buf_.remove(0); //clear buffer from any previous attempts
            product_->drawBg(PRODUCT_SELECTED_COLOUR);
            break;

        case PRODUCT_SELECTED:
            //do nothing as already selected
            break;

        case PRODUCT_INCORRECT:
            //assume they want to try again
            state_ = PRODUCT_SELECTED;
            product_->drawBg(PRODUCT_SELECTED_COLOUR);
            buf_.remove(0);
            break;

        case PRODUCT_CORRECT:
            //do nothing as value previously entered is correct
            break;
        }
    }

    /*
     * processNumber()
     * adds what just been touched, to previously numerics stored in buf_
     * handles enter, case 'e' to process numbers previouly entered  
     * handles backspace, case 'b'
     * anything else, assumes it is numeric and adds to the buffer.
     */
    void processNumber(char c)
    {
        int i;
        product_->drawBg(PRODUCT_SELECTED_COLOUR);
        switch (c)
        {
        case 'e':
            product_->setValue(buf_.toInt());
            if (overRide || calculateAnswer() == product_->getValue())
            {
                product_->drawBg(PRODUCT_CORRECT_COLOUR);
                state_ = PRODUCT_CORRECT;
                product_->setValue(calculateAnswer());
                product_->drawValue();
            }
            else
            {
                product_->drawBg(PRODUCT_INCORRECT_COLOUR);
                state_ = PRODUCT_INCORRECT;
                product_->drawValue();
            }
            break;

        case 'b':
            if (buf_.length() != 0)
            {
                buf_.remove(buf_.length() - 1);
                char tmp[MAX_DATA_LEN + 1]; //+ 1 to allow for null
                buf_.toCharArray(tmp, MAX_DATA_LEN + 1);
                product_->drawValue(tmp);
            }
            break;

        default:
            //covers numeric numbers and '-'
            if (buf_.length() + 1 <= MAX_DATA_LEN)
            {
                buf_.concat(c);
            }
            if (buf_.equals(String(OVER_RIDE_CODE)))
            {
                overRide = true;
                product_->drawBg(BG_COLOUR);
                buf_.remove(0);
                return;
            }
            if (buf_.equals(String(OVER_RIDE_CLEAR)))
            {
                overRide = false;
                product_->drawBg(BG_COLOUR);
                buf_.remove(0);
                return;
            }
            char tmp[MAX_DATA_LEN + 1]; //+ 1 to allow for null
            buf_.toCharArray(tmp, MAX_DATA_LEN + 1);
            product_->drawValue(tmp);
            break;
        }
    }

    /*
     * get and set functions
     */

    int8_t getState()
    {
        return state_;
    }

    void setState(int8_t s)
    {
        state_ = s;
    }
};

#endif
