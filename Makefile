CC = g++
DEPS = minimaxcheckers.h minimax.h game.h board.h
SOURCES = main.cpp minimaxcheckers.cpp game.cpp tests/minimaxcheckerstest.cpp board.cpp
OBJS = main.o minimaxcheckers.o minimaxcheckerstest.o game.o board.o

main: $(OBJS)
	$(CC) -o main $(OBJS) -lsfml-graphics -lsfml-window -lsfml-system

$(OBJS): $(SOURCES) $(DEPS)
	$(CC) -c -g $(SOURCES) $(DEPS)

clean:
	rm *.gch *.o main