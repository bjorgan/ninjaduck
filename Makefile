ARGS = -g
SDL_CFLAGS := $(shell sdl-config --cflags) 
SDL_LDFLAGS := $(shell sdl-config --libs)

all: ninjaduck

ninjaduck: main.o sprites.o game.o scenes.o
	g++ main.o sprites.o game.o scenes.o -o ninjaduck $(SDL_CFLAGS) $(SDL_LDFLAGS) -lSDL_mixer

main.o: main.cpp
	g++ -c $(ARGS) main.cpp $(SDL_CFLAGS) $(SDL_LDFLAGS) -lSDL_mixer

sprites.o: sprites.cpp
	g++ -c $(ARGS) sprites.cpp -lSDL $(SDL_CFLAGS) $(SDL_LDFLAGS) -lSDL_mixer

game.o: game.cpp
	g++ -c $(ARGS) game.cpp $(SDL_CFLAGS) $(SDL_LDFLAGS) -lSDL_mixer

scenes.o: scenes.cpp
	g++ -c $(ARGS) scenes.cpp $(SDL_CFLAGS) $(SDL_LDFLAGS) -lSDL_mixer

clean:
	rm *.o
