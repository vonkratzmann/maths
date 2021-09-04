#include <Arduino.h>
#line 1 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino"
#include <arduino.h>

#line 3 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino"
void setup();
#line 8 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino"
void loop();
#line 3 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino"
void setup()
{
	DDRB |= 0B0000001;
}

void loop()
{
	// put your main code here, to run repeatedly:

	{
		PORTB |= 0B00000001;
		_delay_ms(250);
		PORTB &= 0B11111110;
		_delay_ms(250);
	}
}

