CC=avr-gcc
CFLAGS=-g -Os -mmcu=atmega328p
main.hex: main.c
		$(CC) $(CFLAGS) main.c uart.c debug.c -o main.elf
		avr-objdump -h -S main.elf > main.lst
		avr-objcopy -j .text -j .data -O ihex main.elf main.hex
clean:
	rm -f main.elf main.o main.lst main.hex

program: main.hex
	avrdude -c arduino -p atmega328p -P /dev/ttyUSB0  -b 57600
	avrdude -c arduino -p atmega328p -P /dev/ttyUSB0 -b 57600 -e -U flash:w:main.hex
