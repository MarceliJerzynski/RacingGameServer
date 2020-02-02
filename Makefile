LIBS=-lGL -lglfw -lGLEW
HEADERS= Vec3.h Car.h  Markup.h  Object.h
FILES=Vec3.cpp Car.cpp  Markup.cpp  Object.cpp Server.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o Server $(FILES)  $(LIBS) -I. -lpthread
