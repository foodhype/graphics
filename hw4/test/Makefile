INC = -I ../include

LDLIBS = -lglut -lGL -LGLU -lc -lm -lGLEW

#run:
#	gcc *.cpp $(LDLIBS) $(INC) -o main -lGL -lGLU -lXrandr -lXext -lX11 -lXxf86vm -lglfw3 && ./main
run:
	g++ *.cpp $(LDLIBS) $(INC) -o main -lsfml-graphics -lsfml-window -lsfml-system && ./main
