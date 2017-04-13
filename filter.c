#include <elf.h>
#include <strings.h>
#include <stdlib.h>
#include "filter.h"

const int MAX_BRIGHT =  255;
const int MIN_BRIGHT = -255;


void apply_filter(FilterDef *filter, Image **image){

    /*  Simple Filters */
    if (strcasecmp(filter->name,"INVERT") == 0) {
        invert(image);
    } else if (strcasecmp(filter->name,"RGB") == 0){
        if (*(filter->params+1) != NULL && *(filter->params+2) != NULL){
            rgb(image,
                (short)atoi(filter->params[0]),
                (short)atoi(filter->params[1]),
                (short)atoi(filter->params[2])
            );
        } else brighten(image, (short)atoi(filter->params[0]));
    } else if (strcasecmp(filter->name,"GREYSCALE") == 0){
        greyscale(image);
    }

    /*  Transform Filters */
    else if (strcasecmp(filter->name,"ROTATE") == 0) {
        rotate(image,(short)atoi(filter->params[0]));
    } else if (strcasecmp(filter->name,"FLIP") == 0) {
        flip(image);
    } else if (strcasecmp(filter->name,"MIRROR") == 0) {
        mirror(image);
    }


    /*  Convolution Matrix Filters */
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
    } else {
        fprintf(stderr,"%s: filter not recognized",filter->name);
        exit(EXIT_FAILURE);
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

void greyscale(Image **image){
    const float r = 0.299F;
    const float g = 0.587F;
    const float b = 0.114F;

    int i,j;

    float c;

    for (i=0;i<(*image)->height;i++){
        for (j=0;j<(*image)->width;j++){

            c = ((*image)->matrix[i][j].r*r)+((*image)->matrix[i][j].g*g)+((*image)->matrix[i][j].b*b);

            (*image)->matrix[i][j].r = (unsigned char) c;
            (*image)->matrix[i][j].g = (unsigned char) c;
            (*image)->matrix[i][j].b = (unsigned char) c;
        }
    }

}

void brighten(Image **image, short amount) {
    rgb(image,amount,amount,amount);
}

void rgb(Image **image, short red,short green, short blue){
    int i,j;
    struct {
        int r;
        int g;
        int b;
    } pixel;

    if (red > MAX_BRIGHT || red < MIN_BRIGHT ||
        green > MAX_BRIGHT || green < MIN_BRIGHT ||
        blue > MAX_BRIGHT || blue < MIN_BRIGHT) {
        fprintf(stderr,"RGB filter must have values ranging from -255 to 255.");
        exit(EXIT_FAILURE);
    }

    for (i=0;i<(*image)->height;i++){
        for (j=0;j<(*image)->width;j++){

            pixel.r = (*image)->matrix[i][j].r + (red   * ( (*image)->max_bright / MAX_BRIGHT ));
            pixel.g = (*image)->matrix[i][j].g + (green * ( (*image)->max_bright / MAX_BRIGHT ));
            pixel.b = (*image)->matrix[i][j].b + (blue  * ( (*image)->max_bright / MAX_BRIGHT ));

            (*image)->matrix[i][j].r = (pixel.r < 0)? (uint8_t) 0 :
                                       (pixel.r > (*image)->max_bright)? (*image)->max_bright :
                                       (uint8_t) pixel.r;

            (*image)->matrix[i][j].g = (pixel.g < 0)? (uint8_t) 0 :
                                       (pixel.g > (*image)->max_bright)? (*image)->max_bright :
                                       (uint8_t) pixel.g;

            (*image)->matrix[i][j].b = (pixel.b < 0)? (uint8_t) 0 :
                                       (pixel.b > (*image)->max_bright)? (*image)->max_bright :
                                       (uint8_t) pixel.b;
        }
    }


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