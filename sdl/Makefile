SDL_OBJ=color.o font.o pen.o surface.o brush.o dc.o framerate.o draw.o size.o rect.o point.o
CXX = g++
CXXFLAGS = -O3 -lSDL -lSDL_ttf -I/SDL 

.SUFFIXES: .o .cpp
sdl: ../sdl.a($(SDL_OBJ))

sdlclean:
	@rm -f ../$(SDL).a
