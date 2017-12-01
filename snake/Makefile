# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:snake.elf

#additional rules for files
snake.elf: ${COMMON_OBJECTS} snake.o  Apple.o Snake.o  wdt_handler.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd -lShape -lCircle -lp2sw

Snake.o: Snake.c Snake.h Apple.h
	${CC} ${CFLAGS} ${LDFLAGS} -lLcd -lShape -c Snake.c

Apple.o: Apple.c Apple.h
	${CC} ${CFLAGS} ${LDFLAGS} -lLcd -lShape -lCircle -c Apple.c

load: snake.elf
	mspdebug rf2500 "prog $^"

clean:
	rm -f *.o *.elf
