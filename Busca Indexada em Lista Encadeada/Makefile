MAIN=main.c
BINARY=executavel

all:
	gcc -Wall -g lista.c $(MAIN) -o $(BINARY)

run:
	./$(BINARY)
	
debug:
	gcc -DDEBUG -Wall $(MAIN) lista.c -o $(BINARY)

valgrind:
	valgrind --tool=memcheck --leak-check=full  --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BINARY)

clean:
	@rm *.o
