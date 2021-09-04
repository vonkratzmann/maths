#include <arduino.h>

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
