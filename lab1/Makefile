all: default parall

parall:
	mpicc par_solver.c -o parallel -lm
	mpirun -np 4 parallel
	python3 vision.py parallel.txt

default:
	gcc -o dir direct_solver.c src/methods.c src/next_point.c -lm
	./dir
	python3 vision.py

py:
	python3 vision.py

debug:
	pylint vision.py
	valgrind --leak-check=full --show-leak-kinds=all ./dir
	mpirun -n 2 valgrind --leak-check=full --show-leak-kinds=all ./parallel
