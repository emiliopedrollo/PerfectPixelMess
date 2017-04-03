//
// Created by ebpedrollo on 03/04/17.
//

#ifndef PERFECTPIXELMESS_CONV_MAT_H


typedef struct {
    char* name;
    char** params;
} FilterDef;

typedef struct {
    short** matrix;
    unsigned char matrix_size;
    float multiplier;
} ConvolutionMatrix;

void free_matrix(short **matrix,unsigned char matrix_size);
ConvolutionMatrix get_test_matrix();
ConvolutionMatrix get_blur_matrix();
ConvolutionMatrix get_extra_blur_matrix();
ConvolutionMatrix get_border_detect_matrix();
ConvolutionMatrix get_sharp_matrix();
ConvolutionMatrix get_reacle_matrix();

#define PERFECTPIXELMESS_CONV_MAT_H

#endif //PERFECTPIXELMESS_CONV_MAT_H