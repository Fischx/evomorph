CC=g++
CFLAGS=-c -Wall `sdl-config --cflags ` -ggdb
LDFLAGS= -lode -lGL -lGLU `sdl-config --cflags --libs`
SOURCES= body.cpp events.cpp evolution.cpp graphics.cpp neuralnet.cpp simulate.cpp substratum.cpp vector2.cpp vector3.cpp genotype.cpp micro_evo.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=micro

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	@$(CC) $(CFLAGS) $< -o $@


clean:
	@echo Cleaning up...
	@rm micro *.o
	@echo Done.
