CC = g++ -g -Wall -O0 -m32
CFLAG = -c -fkeep-inline-functions
LDFLAG = -lSDL -lpthread

BIN  = emu
OBJ = $(patsubst %.cpp,%.o,$(wildcard *.cpp)) 

all : $(BIN)
$(BIN) : ${OBJ}
	$(CC) $^ -o $@ $(LDFLAG)

$(OBJ) : %.o : %.cpp
	$(CC) $(CFLAG) $< -o $@

clean :
	rm -f *.o $(BIN) core
