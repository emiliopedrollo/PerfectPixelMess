#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "image.h"

Image *load_image(FILE *stream) {
    int i, j, width, height;
    unsigned char max_bright;

    Image *image = NULL;

    char format[5];
    if (stream != NULL) {
        /* TODO: Treat invalid stream */
        /* TODO: Treat comment lines */
        /* TODO: Treat data too short */

        fscanf(stream, "%s\n", format);
        if (strcmp(format,"P3") != 0){
            fprintf(stderr,"Input is not in a valid format. Please use PPM P3.\n");
            exit(EXIT_FAILURE);
        }

        fscanf(stream, "%d %d\n", &width, &height);
        fscanf(stream, "%hhu\n", &max_bright);

        image = new_image(height,width,max_bright);

        for (i=0;i<image->height;i++){
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

    copy = new_image(image->height,image->width,image->max_bright);

    for (i=0;i<image->height;i++){
        for (j=0;j<image->width;j++){
            copy->matrix[i][j].r = image->matrix[i][j].r;
            copy->matrix[i][j].g = image->matrix[i][j].g;
            copy->matrix[i][j].b = image->matrix[i][j].b;
        }
    }

    return copy;

}


Image *new_image(int height,int width, unsigned char max_bright){
    int i;
    Image* image;
    image = malloc(sizeof(Image));
    image->height = height;
    image->width = width;
    image->max_bright = max_bright;


    image->matrix = (Pixel**) malloc(image->height * sizeof(Pixel*));
    for (i=0;i<image->height;i++){
        image->matrix[i] = (Pixel*) malloc(image->width * sizeof(Pixel));
    }

    return image;
}

void free_image(Image* image){
    int i;
    for (i=0;i<image->height;i++){
        free(image->matrix[i]);
    }
    free(image->matrix);
    free(image);
}
