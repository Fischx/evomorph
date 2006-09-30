CC=g++
CFLAGS=-c -Wall `sdl-config --cflags ` -O3
# -ggdb

LDFLAGS= -lode #-lGL -lGLU `sdl-config --cflags --libs`
LDFLAGSV= -lode -lGL -lGLU `sdl-config --cflags --libs`
SOURCES= body.cpp evolution.cpp neuralnet.cpp simulate.cpp substratum.cpp vector2.cpp vector3.cpp genotype.cpp micro_evo.cpp

SOURCESV= body.cpp gui.cpp evolution.cpp neuralnet.cpp simulate.cpp substratum.cpp vector2.cpp vector3.cpp genotype.cpp evomorph_view.cpp


OBJECTSV=$(SOURCESV:.cpp=.o)

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=micro

EXECUTABLEV=evomorph_view


all: $(SOURCES) $(EXECUTABLE) $(SOURCESV) $(EXECUTABLEV)

$(EXECUTABLE): $(OBJECTS) 
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(EXECUTABLEV): $(OBJECTSV) 
	@$(CC) $(LDFLAGSV) $(OBJECTSV) -o $@


.cpp.o:
	@$(CC) $(CFLAGS) $< -o $@


clean:
	@echo Cleaning up...
	@rm micro *.o
	@echo Done.
