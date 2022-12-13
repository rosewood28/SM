serial: serial.c
	gcc serial.c -o serial -lm

openmp: openmp.c
	gcc openmp.c -o openmp -lm -fopenmp

mpi: mpi.c
	mpicc mpi.c -o mpi -lm

pthreads: pthreads.c
	gcc pthreads.c -o pthreads -lpthread -lm

mpi-openmp: mpi-openmp.c
	mpicc mpi-openmp.c -o mpi-openmp -lm -fopenmp

clean:
	rm -f serial openmp
