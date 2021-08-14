/*
 * Classes for number and command
 * These are displayed on the screen and the user selects these to fill in the product field,
 * or execute a command.
 * Numbers are '0' to 9 and '-' and user touches these to enter an answer.
 * Commands are "Ent" for enter, "Bck" for backspace, "New" for new game.
 * For each number and command a new instance of the class is defined.
 */
#ifndef NumberCommand_h
#define NumberCommand_h

#include "maths.h"
#include "graphic.h"

/*
 * class Number
 */
class Number : public Graphic
{
    char char_;

public:
    //constructor
    Number(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol, char chr)
        : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
    {
        char_ = chr;
        Number::draw();
    }
    //end of constructor

private:
    void draw()
    {
        lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
        lcd_->setTextColor(fgColour_);
        lcd_->print(char_);
    }
};

/*
 * class Command
 */
class Command : public Graphic
{
    char *s_;

public:
    //constructor
    Command(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol, char *s)
        : Graphic{screen, x, y, w, h, fgCol, bgCol, borderCol}
    {
        s_ = s;
        Command::draw();
    }
    //end of constructor

private:
    void draw()
    {
        lcd_->setCursor(x_, y_ + FONT_HEIGHT - FONT_VERT_CORRECTION);
        lcd_->setTextColor(fgColour_);
        lcd_->print(s_);
    }
};
#endif
