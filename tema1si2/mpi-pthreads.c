#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <mpi.h>
#include <math.h>
#include <pthread.h>

/* Used for loading the initial image */
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

/* Used for writing the sepia image */
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define LEN 60
#define PARENT 0
#define NUM_THREADS 8

struct argum {
    int id;
    int image_size;
    int channels;
    unsigned char *img;
    unsigned char *sepia_image;
};

void *f(void *arg) {
    struct argum s = *(struct argum*) arg;
    
    int start = s.id * (double) s.image_size / NUM_THREADS;
    int end = fmin((s.id + 1) * (double) s.image_size / NUM_THREADS, s.image_size);

    for(int i = start; i < end; i += s.channels) {
        s.sepia_image[i]     = (uint8_t)fmin(0.393 * s.img[i] + 0.769 * s.img[i + 1] + 0.189 * s.img[i + 2], 255.0);         /* Red   */
        s.sepia_image[i + 1] = (uint8_t)fmin(0.349 * s.img[i] + 0.686 * s.img[i + 1] + 0.168 * s.img[i + 2], 255.0);         /* Green */
        s.sepia_image[i + 2] = (uint8_t)fmin(0.272 * s.img[i] + 0.534 * s.img[i + 1] + 0.131 * s.img[i + 2], 255.0);         /* Blue  */        
        
        if(s.channels == 4) 
            s.sepia_image[i + 3] = s.img[i + 3];
        
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    /* Open input directory */
    DIR *inputDIRp = opendir("./original_images"); 
    if (inputDIRp == NULL) {
        printf ("Error: Cannot open input directory.\n");
        return 1;
    }

    /* If output directory does not exist, it is created */
    DIR *outputDIRp = opendir("output_images");
    if (outputDIRp == NULL)
        system("mkdir output_images");

    /* Initialize the processes */
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int dest = 0;
    /* Parent process sends image loading and writing path */
    if (rank == PARENT) {
        /* Read the files from input directory */
        struct dirent *entry;
        while ((entry = readdir(inputDIRp)) != NULL) {
            /* If the entry is a regular file */
            if (entry->d_type == DT_REG) {
                dest++;

                /* Construct the path from where the initial image will be loaded */
                char file_path[LEN] = "./original_images/";
                strcat(file_path, entry->d_name);

                /* Send loading path to destination process */
                MPI_Send(file_path, LEN, MPI_BYTE, dest, 0, MPI_COMM_WORLD);

                /* Replace "./original" with "./output" to create output file path */
                char aux[LEN], out_path[LEN];
                memmove(aux, file_path + 10, strlen(file_path) - 9);
                strcpy(out_path, "./output");
                strcat(out_path, aux);

                /* Send writing path to destination process */
                MPI_Send(out_path, LEN, MPI_BYTE, dest, 1, MPI_COMM_WORLD);
                
                memmove(aux, aux + 8, strlen(file_path) - 7);
                printf("Image %s was sent to process %d\n", aux, dest);
            }
        }
    }
    else {
        /* Receive image's load and write paths */
        char file_path[LEN], out_path[LEN];
        MPI_Recv(&file_path, LEN, MPI_BYTE, PARENT, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //printf("Process %d Received1 %s\n", rank, file_path);
        MPI_Recv(&out_path, LEN, MPI_BYTE, PARENT, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //printf("Process %d Received2 %s\n", rank, out_path);

        /* Loading initial image */
        int width, height, channels;
        unsigned char *img = stbi_load(file_path, &width, &height, &channels, 0);
        if (img == NULL) {
            printf("Error: Cannot load the image.\n");
            exit(1);
        }

        /* Allocate memory for the output image */
        unsigned char *sepia_image = malloc(width * height * channels);
        if (sepia_image == NULL) {
            printf("Error: Cannot allocate memory for the sepia image.\n");
            exit(1);
        }

        /* Apply the sepia filter on the original image and save the result in sepia_image */
        int image_size = width * height * channels;

        void *status;
        pthread_t threads[NUM_THREADS];
        struct argum arguments[NUM_THREADS];

        /* Create threads */
        for (int id = 0; id < NUM_THREADS; id++) {
            arguments[id].id = id;
            arguments[id].image_size = image_size;
            arguments[id].channels = channels;
            arguments[id].img = img;
            arguments[id].sepia_image = sepia_image;
            
            int r = pthread_create(&threads[id], NULL, f, (void *) &arguments[id]);
            printf("Thread %d created in process %d\n", id, rank);

            if (r) {
                printf("Error at creating thread %d\n", id);
                exit(-1);
            }
        }

        for (int id = 0; id < NUM_THREADS; id++) {
            int r = pthread_join(threads[id], &status);
            if (r) {
                printf("Error at waiting thread %d\n", id);
                exit(-1);
            }
        }

        /* Write the sepia image to the output file path */
        stbi_write_jpg(out_path, width, height, channels, sepia_image, 100);

        printf("RESULT process %d:    %s -> %s\n", rank, file_path, out_path);

        free(sepia_image);
        stbi_image_free(img);
    }

    MPI_Finalize();
    closedir(inputDIRp);

    return 0;
}