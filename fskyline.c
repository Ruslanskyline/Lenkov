all: lodepng.o main.o
	gcc *.o -o main

main.o: skyline.c lodepng.h
	gcc -c skyline.c

lodepng.o: lodepng.c lodepng.h
	gcc -c lodepng.c

clean:
	rm -f *.o
