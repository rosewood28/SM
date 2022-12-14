IMAGES := $(wildcard original_images/*.jpg)
NO_OF_FILES := $(words $(IMAGES))
ONE := 1
NP := $(shell expr $(NO_OF_FILES) + $(ONE) )

#$(info    NO_OF_FILES is $(NO_OF_FILES))
#$(info    NP is $(NP))

serial: serial.c
	gcc serial.c -o serial -lm

runserial: serial
	./serial

openmp: openmp.c
	gcc openmp.c -o openmp -lm -fopenmp

runopenmp: openmp
	./openmp

mpi: mpi.c
	mpicc mpi.c -o mpi -lm

runmpi: mpi
	mpirun -np $(NP) --oversubscribe -quiet mpi

pthreads: pthreads.c
	gcc pthreads.c -o pthreads -lpthread -lm

runpthreads: pthreads
	./pthreads

mpi-openmp: mpi-openmp.c
	mpicc mpi-openmp.c -o mpi-openmp -lm -fopenmp

runmpi-openmp: mpi-openmp
	mpirun -np $(NP) --oversubscribe -quiet mpi-openmp

mpi-pthreads: mpi-pthreads.c
	mpicc mpi-pthreads.c -o mpi-pthreads -lm -lpthread

runmpi-pthreads: mpi-pthreads.c
	mpirun -np $(NP) --oversubscribe -quiet mpi-pthreads

clean:
	rm -f serial openmp mpi pthreads mpi-openmp
