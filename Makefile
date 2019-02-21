PROG = circle semicircle square elipse ring shapes
OBJS = circle.o semicircle.o square.o elipse.o ring.o master.o handler.o
SRCS = circle.c semicircle.c square.c elipse.c ring.c master.c handler.c
HDRS = handler.h
all: $(PROG)

master.o:
	gcc -g -c master.c

circle.o:
	gcc -g -c circle.c
	
semicircle.o:
	gcc -g -c semicircle.c
	
square.o:
	gcc -g -c square.c
	
elipse.o:
	gcc -g -c elipse.c
	
ring.o:
	gcc -g -c ring.c

handler.o:
	gcc -g -c handler.c
	
shapes: master.o handler.o
	gcc -g master.o handler.o -o shapes

circle: circle.o
	gcc -g circle.o -o circle
	
semicircle: semicircle.o
	gcc -g semicircle.o -o semicircle
	
square: square.o
	gcc -g square.o -o square
	
elipse: elipse.o
	gcc -g elipse.o -o elipse
	
ring: ring.o
	gcc -g ring.o -o ring

clean:
	rm $(OBJS) $(PROG)
