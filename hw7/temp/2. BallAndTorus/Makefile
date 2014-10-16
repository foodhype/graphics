INC = -I ../include
LDLIBS =  -lGLEW -lglut -lGL -lGLU -lXmu -lX11 -lc -lm

build:
	g++ ballAndTorusShadowed.cpp -g $(LDLIBS) $(INC) -o main

run:
	g++ ballAndTorusShadowed.cpp -g $(LDLIBS) $(INC) -o main && ./main
