CC = g++
SRC = main.cpp ppm.cpp shaderManager.cpp GLee.c
LIBS = -L/comp/175/public_html/labs/lib -lglui -lglut -lGL -lGLU -lm -lstdc++
INC =  -I. -I/comp/175/public_html/labs/include/ 
EXEC = drag

all:
	$(CC) $(SRC) -o $(EXEC) $(LIBS) $(INC)

clean:
	rm -rf $(EXEC) *~
