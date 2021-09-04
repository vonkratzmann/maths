# 1 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino"
# 2 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino" 2

void setup()
{
 
# 5 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino" 3
(*(volatile uint8_t *)((0x04) + 0x20)) 
# 5 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino"
     |= 0B0000001;
}

void loop()
{
 // put your main code here, to run repeatedly:

 {
  
# 13 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 13 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino"
       |= 0B00000001;
  _delay_ms(250);
  
# 15 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 15 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\blink\\blink.ino"
       &= 0B11111110;
  _delay_ms(250);
 }
}
