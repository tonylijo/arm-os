NAME   = debounce
PATH = /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/tonylijo/bin/
CC      = arm-none-eabi-gcc

LD      = arm-none-eabi-ld
AR      = arm-none-eabi-ar
AS      = arm-none-eabi-as
CP      = arm-none-eabi-objcopy
OD      = arm-none-eabi-objdump

CFLAGS  = -mcpu=arm7tdmi -Ic:/gccfd/yagarto/arm-elf/include -c -fno-common -O0 -g
AFLAGS  = -ahls -mapcs-32 -o bootload.o
LFLAGS= -mcpu=arm7tdmi -Wl,-Map,main.map,-Tlink.ld -nostdlib
LINK_LIBS= -lc -lm -lg -lgcc
CPFLAGS = -O binary
HEXFLAGS = -O ihex
ODFLAGS   = -x --syms

all: test

clean:
	-rm bootload.lst bootload.o main.o main.out main.map main.dmp main.bin VIClowlevel.o schedule.o isr.o

test: main.out
	$(CP) $(CPFLAGS) main.out main.bin
	$(OD) $(ODFLAGS) main.out > main.dmp
	$(CP) $(HEXFLAGS) main.out main.hex

main.out: bootload.o VIClowlevel.o main.o schedule.o isr.o link.ld
	$(CC) $(LFLAGS) -o main.out bootload.o VIClowlevel.o main.o schedule.o isr.o $(LINK_LIBS)

bootload.o: bootload.s main.o
	$(AS) $(AFLAGS) bootload.s > bootload.lst

VIClowlevel.o: VIClowlevel.c
	$(CC) $(CFLAGS) VIClowlevel.c


main.o: main.c schedule.c isr.c
	$(CC) $(CFLAGS) main.c schedule.c isr.c

lpc: lpc21isp.c
	gcc lpc21isp.c -o lpc

burn:	main.hex lpc
	./lpc main.hex /dev/ttyUSB0 38400 1200
