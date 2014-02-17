CC = g++
SRC = main.cpp movieCamera.cpp ply.cpp entity.cpp
LIBS = -L/comp/175/public_html/labs/lib -lGL -lGLU -lglui -lglut -lm
EXEC = movie

all:
	$(CC) $(SRC) -o $(EXEC) $(LIBS) -I. -I/comp/175/public_html/labs/include/

clean:
	rm -rf $(EXEC) *~
