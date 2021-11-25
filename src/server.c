#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "image_process.hpp"

int main (int argc, char* argv[]) {
    char* imageName = argv[1];
    char* function  = argv[2];
    char* low_t  = argv[3];
    char* high_t  = argv[4];
    char output_path[128];


    if (argc < 3) {
        printf("Usage:\t./test <image> <function>\n");
        return 0;
    }

    //Create output directory
    if (mkdir(OUTPUT_DIR, 0751) < 0) {
        printf("Error creating output directory.\n");
    }

    strcpy(output_path, OUTPUT_DIR);

    if (!strcmp(function, "grayscale")) {
        strcat(output_path, "gray_image.png");
        grayscale_file(imageName, output_path);
    }
    else if (!strcmp(function, "blur")) {
        strcat(output_path, "blur_image.png");
        blur_file(imageName, output_path);
    }
    else if (!strcmp(function, "edge-detect")) {
        if (argc != 5) {
            printf("Usage:\t./test <image> <function>\n");
            return 0;
        }
        int low_threshold = atoi(low_t);
        int high_threshold = atoi(high_t);
        strcat(output_path, "edge_detection_image.png");
        edge_detect_file(imageName, output_path, low_threshold, high_threshold);
    }
    return 0;
}
