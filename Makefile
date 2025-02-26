TARGET = main.exe

CXX = g++
CXXFLAGS = -Isrc/Include -Lsrc/lib -mwindows
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp $(LIBS)
	$(TARGET) run

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)