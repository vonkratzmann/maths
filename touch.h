/*
 * class for touch
 *
 * Results came from calibration program.
 * For landscape had to swap x and y and the calibration values.
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
    Touch();

    /*
     * isThereATouch()
     *
     * Ensure before returning saying there is a touch,
     * it has gone from no touch to touch.
     */

    bool isThereATouch(void);

    /*
     * waitStableTouchOrNoTouch()
     *
     * touch or no touch has to be stable for 20ms
     */
    bool waitStableTouchOrNoTouch(void);

    /*
     * readResistiveTouch()
     */
    void readResistiveTouch(void);

    /*
     * getTouchPoint()
     */
    void getTouchPoint(int16_t *x, int16_t *y);
};

#endif