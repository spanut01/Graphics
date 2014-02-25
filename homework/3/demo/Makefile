CC = g++
SRC = Assignment3.cpp Camera.cpp scene/*.cpp
LIBS = -L/comp/175/public_html/labs/lib -lGL -lGLU -lglut -lglui
INC =  -I. -I/comp/175/public_html/labs/include/
EXEC = sceneview

all:
	$(CC) $(SRC) -o $(EXEC) $(LIBS) $(INC)

clean:
	rm -rf $(EXEC) *~
