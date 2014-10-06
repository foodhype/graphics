INC = -I ../include
LDLIBS =  -lGLEW -lglut -lGL -lGLU -lXmu -lX11 -lc -lm

build:
	gcc *.c -g $(LDLIBS) $(INC) -o main

run:
	gcc *.c -g $(LDLIBS) $(INC) -o main && ./main
