#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "image_process.hpp"

int main (int argc, char* argv[]) {
    char* imageName = argv[1];
    char* function  = argv[2];
    char output_path[128];

    if (argc != 3) {
        printf("Usage:\t./test <image> <function\n");
        return 0;
    }

    //Create output directory
    if (mkdir(OUTPUT_DIR, 0751) < 0) {
        printf("Error creating output directory.\n");
    }

    strcpy(output_path, OUTPUT_DIR);
    strcat(output_path, "gray_image.png");

    if (!strcmp(function, "grayscale"))
        grayscale_image(imageName, output_path);
    return 0;
}
