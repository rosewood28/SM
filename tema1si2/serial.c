#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {

    int file_count = 0;

    DIR *inputDIRp;
    struct dirent *entry;

    inputDIRp = opendir("./original_images"); 
    if (inputDIRp == NULL) {
        printf ("Cannot open input directory \n");
        return 1;
    }

    /* Counts images in input directory */
    while ((entry = readdir(inputDIRp)) != NULL) {
        /* If the entry is a regular file */
        if (entry->d_type == DT_REG) { 
            file_count++;
        }
    }

    /* If output directory does not exist, it is created */
    DIR *outputDIRp;
    outputDIRp = opendir("output_images");
    if (outputDIRp == NULL)
        system("mkdir output_images");

    for (int i = 0; i < file_count; i++) {
        //creating image name
        char img_name[20];


    }

    closedir(inputDIRp);
    return 0;
}