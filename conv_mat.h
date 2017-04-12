#ifndef PERFECTPIXELMESS_CONV_MAT_H


typedef struct {
    char* name;
    char** params;
} FilterDef;

typedef struct {
    short** matrix;
    unsigned char matrix_size;
    double multiplier;
} ConvolutionMatrix;

void free_matrix(short **matrix,unsigned char matrix_size);
short **generate_matrix(unsigned char size);
ConvolutionMatrix get_test_matrix();
ConvolutionMatrix get_blur_matrix(char sample_size);
ConvolutionMatrix get_border_detect_matrix();
ConvolutionMatrix get_sharp_matrix();

#define PERFECTPIXELMESS_CONV_MAT_H

#endif //PERFECTPIXELMESS_CONV_MAT_H
