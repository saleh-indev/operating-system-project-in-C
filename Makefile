# Makefile
EXEC = main
CC  = gcc
CFLAGS = -Wall -Wextra -Werror 
OBJ = main.o


$(EXEC) : ${OBJ}
	$(CC) $(CFLAGS) ${OBJ} -o ${EXEC}
	

clean : 
	rm *.o

test : 
	./main fichierTest




