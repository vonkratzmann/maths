/*
 * Classes for number and command
 * These are displayed on the screen and the user selects these to fill in the product field,
 * or execute a command.
 * Numbers are '0' to 9 and '-' and user touches these to enter an answer.
 * Commands are "Ent" for enter, "Bck" for backspace, "New" for new game.
 * For each individual number and command a new instance of the class is defined.
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
    Number(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol, char chr);

private:
    void draw();
};

/*
 * class Command
 */
class Command : public Graphic
{
    char *s_;

public:
    //constructor
    Command(MCUFRIEND_kbv *screen, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgCol, uint16_t bgCol, uint16_t borderCol, char *s);

private:
    void draw();
};
#endif
