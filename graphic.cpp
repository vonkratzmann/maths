/*
 * Graphic class
 *
 * Base graphic class for terms in any of the equations
 */

#include "graphic.h"

//constructors
Graphic::Graphic()
{
}

Graphic::Graphic(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol)
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
 * draws a rectangle with a coloured border and separate infill or background colour
 */

void Graphic::draw()
{
    lcd_->drawRect(x_, y_, width_, height_, borderColour_);
    lcd_->fillRect(x_ + 2, y_ + 2, width_ - 4, height_ - 4, bgColour_);
}

/*
* draw with background colour
* used to clear out old text
*/

void Graphic::drawBg(uint16_t bgColour)
{
    lcd_->fillRect(x_ + 2, y_ + 2, width_ - 4, height_ - 4, bgColour);
}

/*
* touched()
*
* Check if spot touched on screen was this item
*/
bool Graphic::touched(int16_t x, int16_t y)
{
#ifdef DEBUG_TOUCHED
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
