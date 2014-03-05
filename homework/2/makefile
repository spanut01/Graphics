CC = g++
SRC = Assignment2.cpp Camera.cpp Cone.cpp Sphere.cpp Cube.cpp Torus.cpp Cylinder.cpp
LIBS = -L/comp/175/public_html/labs/lib -lGL -lGLU -lglut -lglui
INC =  -I. -I/comp/175/public_html/labs/include/
EXEC = Assignment2

all:
	$(CC) $(SRC) -o $(EXEC) $(LIBS) $(INC)

clean:
	rm -rf $(EXEC) *~
