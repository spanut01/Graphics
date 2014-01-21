
EXE = example
DLIBS = -L/comp/175/public_html/labs/lib -lGL -lGLU -lglut -lglui
#SLIBS = /comp/175/lib/libGLEW.a
INC = -I./ -I../include

$(EXE) : *.cpp
	gcc $(INC) $< -o $@ $(DLIBS) $(SLIBS)

clean:
	rm -rf *.o *~ $(EXE)
