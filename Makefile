CC = g++
DEPS = minimaxcheckers.h minimax.h game.h board.h figure.h clickablerect.h
SOURCES = main.cpp minimaxcheckers.cpp game.cpp tests/minimaxcheckerstest.cpp board.cpp figure.cpp clickablerect.cpp
OBJS = main.o minimaxcheckers.o minimaxcheckerstest.o game.o board.o figure.o clickablerect.o
EXEC_FILE = checkers

$(EXEC_FILE): $(OBJS)
	$(CC) -Wall -o $(EXEC_FILE) $(OBJS) -lsfml-graphics -lsfml-window -lsfml-system

$(OBJS): $(SOURCES) $(DEPS)
	$(CC) -c -g $(SOURCES) $(DEPS)

clean:
	rm *.gch *.o $(EXEC_FILE)