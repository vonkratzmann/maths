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
    Graphic()
    {
    }

    Graphic(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
    {
        lcd_ = screen;
        x_ = x;
        y_ = y;
        width_ = w;
        height_ = h;
        fgColour_ = fgCol;
        bgColour_ = bgCol;
        borderColour_ = borderCol;
        draw();
    }
    //end of constructors

    /*
     * draws a rectangle with a coloured border and seperate infill or background colour
     */
protected:
    void draw()
    {
        lcd_->drawRect(x_, y_, width_, height_, borderColour_);
        lcd_->fillRect(x_ + 2, y_ + 2, width_ - 4, height_ - 4, bgColour_);
    }

    /*
     * draw with background colour
     * used to clear out old text
     */
public:
    void drawBg(uint16_t bgColour)
    {
        lcd_->fillRect(x_ + 2, y_ + 2, width_ - 4, height_ - 4, bgColour);
    }

    /*
     * touched()
     *
     * Check if spot touched on screen was this item
     */
    bool touched(int16_t x, int16_t y)
    {
#ifdef DEBUG
        Serial.println("Touch ");
        Serial.print("x: ");
        Serial.print(x);
        Serial.print("  y: ");
        Serial.println(y);

        Serial.println("class ");
        Serial.print("x_: ");
        Serial.print(x_);
        Serial.print("  ");
        Serial.println(x_ + width_);
        Serial.print("y_: ");
        Serial.print(y_);
        Serial.print("  ");
        Serial.println(y_ + height_);
#endif
        return (x >= x_ && x <= (x_ + width_) && y >= y_ && y <= (y_ + height_));
    }
};
#endif
