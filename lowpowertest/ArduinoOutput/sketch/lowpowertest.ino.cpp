#include <Arduino.h>
#line 1 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
#include "LowPower.h"
#include <avr/sleep.h>

//PortC 0 pin 23 on ATMega328P, A0, PCINT8
#define WAKE_UP_PIN PORTC0
#define WAKE_UP_PORT PORTC
#define WAKE_UP_DDR DDRC

//PortB 0 pin 14 on ATMega328P
#define BLINK_PIN PORTB0
#define BLINK_PORT PORTB
#define BLINK_DDR DDRB

bool volatile flashFast = true;

#line 16 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
void setup();
#line 37 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
void loop();
#line 16 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
void setup()
{
    BLINK_DDR |= (1 << BLINK_PIN); // set as output

    // Configure wake up pin as input.
    // This will consume a few uA of current.
    WAKE_UP_DDR &= ~(1 << WAKE_UP_PIN);
    WAKE_UP_PORT |= (1 << WAKE_UP_PIN);

    cli();
    PCICR |= B00000010;
    PCMSK1 |= B00000001; //interrupt on A0, PCINT8
    sei();
    asm("nop \n");
}

ISR(PCINT1_vect)
{
    flashFast = false;
}

void loop()
{
    if (flashFast)
    {
        delay(250);
        PORTB &= ~(1 << BLINK_PIN); //set it low
        delay(250);
        PORTB |= (1 << BLINK_PIN); //set it high
    }
    else
    {
        delay(1000);
        PORTB &= ~(1 << BLINK_PIN); //set it low
        delay(1000);
        PORTB |= (1 << BLINK_PIN); //set it high
    }
}
