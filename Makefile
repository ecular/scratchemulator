CC = g++ -g -Wall -O0
CFLAG = -c -fkeep-inline-functions
LDFLAG = 

BIN  = emu
OBJ = $(patsubst %.cpp,%.o,$(wildcard *.cpp)) 

all : $(BIN)
$(BIN) : ${OBJ}
	$(CC) $^ -o $@ $(LDFLAG)

$(OBJ) : %.o : %.cpp
	$(CC) $(CFLAG) $< -o $@

clean :
	rm -f *.o $(BIN) core
