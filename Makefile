TARGET = main.exe

all:
	g++ -Isrc/Include -Lsrc/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2
	$(TARGET) run

run $(TARGET):
	.\$(TARGET)