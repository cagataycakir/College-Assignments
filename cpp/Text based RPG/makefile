out: main.o creature.o
	g++ -o out main.o creature.o

main.o: main.cpp
	g++ -c main.cpp

creature.o: creature.cpp
	g++ -c creature.cpp

clean:
	rm out main.o creature.o