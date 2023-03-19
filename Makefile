dict3: driver.c data.o quadtree.o array.o list.o
	gcc -Wall -o dict3 driver.c data.o quadtree.o array.o list.o
dict4: driver.c data.o quadtree.o array.o list.o
	gcc -Wall -o dict4 driver.c data.o quadtree.o array.o list.o
data.o: data.h data.c
	gcc -Wall -c data.c
list.o: list.h list.c
	gcc -Wall -c list.c
array.o: array.h array.c
	gcc -Wall -c array.c
quadtree.o: quadtree.h quadtree.c
	gcc -Wall -c quadtree.c

