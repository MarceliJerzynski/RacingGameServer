LIBS=-lGL -lglfw -lGLEW
HEADERS=Car.h  Markup.h  Object.h
FILES=Car.cpp  Markup.cpp  Object.cpp Server.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o Server $(FILES)  $(LIBS) -I. -lpthread
