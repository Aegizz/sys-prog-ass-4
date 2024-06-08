part1: part1.c slow_functions.c
	gcc part1.c slow_functions.c -o PART1 -std=c99 -lpthread
part2: part2.c slow_functions.c
	gcc part2.c slow_functions.c -o PART2 -std=c99 -lpthread
clean:
	rm part1 part2