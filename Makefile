all:
	g++ src/*.c -o kart -Iinclude -lSDL2 -lSDL2_image -lSDL2_ttf -lm -O3 -march=native -flto -pipe -Wall -Wextra -Werror -pedantic -ansi -fno-exceptions -fno-rtti
