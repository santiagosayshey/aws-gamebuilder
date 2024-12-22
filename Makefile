all:
	g++ src/main.cpp -o game \
	-I./SFML/include \
	-L./SFML/lib \
	-lsfml-graphics -lsfml-window -lsfml-system \
	-Wl,-rpath,./SFML/lib