CC = g++
SRC = Assignment1.cpp Cube.cpp Cylinder.cpp Cone.cpp Sphere.cpp Torus.cpp funShape.cpp
LIBS = -L/comp/175/public_html/labs/lib -lGL -lGLU -lglut -lglui
INC =  -I. -I/comp/175/public_html/labs/include/
EXEC = Assignment1

all:
	$(CC) $(SRC) -o $(EXEC) $(LIBS) $(INC)

clean:
	rm -rf $(EXEC) *~
