allocate: allocate.o list.o utils.o cpu.o
	gcc -g -Wall -o allocate allocate.o list.o utils.o cpu.o

allocate.o: allocate.c 
	gcc -g -Wall -c allocate.c 

list.o: list.c list.h
	gcc -g -Wall -c list.c 

utils.o: utils.c utils.h
	gcc -g -Wall -c utils.c

cpu.o: cpu.c cpu.h
	gcc -g -Wall -c cpu.c

clean:
	$(RM) *.o
	$(RM) allocate