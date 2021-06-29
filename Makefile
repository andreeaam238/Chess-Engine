CC = g++
CFLAGS = -Wall -Wextra -g
EXE = engineTry
SRCS = engineTry.cpp engineTryTable.cpp engineTryPawn.cpp engineTryHorse.cpp engineTryKing.cpp engineTryBishop.cpp engineTryRook.cpp engineTryQueen.cpp alphaBetaAlgo.cpp

build:
	$(CC) $(CFLAGS) $(SRCS) -o $(EXE)

run:
	./$(EXE)

.PHONY: clean

clean:
	rm -f *.o xboard.debug table.txt buffer.txt