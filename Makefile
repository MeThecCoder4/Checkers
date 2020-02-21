CC = g++
DEPS = minimaxcheckers.h minimax.h
SOURCES = main.cpp minimaxcheckers.cpp tests/minimaxcheckerstest.cpp
OBJS = main.o minimaxcheckers.o minimaxcheckerstest.o

main: $(OBJS)
	$(CC) -o main $(OBJS)

$(OBJS): $(SOURCES) $(DEPS)
	$(CC) -c -g $(SOURCES) $(DEPS)

clean:
	rm *.gch *.o main