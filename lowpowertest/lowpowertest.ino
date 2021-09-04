#include "LowPower.h"

// Use pin 2 as wake up pin
const int wakeUpPin = PC0;
const int blinkPin = PB0;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup()
{
    // Configure wake up pin as input.
    // This will consumes few uA of current.
    pinMode(wakeUpPin, INPUT);      //pin 23
}

void loop()
{
    // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, LOW);

    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0);

    // Do something here

    pinMode(blinkPin, OUTPUT);
    while (1)

    {
        digitalWrite(blinkPin, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(1000);                     // wait for a second
        digitalWrite(blinkPin, LOW);  // turn the LED off by making the voltage LOW
        delay(1000);                     // wait for a second
    }
}
