CC = clang++ -g
DLIBS = -L/usr/X11R6/lib -lGL -lGLU -lglut -lglui
INC = -I./ -I/usr/X11R6/include
EXE = lab7

all:
	$(CC) $(INC) *.cpp -o $(EXE) $(DLIBS)

clean:
	rm -rf *.o *~ $(EXE)
