CC = g++
DEPS = minimaxcheckers.h minimax.h game.h board.h figure.h pawn.h crownhead.h clickablerect.h
SOURCES = main.cpp minimaxcheckers.cpp game.cpp tests/minimaxcheckerstest.cpp board.cpp figure.cpp pawn.cpp crownhead.cpp clickablerect.cpp tests/pawntest.cpp
OBJS = main.o minimaxcheckers.o minimaxcheckerstest.o game.o board.o figure.o pawn.o crownhead.o clickablerect.o pawntest.o
EXEC_FILE = checkers

$(EXEC_FILE): $(OBJS)
	$(CC) -o $(EXEC_FILE) $(OBJS) -lsfml-graphics -lsfml-window -lsfml-system

$(OBJS): $(SOURCES) $(DEPS)
	$(CC) -c -g $(SOURCES) $(DEPS)

clean:
	rm *.gch *.o $(EXEC_FILE)