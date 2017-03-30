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
    unsigned char max_bright;
    Pixel **matrix;
} Image;

typedef struct {
    char* name;
    char** params;
} FilterDef;

#define PERFECTPIXELMESS_MAIN_H

#endif //PERFECTPIXELMESS_MAIN_H
