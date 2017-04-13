#include "image.h"
#include "conv_mat.h"

#ifndef PERFECTPIXELMESS_FILTER_H

void apply_filter(FilterDef *filter, Image **image);
void convolution_matrix(Image **image, ConvolutionMatrix filter);
void greyscale(Image **image);
void brighten(Image **image, short amount);
void rgb(Image **image, short red,short green, short blue);
void invert(Image **image);
void rotate(Image **image,short angle);
void flip(Image **image);
void mirror(Image **image);

#define PERFECTPIXELMESS_FILTER_H

#endif /* PERFECTPIXELMESS_FILTER_H */
