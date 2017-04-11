//
// Created by ebpedrollo on 03/04/17.
//

#include <elf.h>
#include <strings.h>
#include <stdlib.h>
#include "filter.h"

void apply_filter(FilterDef *filter, Image **image){

    // Simple Filters
    if (strcasecmp(filter->name,"INVERT") == 0) {
        invert(image);
    } else if (strcasecmp(filter->name,"RGB") == 0){
        // TODO: implement RGB filter
    } else if (strcasecmp(filter->name,"GREYSCALE") == 0){
        // TODO: implement Greyscale filter
    }

    // Transform Filters
    else if (strcasecmp(filter->name,"ROTATE") == 0) {
        rotate(image,(short)atoi(filter->params[0]));
    } else if (strcasecmp(filter->name,"FLIP") == 0) {
        flip(image);
    } else if (strcasecmp(filter->name,"MIRROR") == 0) {
        mirror(image);
    }


    // Convolution Matrix Filters
    else if (strcasecmp(filter->name,"TEST") == 0){
        convolution_matrix(image,get_test_matrix());

    } else if (strcasecmp(filter->name,"BLUR") == 0){
        if (filter->params != NULL){
            convolution_matrix(image, get_blur_matrix((char)atoi(filter->params[0])));
        } else {
            convolution_matrix(image, get_blur_matrix(1));
        }
    } else if (strcasecmp(filter->name,"BORDER") == 0){
        convolution_matrix(image,get_border_detect_matrix());

    } else if (strcasecmp(filter->name,"SHARP") == 0){
        convolution_matrix(image,get_sharp_matrix());
    } else if (strcasecmp(filter->name,"CUSTOM") == 0){
        // TODO: implements the custom matrix filter
    }
}

void convolution_matrix(Image **image, ConvolutionMatrix filter) {
    Image* copy;
    int i,j,x,y,t,r;
    int matrix_offset;

    struct {
        int r;
        int g;
        int b;
    } current_bright;

    copy = copy_image(*image);

    matrix_offset = filter.matrix_size/2;

    for (i=0;i<(*image)->height;i++) {
        for (j=0;j<(*image)->width;j++) {

            current_bright.r = 0;
            current_bright.g = 0;
            current_bright.b = 0;
            for (x = 0; x < filter.matrix_size; x++) {
                for (y = 0; y < filter.matrix_size; y++) {

                    t = (x - matrix_offset);
                    r = (y - matrix_offset);

                    t = (i + t < 0 || i + t > (*image)->height - 1)? 0 : t;
                    r = (j + r < 0 || j + r > (*image)->width  - 1)? 0 : r;

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

    free_matrix(filter.matrix,filter.matrix_size);

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

    int i,j;
    Image* copy;

    switch (angle){
        case 90:
            copy = new_image((*image)->width,(*image)->height,(*image)->max_bright);
            for (i=0;i<(*image)->height;i++){
                for (j=0;j<(*image)->width;j++){
                    copy->matrix[j][copy->width-i-1].r = (*image)->matrix[i][j].r;
                    copy->matrix[j][copy->width-i-1].g = (*image)->matrix[i][j].g;
                    copy->matrix[j][copy->width-i-1].b = (*image)->matrix[i][j].b;
                }
            }
            break;
        case 180:
            copy = copy_image(*image);
            for (i=0;i<(*image)->height;i++){
                for (j=0;j<(*image)->width;j++){
                    copy->matrix[copy->height-i-1][copy->width-j-1].r = (*image)->matrix[i][j].r;
                    copy->matrix[copy->height-i-1][copy->width-j-1].g = (*image)->matrix[i][j].g;
                    copy->matrix[copy->height-i-1][copy->width-j-1].b = (*image)->matrix[i][j].b;
                }
            }
            break;
        case 270:
            copy = new_image((*image)->width,(*image)->height,(*image)->max_bright);
            for (i=0;i<(*image)->height;i++){
                for (j=0;j<(*image)->width;j++){
                    copy->matrix[copy->height-j-1][i].r = (*image)->matrix[i][j].r;
                    copy->matrix[copy->height-j-1][i].g = (*image)->matrix[i][j].g;
                    copy->matrix[copy->height-j-1][i].b = (*image)->matrix[i][j].b;
                }
            }
            break;
        case 0:
        case 360:
            return;
        default:
            fprintf(stderr,"Rotation only works with 0, 90, 180, 270 or 360 degrees.\n");
            exit(EXIT_FAILURE);
    }

    free_image(*image);
    *image = copy;

}

void flip(Image **image){

    int i,j;
    Image *copy;
    copy = copy_image(*image);


    for (i=0;i<(*image)->height;i++){
        for (j=0;j<(*image)->width;j++){
            copy->matrix[(*image)->height-i-1][j].r = (*image)->matrix[i][j].r;
            copy->matrix[(*image)->height-i-1][j].g = (*image)->matrix[i][j].g;
            copy->matrix[(*image)->height-i-1][j].b = (*image)->matrix[i][j].b;
        }
    }

    free_image(*image);
    *image = copy;
}

void mirror(Image **image){

    int i,j;
    Image *copy;
    copy = copy_image(*image);

    for (i=0;i<(*image)->height;i++){
        for (j=0;j<(*image)->width;j++){
            copy->matrix[i][(*image)->width-j-1].r = (*image)->matrix[i][j].r;
            copy->matrix[i][(*image)->width-j-1].g = (*image)->matrix[i][j].g;
            copy->matrix[i][(*image)->width-j-1].b = (*image)->matrix[i][j].b;
        }
    }

    free_image(*image);
    *image = copy;
}