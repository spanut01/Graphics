CC = g++
SRC = Assignment3.cpp Camera.cpp  
SHAPESRC = Cone.cpp Cube.cpp Cylinder.cpp Sphere.cpp
PARSESRC = SceneParser.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp
LIBS = -L/comp/175/public_html/labs/lib -lGL -lGLU -lglut -lglui
INC =  -I. -I/comp/175/public_html/labs/include/
EXEC = sceneview

all:
	$(CC) $(SRC) $(PARSESRC) $(SHAPESRC) -o $(EXEC) $(LIBS) $(INC)

clean:
	rm -rf $(EXEC) *~
