/*
 * Graphic class
 *
 * Base graphic class for terms in any of the equations
 */

#ifndef Graphic_h
#define Graphic_h

#include "maths.h"

class Graphic
{
protected:
    int16_t x_;
    int16_t y_;
    int16_t width_;
    int16_t height_;
    uint16_t fgColour_;
    uint16_t bgColour_;
    uint16_t borderColour_;
    MCUFRIEND_kbv *lcd_;

public:
    //constructors
    Graphic();

    Graphic(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol);

    //end of constructors

    /*
     * draws a rectangle with a coloured border and separate infill or background colour
     */
protected:
    void draw();

    /*
     * draw with background colour
     * used to clear out old text
     */
public:
    void drawBg(uint16_t bgColour);

    /*
     * touched()
     *
     * Check if spot touched on screen was this item
     */
    bool touched(int16_t x, int16_t y);
};
#endif
