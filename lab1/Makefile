all: default parall

parall:
	mpicc par_solver.c -o parallel -lm
	mpirun -np 4 parallel


default:
	gcc -o main main.c src/methods.c src/next_point.c -lm
	./main

py:
	python3 vision.py
