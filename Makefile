CC = g++
cflag = -g -Wall -c -fkeep-inline-functions
main : main.o cpu.o
	$(CC) main.o cpu.o -o main

main.o : main.cpp cpu.h
	$(CC) $(cflag) main.cpp cpu.h

cpu.o : cpu.cpp cpu.h
	$(CC) $(cflag) cpu.cpp cpu.h
	
clean:
	rm -f *.o *.gch main
