all: compile run

compile:
	g++ src/main.cpp -o game \
	-I./SFML/include \
	-L./SFML/lib \
	-lsfml-graphics -lsfml-window -lsfml-system \
	-Wl,-rpath,./SFML/lib

run:
	./game

clean:
	rm -f game