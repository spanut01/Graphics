CC = g++
SRC = main.cpp  movieCamera.cpp  ply.cpp entity.cpp
LIBS = -L/comp/175/public_html/labs/lib -lGL -lGLU -lglui -lglut -lm
EXEC = ply

all:
	$(CC) $(SRC) -o $(EXEC) $(LIBS)

clean:
	rm -rf $(EXEC) *~
