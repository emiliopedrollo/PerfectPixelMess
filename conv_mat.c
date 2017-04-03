//
// Created by ebpedrollo on 03/04/17.
//

#include <stdlib.h>
#include "conv_mat.h"

void free_matrix(short **matrix,unsigned char matrix_size){
    unsigned char i;
    for (i=0;i<matrix_size;i++){
        free(matrix[i]);
    }
    free(matrix);
}

short **generate_matrix(unsigned char size){
    unsigned char i;
    short **matrix;
    matrix = malloc(size*sizeof(short*));
    for (i=0;i<size;i++){
        matrix[i] = malloc(size*sizeof(short));
    }
    return matrix;
}

ConvolutionMatrix get_test_matrix(){
    ConvolutionMatrix convolution;
    convolution.multiplier = 1;
    convolution.matrix_size = 5;
    convolution.matrix = generate_matrix(convolution.matrix_size);

    convolution.matrix[0][0] = 0;
    convolution.matrix[0][1] = 0;
    convolution.matrix[0][2] = 0;
    convolution.matrix[0][3] = 0;
    convolution.matrix[0][4] = 0;

    convolution.matrix[1][0] = 0;
    convolution.matrix[1][1] = 0;
    convolution.matrix[1][2] = 0;
    convolution.matrix[1][3] = 0;
    convolution.matrix[1][4] = 0;

    convolution.matrix[2][0] = 0;
    convolution.matrix[2][1] = 0;
    convolution.matrix[2][2] = 1;
    convolution.matrix[2][3] = 0;
    convolution.matrix[2][4] = 0;

    convolution.matrix[3][0] = 0;
    convolution.matrix[3][1] = 0;
    convolution.matrix[3][2] = 0;
    convolution.matrix[3][3] = 0;
    convolution.matrix[3][4] = 0;

    convolution.matrix[4][0] = 0;
    convolution.matrix[4][1] = 0;
    convolution.matrix[4][2] = 0;
    convolution.matrix[4][3] = 0;
    convolution.matrix[4][4] = 0;

    return convolution;
}

ConvolutionMatrix get_blur_matrix(){
    ConvolutionMatrix convolution;
    convolution.multiplier = (float) 1/9;
    convolution.matrix_size = 3;
    convolution.matrix = generate_matrix(convolution.matrix_size);

    convolution.matrix[0][0] = 1;
    convolution.matrix[0][1] = 1;
    convolution.matrix[0][2] = 1;

    convolution.matrix[1][0] = 1;
    convolution.matrix[1][1] = 1;
    convolution.matrix[1][2] = 1;

    convolution.matrix[2][0] = 1;
    convolution.matrix[2][1] = 1;
    convolution.matrix[2][2] = 1;

    return convolution;
}

ConvolutionMatrix get_extra_blur_matrix(){
    ConvolutionMatrix convolution;
    convolution.multiplier = (float) 1/25;
    convolution.matrix_size = 5;
    convolution.matrix = generate_matrix(convolution.matrix_size);

    convolution.matrix[0][0] = 1;
    convolution.matrix[0][1] = 1;
    convolution.matrix[0][2] = 1;
    convolution.matrix[0][3] = 1;
    convolution.matrix[0][4] = 1;

    convolution.matrix[1][0] = 1;
    convolution.matrix[1][1] = 1;
    convolution.matrix[1][2] = 1;
    convolution.matrix[1][3] = 1;
    convolution.matrix[1][4] = 1;

    convolution.matrix[2][0] = 1;
    convolution.matrix[2][1] = 1;
    convolution.matrix[2][2] = 1;
    convolution.matrix[2][3] = 1;
    convolution.matrix[2][4] = 1;

    convolution.matrix[3][0] = 1;
    convolution.matrix[3][1] = 1;
    convolution.matrix[3][2] = 1;
    convolution.matrix[3][3] = 1;
    convolution.matrix[3][4] = 1;

    convolution.matrix[4][0] = 1;
    convolution.matrix[4][1] = 1;
    convolution.matrix[4][2] = 1;
    convolution.matrix[4][3] = 1;
    convolution.matrix[4][4] = 1;

    return convolution;
}

ConvolutionMatrix get_border_detect_matrix(){
    ConvolutionMatrix convolution;
    convolution.multiplier = 1;
    convolution.matrix_size = 3;
    convolution.matrix = generate_matrix(convolution.matrix_size);

    convolution.matrix[0][0] = 0;
    convolution.matrix[0][1] = 0;
    convolution.matrix[0][2] = 0;

    convolution.matrix[1][0] = -1;
    convolution.matrix[1][1] = 1;
    convolution.matrix[1][2] = 0;

    convolution.matrix[2][0] = 0;
    convolution.matrix[2][1] = 0;
    convolution.matrix[2][2] = 0;

    return convolution;
}

ConvolutionMatrix get_sharp_matrix(){
    ConvolutionMatrix convolution;
    convolution.multiplier = 1;
    convolution.matrix_size = 3;
    convolution.matrix = generate_matrix(convolution.matrix_size);

    convolution.matrix[0][0] = 0;
    convolution.matrix[0][1] = -1;
    convolution.matrix[0][2] = 0;

    convolution.matrix[1][0] = -1;
    convolution.matrix[1][1] = 5;
    convolution.matrix[1][2] = -1;

    convolution.matrix[2][0] = 0;
    convolution.matrix[2][1] = -1;
    convolution.matrix[2][2] = 0;

    return convolution;
}

ConvolutionMatrix get_reacle_matrix(){
    ConvolutionMatrix convolution;
    convolution.multiplier = 1;
    convolution.matrix_size = 3;
    convolution.matrix = generate_matrix(convolution.matrix_size);

    convolution.matrix[0][0] = -1;
    convolution.matrix[0][1] = -1;
    convolution.matrix[0][2] = -1;

    convolution.matrix[1][0] = -1;
    convolution.matrix[1][1] = 9;
    convolution.matrix[1][2] = -1;

    convolution.matrix[2][0] = -1;
    convolution.matrix[2][1] = -1;
    convolution.matrix[2][2] = -1;

    return convolution;
}