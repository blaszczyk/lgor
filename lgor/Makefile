CC=g++
CFLAGS=-c -Wall
LDFLAGS= -lpthread
SOURCES=main.cpp A19Models.cpp Bunch.cpp Complex.cpp Field.cpp Lgor.cpp Matrix.cpp Model.cpp ModelData.cpp ModelSum.cpp Monomial.cpp Parseline.cpp Polynomial.cpp PolySpace.cpp Sector.cpp SField.cpp SimpleMath.cpp SpectrumSum.cpp SuperMonomial.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=lgor

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run:
	./$(EXECUTABLE)

clean:
	rm *.o $(EXECUTABLE)

init:
	rm ModelData.o $(EXECUTABLE)
