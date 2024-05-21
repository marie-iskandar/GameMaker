CXX = clang++
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CFLAGS := -g -O0 $(SDL_CFLAGS)
LFLAGS := $(SDL_LDFLAGS)

TRY.exe: TRY.CPP VIDDRIVE.o
	$(CXX) $(CFLAGS) $(LFLAGS) -o TRY.exe TRY.CPP VIDDRIVE.o

VIDDRIVE.o: VIDDRIVE.HPP VIDDRVC.CPP
	$(CXX) $(CFLAGS) -c VIDDRVC.CPP -o VIDDRIVE.o

clean:
	rm *.o *.exe
