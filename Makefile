space_invaders: main.o funcs.o desenho.o movimento.o
	gcc main.o funcs.o desenho.o movimento.o -o space_invaders -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_main -lm
	
main.o: main.c funcs.h desenho.h movimento.h
	gcc -c main.c -o main.o -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_main -lm

funcs.o: funcs.c funcs.h
	gcc -c funcs.c -o funcs.o -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_main -lm

desenho.o: desenho.c funcs.h desenho.h
	gcc -c desenho.c -o desenho.o -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_main -lm

movimento.o: movimento.c funcs.h movimento.h
	gcc -c	movimento.c -o movimento.o -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_main -lm
	
clean:
	rm  -f *.o
	
purge:	clean
	rm -f space_invaders