//
// Created by ebpedrollo on 03/04/17.
//

#include <elf.h>
#include "filter.h"
#include "image.h"

void convolution_matrix(Image **image, ConvolutionMatrix filter) {
    Image* copy;
    int i,j,x,y,t,r;
    int total_weight = 0;
    int matrix_offset;

    struct {
        short r;
        short g;
        short b;
    } current_bright;

    copy = copy_image(*image);

    matrix_offset = filter.matrix_size/2;

    for (x=0;x<filter.matrix_size;x++)
        for (y=0;y<filter.matrix_size;y++)
            total_weight += filter.matrix[x][y];



    for (i=0;i<(*image)->height;i++) {
        for (j=0;j<(*image)->width;j++) {

            current_bright.r = 0;
            current_bright.g = 0;
            current_bright.b = 0;
            for (x = 0; x < filter.matrix_size; x++) {
                for (y = 0; y < filter.matrix_size; y++) {

                    t = (x - matrix_offset);
                    r = (y - matrix_offset);

                    if (i + t < 0 || i + t > (*image)->height - 1) t = 0;
                    if (j + r < 0 || j + r > (*image)->width  - 1) t = 0;

                    current_bright.r += (*image)->matrix[i+t][j+r].r * filter.matrix[x][y];
                    current_bright.g += (*image)->matrix[i+t][j+r].g * filter.matrix[x][y];
                    current_bright.b += (*image)->matrix[i+t][j+r].b * filter.matrix[x][y];

                }
            }

            current_bright.r *= filter.multiplier;
            current_bright.g *= filter.multiplier;
            current_bright.b *= filter.multiplier;

            copy->matrix[i][j].r = (current_bright.r < 0)? (uint8_t) 0 :
                                   (current_bright.r > (*image)->max_bright)? (*image)->max_bright :
                                   (uint8_t) current_bright.r;

            copy->matrix[i][j].g = (current_bright.g < 0)? (uint8_t) 0 :
                                   (current_bright.g > (*image)->max_bright)? (*image)->max_bright :
                                   (uint8_t) current_bright.g;

            copy->matrix[i][j].b = (current_bright.b < 0)? (uint8_t) 0 :
                                   (current_bright.b > (*image)->max_bright)? (*image)->max_bright :
                                   (uint8_t) current_bright.b;

        }
    }

    free_image(*image);
    *image = copy;
}


void invert(Image **image){

    int i,j;

    for (i=0;i<(*image)->height;i++){
        for (j=0;j<(*image)->width;j++){
            (*image)->matrix[i][j].r = (*image)->max_bright - (*image)->matrix[i][j].r;
            (*image)->matrix[i][j].g = (*image)->max_bright - (*image)->matrix[i][j].g;
            (*image)->matrix[i][j].b = (*image)->max_bright - (*image)->matrix[i][j].b;
        }
    }

}

void rotate(Image **image,short angle){

}