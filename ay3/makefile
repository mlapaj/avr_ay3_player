CC=avr-gcc
CFLAGS=-g -Os -mmcu=atmega8
hello.exe: hello.c
		$(CC) $(CFLAGS) hello.c hd44780.c ay3_89xx.c -o hello.elf
		avr-objdump -h -S hello.elf > helo.lst
		avr-objcopy -j .text -j .data -O ihex hello.elf hello.hex
clean:
	rm hello.elf

program: hello.hex
	avrdude -c usbasp -p m8 -B5
	avrdude -c usbasp -p m8 -B5 -e -U flash:w:hello.hex
