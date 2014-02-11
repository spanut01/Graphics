BCC = g++
DLIBS = -L/comp/175/public_html/labs/lib -lGL -lGLU -lglut -lglui
#SLIBS = /h/mshah08/Desktop/comp175/lib/libGLEW.a
#INC = -I./ -I/h/mshah08/Desktop/comp175/include
EXE = Assignment2
SRC = Assignment2.cpp Camera.cpp

$(EXE) : 
	$(BCC) $(INC) $(SRC) -o $@ $(DLIBS) $(SLIBS)

clean:
	rm -rf *.o *~ $(EXE)
