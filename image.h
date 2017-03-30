//
// Created by ncc on 30/03/17.
//

#include "main.h"

#ifndef PERFECTPIXELMESS_IMAGE_H

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

typedef struct {
    int width;
    int height;
    unsigned char max_bright;
    Pixel **matrix;
} Image;

Image *load_image(FILE *stream);
bool output(FILE *stream, Image *image);
Image *copy_image(Image *image);
void free_image(Image* image);

#define PERFECTPIXELMESS_IMAGE_H

#endif //PERFECTPIXELMESS_IMAGE_H
