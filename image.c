#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "image.h"
#include "main.h"

Image *load_image(FILE *stream) {
    int i, j;

    Image *image = NULL;

    char format[5];
    if (stream != NULL) {
        //TODO: Treat invalid stream
        //TODO: Treat comment lines
        //TODO: Treat data too short

        fscanf(stream, "%s\n", format);
        if (strcmp(format,"P3") != 0){
            fprintf(stderr,"Input is not in a valid format. Please use PPM P3.");
            exit(EXIT_FAILURE);
        }

        image = malloc(sizeof (Image));
        fscanf(stream, "%d %d\n", &image->width, &image->height);
        fscanf(stream, "%hhu\n", &image->max_bright);

        image->matrix = (Pixel**) malloc(image->height * sizeof(Pixel*));
        for (i=0;i<image->height;i++){
            image->matrix[i] = (Pixel*) malloc(image->width * sizeof(Pixel));
            for (j=0;j<image->width;j++){
                fscanf(stream, "%hhu", &image->matrix[i][j].r);
                fscanf(stream, "%hhu", &image->matrix[i][j].g);
                fscanf(stream, "%hhu", &image->matrix[i][j].b);
            }
        }
    }
    return image;
}

bool output(FILE *stream, Image *image) {
    int i, j;

    if (stream == NULL){
        return false;
    }

    fprintf(stream, "P3\n");
    fprintf(stream, "%d %d\n", image->width, image->height);
    fprintf(stream, "%d\n", image->max_bright);

    for (i = 0; i < image->height; i++) {
        for (j = 0; j < image->width; j++) {
            fprintf(stream, "%d ", image->matrix[i][j].r);
            fprintf(stream, "%d ", image->matrix[i][j].g);
            fprintf(stream, "%d ", image->matrix[i][j].b);
        }
        fprintf(stream, "\n");
    }
    return true;
}

Image *copy_image(Image *image){

    int i,j;
    Image *copy;

    copy = malloc(sizeof(Image));
    copy->max_bright = image->max_bright;
    copy->width = image->width;
    copy->height = image->height;

    copy->matrix = (Pixel**) malloc(copy->height * sizeof(Pixel*));
    for (i=0;i<image->height;i++){
        copy->matrix[i] = (Pixel*) malloc(copy->width * sizeof(Pixel));
        for (j=0;j<image->width;j++){
            copy->matrix[i][j].r = image->matrix[i][j].r;
            copy->matrix[i][j].g = image->matrix[i][j].g;
            copy->matrix[i][j].b = image->matrix[i][j].b;
        }
    }

    return copy;

}