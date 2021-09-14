/*
 * Class Battery
 *
 * Handle operating from battery,
 *
 */

#ifndef Battery_h
#define Battery_h

#include "maths.h"
#include "LowPower.h"
#include <avr/sleep.h>
//Line below to stop IDE complaining about undefined values
#include "C:\Program Files (x86)\arduino-1.8.15\hardware\tools\avr\avr\include\avr\iom328p.h"

class Battery
{
public:
    void shutDown()
    {
        //configure wake up pin as input.
        //configure internal pullup.
        //this will consume a few uA of current.
        WAKE_UP_DDR &= ~(1 << WAKE_UP_PIN);
        WAKE_UP_PORT |= (1 << WAKE_UP_PIN);

        //enable interrupts on wakeup pin
        cli();
        PCICR |= B00000010;
        PCMSK1 |= B00000001; //interrupt on PCINT8, PortC0
        sei();

        //turn off analogue comparator
        cli();
        ACSR |= (1 << ACD);
        sei();

        //WDT is turned off in power down mode with SLEEP_FOREVER,
        //so need to specifically turn it off.
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    }

    void disableWakeUpInterrupts()
    {
        cli();
        PCMSK1 &= ~B00000001; //interrupt on PCINT8, PortC0
        sei();
    }
};

#endif
