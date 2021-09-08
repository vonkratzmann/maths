# 1 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
# 2 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 2
# 3 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 2


# 4 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
//PortC 0 pin 23 on ATMega328P, A0, PCINT8




//PortB 0 pin 14 on ATMega328P




bool volatile flashFast = true;

void setup()
{
    
# 18 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 18 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
             |= (1 << 
# 18 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
                      0
# 18 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
                               ); // set as output

    // Configure wake up pin as input.
    // This will consume a few uA of current.
    
# 22 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
   (*(volatile uint8_t *)((0x07) + 0x20)) 
# 22 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
               &= ~(1 << 
# 22 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
                         0
# 22 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
                                    );
    
# 23 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
   (*(volatile uint8_t *)((0x08) + 0x20)) 
# 23 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
                |= (1 << 
# 23 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
                         0
# 23 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
                                    );

    
# 25 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
   __asm__ __volatile__ ("cli" ::: "memory")
# 25 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
        ;
    
# 26 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
   (*(volatile uint8_t *)(0x68)) 
# 26 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
         |= 2;
    
# 27 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
   (*(volatile uint8_t *)(0x6C)) 
# 27 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
          |= 1; //interrupt on A0, PCINT8
    
# 28 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
   __asm__ __volatile__ ("sei" ::: "memory")
# 28 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
        ;
    asm("nop \n");
}


# 32 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
extern "C" void __vector_4 /* Pin Change Interrupt Request 0 */ (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_4 /* Pin Change Interrupt Request 0 */ (void)

# 33 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
{
    flashFast = false;
}

void loop()
{
    if (flashFast)
    {
        delay(250);
        
# 42 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
       (*(volatile uint8_t *)((0x05) + 0x20)) 
# 42 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
             &= ~(1 << 
# 42 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
                       0
# 42 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
                                ); //set it low
        delay(250);
        
# 44 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
       (*(volatile uint8_t *)((0x05) + 0x20)) 
# 44 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
             |= (1 << 
# 44 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
                      0
# 44 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
                               ); //set it high
    }
    else
    {
        delay(1000);
        
# 49 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
       (*(volatile uint8_t *)((0x05) + 0x20)) 
# 49 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
             &= ~(1 << 
# 49 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
                       0
# 49 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
                                ); //set it low
        delay(1000);
        
# 51 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
       (*(volatile uint8_t *)((0x05) + 0x20)) 
# 51 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
             |= (1 << 
# 51 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino" 3
                      0
# 51 "c:\\Users\\vonkr\\Documents\\kirk\\maths\\lowpowertest\\lowpowertest.ino"
                               ); //set it high
    }
}
