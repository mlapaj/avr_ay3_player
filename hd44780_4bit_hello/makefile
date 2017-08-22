CC=avr-gcc
CFLAGS=-g -Os -mmcu=atmega8
main.elf: main.c
		$(CC) $(CFLAGS) main.c -o main.elf
		avr-objdump -h -S main.elf > main.lst
		avr-objcopy -j .text -j .data -O ihex main.elf main.hex
clean:
	rm -f main.elf main.o main.lst main.hex

program: main.hex
	avrdude -c usbasp -p m8 -B5
	avrdude -c usbasp -p m8 -B5 -e -U flash:w:main.hex
