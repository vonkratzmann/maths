/*
 * class for touch
 *
 * Results came from calibration program.
 * For landscape had to swap x and y and the calibration values.
 */

#include "touch.h"

//constructor
Touch::Touch()
{
}

/*
 * isThereATouch()
 *
 * Ensure before returning saying there is a touch,
 * it has gone from no touch to touch.
 */

bool Touch::isThereATouch(void)
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
bool Touch::waitStableTouchOrNoTouch(void)

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
void Touch::readResistiveTouch(void)
{
    tp = ts.getPoint();
    pinMode(YP, OUTPUT); //restore shared pins
    pinMode(XM, OUTPUT);
}

/*
 * getTouchPoint()
*/
void Touch::getTouchPoint(int16_t *x, int16_t *y)
{
#if ORIENTATION == PORTRAIT
    *x = map(tp.x, TS_LEFT, TS_RT, 0, WIDTH);
    *y = map(tp.y, TS_TOP, TS_BOT, 0, HEIGHT);
#else
    // For landscape had to swap x and y and the calibration values.
    *y = map(tp.x, TS_RT, TS_LEFT, 0, HEIGHT); //kk check
    *x = map(tp.y, TS_BOT, TS_TOP, 0, WIDTH);  //kk check
#endif
}
