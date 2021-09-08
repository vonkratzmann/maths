
@echo off
"C:/Program Files (x86)/arduino-1.8.15/hardware/tools/avr/bin/avrdude" -C"C:/Program Files (x86)/arduino-1.8.15/hardware/tools/avr/etc/avrdude.conf" -patmega328p -cstk500 -PCOM4 -Uflash:w:"C:/Users/vonkr/Documents/kirk/maths/lowpowertest/ArduinoOutput/lowpowertest.ino.hex":i
