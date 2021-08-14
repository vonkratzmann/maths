/*
 * class for touch
 *
 * Results came from calibration program.
 * For landscape had to swap x and y and the calibration values.
 * 
 * Note from calibration program:
 * most mcufriend have touch (with icons) that extends below the TFT
 * screens without icons need to reserve a space for "erase"
 * scale the ADC values from ts.getPoint() to screen values e.g. 0-239
 */

#ifndef Touch_h
#define Touch_h
#include <TouchScreen.h>

#include "maths.h"

#define MINPRESSURE 200
#define MAXPRESSURE 1000

/*
 * class touch
 */
class Touch
{
    const int XP = 8, XM = A2, YP = A3, YM = 9;
#if ORIENTATION == PORTRAIT
    const int TS_LEFT = 173, TS_RT = 913 TS_TOP = 148, TS_BOT = 913; //check values as not tested
#else
    const int TS_LEFT = 148, TS_RT = 913, TS_TOP = 920, TS_BOT = 155;
#endif
    TSPoint tp;
    uint16_t xpos, ypos; //screen coordinates
    TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
    bool previousStatus, currentStatus;

public:
    //constructor
    Touch()
    {
    }
    //end of constructor

    /*
     * isThereATouch()
     *
     * Ensure before returning saying there is a touch,
     * it has gone from no touch to touch.
     */

    bool isThereATouch(void)
    {
        currentStatus = waitStableTouchOrNoTouch();
        if (previousStatus == currentStatus)
        {
            return false;
        }
        previousStatus = currentStatus;
        return currentStatus;
    }

    /*
     * waitStableTouchOrNoTouch()
     *
     * touch or no touch has to be stable for 20ms
     */
    bool waitStableTouchOrNoTouch(void)

    {
        int count = 0;
        bool state, oldstate;
        while (count < 4)
        {
            readResistiveTouch();
            state = tp.z > MINPRESSURE;
            if (state == oldstate)
                count++;
            else
                count = 0;
            oldstate = state;
            delay(5);
        }
        return oldstate;
    }

    /*
     * readResistiveTouch()
     */
    void readResistiveTouch(void)
    {
        tp = ts.getPoint();
        pinMode(YP, OUTPUT); //restore shared pins
        pinMode(XM, OUTPUT);
    }

    /*
     * getTouchPoint()
     */
    void getTouchPoint(int16_t *x, int16_t *y)
    {
#if ORIENTATION == PORTRAIT
        *x = map(tp.x, TS_LEFT, TS_RT, 0, WIDTH);
        *y = map(tp.y, TS_TOP, TS_BOT, 0, HEIGHT);
#else
        // For landscape had to swap x and y and the calibration values.
        *y = map(tp.x, TS_RT, TS_LEFT, 0, tft.height());
        *x = map(tp.y, TS_BOT, TS_TOP, 0, tft.width());
#endif
    }
};
#endif