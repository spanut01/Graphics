CC = g++
SRC = main.cpp solarSystem.cpp 
LIBS = -L/comp/175/public_html/labs/lib -lGL -lGLU -lglui -lglut -lm
INC =  -I. -I/comp/175/public_html/labs/include/ 
EXEC = solar

all:
	$(CC) $(SRC) -o $(EXEC) $(LIBS) $(INC) 

clean:
	rm -rf $(EXEC) *~
