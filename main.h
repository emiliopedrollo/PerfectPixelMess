//
// Created by ncc on 28/03/17.
//

#ifndef PERFECTPIXELMESS_MAIN_H

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

typedef struct {
    int width;
    int height;
    int max_bright;
    Pixel **matrix;
} Image;

#define PERFECTPIXELMESS_MAIN_H

#endif //PERFECTPIXELMESS_MAIN_H
