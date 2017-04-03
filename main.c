#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <limits.h>
#include "main.h"
#include "utils.h"
#include "lists.h"
#include "image.h"

FilterDef *extractFilterDef(char* argument);
void convolution_matrix(Image **image);
void display_usage();

void print_filter(void *pVoid);

int main (int argc, char *argv[]) {

    int c, i;
    bool reading = true;
    bool read_from_stdin = true;
    bool output_to_stdout = true;
    bool show_help = false;
    char *read_from = NULL;
    char *output_to = NULL;
    char** tokens;
    Node *filters = NULL;
    Image *image;

    // Os blocos a seguir verificam a existência de parametros de
    // execução do executavel e definem variaveis que alterarão o
    // comportamento do mesmo de acordo com estes parametros
    static struct option long_options[] = {
            {"input",  required_argument, 0, 'i'},
            {"output", required_argument, 0, 'o'},
            {"filter", required_argument, 0, 'f'},
            {"help",   no_argument      , 0, 'h'},
            {0,        0,                 0,  0 }
    };

    while (reading) {
        int option_index = 0;
        c = getopt_long(argc, argv, "i:o:f:h", long_options, &option_index);

        switch (c) {
            case 'i':
                read_from_stdin = false;
                read_from = malloc((strlen(optarg)+1) * sizeof(char));
                strcpy(read_from,optarg);
                break;
            case 'o':
                output_to_stdout = false;
                output_to = malloc((strlen(optarg)+1) * sizeof(char));
                strcpy(output_to,optarg);
                break;
            case 'f':
                filters = list_new();

                tokens = str_split(optarg,',');
                for (i = 0; *(tokens + i); i++){
                    list_insert(filters,extractFilterDef(*(tokens + i)));
                    free(*(tokens + i));
                }
                free(tokens);

                list_print(filters, print_filter);

                exit(EXIT_SUCCESS);

                break;
            case 'h':
            case '?':
                show_help = true;
                break;
            case -1:
                reading = false;
                break;
            default:
                break;
        }
    }

    if (show_help){
        display_usage();
        exit(EXIT_SUCCESS);
    }

    if (read_from_stdin){
        image = load_image(stdin);
    } else {
        FILE *in_file = fopen(read_from, "r");
        image = load_image(in_file);
        fclose(in_file);
    }

    convolution_matrix(&image);

    //negative(pixel,&a,&l,&m);

    //aumentar_brilho(pixel,&a,&l,&m);

//    diminuir_brilho(pixel, &a, &l, &m);

    //convolution_matrix(pixel ,&a, &l,&m);


    if (output_to_stdout){
        output(stdout,image);
    } else {
        FILE *out_file = fopen(output_to, "w");
        output(out_file,image);
        fclose(out_file);
    }

    exit(0);
}

void print_filter(void *pVoid) {
    FilterDef *filter;
    int i;

    filter = (FilterDef*) pVoid;
    printf("Filter: %s\n", filter->name);
    if (filter->params != NULL) {
        printf("Params: ");
        for (i = 0; *(filter->params + i); i++) {
            printf("%s ", *(filter->params + i));
        }
        printf("\n");
    }
    printf("\n");
}

FilterDef *extractFilterDef(char* argument){
    FilterDef* filter;
    char** tokens;
    char* params_raw;

    filter = malloc(sizeof (FilterDef));
    if (countChars(argument,'=') == 0){
        filter->name = malloc((sizeof argument)+1);
        strcpy(filter->name,argument);
        filter->params = NULL;
    } else {
        tokens = str_split(argument,'=');
        filter->name = *tokens;
        params_raw = malloc((sizeof *(tokens+1))+1);
        strcpy(params_raw,*(tokens+1));

//        if (countChars(params_raw,';') > 1)
//            topntail(params_raw);

        filter->params = str_split(params_raw,';');
    }

    return filter;
}

void display_usage() {
    printf("Usage: ppm [options]...\n"
    "Apply filters to ppm images\n"
    "\n"
    "Mandatory arguments to long options are mandatory for short options too.\n"
    "  -i --input=FILE            Use FILE as input stream. Otherwise ppm uses stdin.\n"
    "  -o --output=FILE           Outputs resulted image to FILE. Otherwise prints to stdout.\n"
    "  -f --filter=F1,F2=1;7      Sets the filter to use on the input image. Where F1 represent \n"
    "                               one of the filters listed bellow, F2=1 represent another with 1\n"
    "                               as an argument. Filters must be separated by commas. Arguments, \n"
    "                               if more than one must be separated by semicolons.\n"
    "\n"
    "\n"
    "The available filters are:\n"
    "  INV        Inverts the image\n"
    "  BRI=INT    Changes the bright of the image. INT is the intensity of the filter, must be\n"
    "               between -255 and 255.\n"
    "\n");
}




void negative(Image *image) {
    int i, j;
    for (i = 0; i < image->height; i++) {
        for (j = 0; j < image->width; j++) {
            image->matrix[i][j].r = image->max_bright - image->matrix[i][j].r;
            image->matrix[i][j].g = image->max_bright - image->matrix[i][j].g;
            image->matrix[i][j].b = image->max_bright - image->matrix[i][j].b;
        }
    }
}

void convolution_matrix(Image **image) {
    Image* copy;
    short matrix[5][5];
    int i,j,x,y,t,r;
    int matrix_size = 5;
    int total_weight = 0;
    int matrix_offset;
    float matrix_multiplier = (float)1/25;

    struct {
        short r;
        short g;
        short b;
    } current_bright;

    copy = copy_image(*image);

    matrix[0][0] =  1;
    matrix[0][1] =  1;
    matrix[0][2] =  1;
    matrix[0][3] =  1;
    matrix[0][4] =  1;

    matrix[1][0] =  1;
    matrix[1][1] =  1;
    matrix[1][2] =  1;
    matrix[1][3] =  1;
    matrix[1][4] =  1;

    matrix[2][0] =  1;
    matrix[2][1] =  1;
    matrix[2][2] =  1;
    matrix[2][3] =  1;
    matrix[2][4] =  1;

    matrix[3][0] =  1;
    matrix[3][1] =  1;
    matrix[3][2] =  1;
    matrix[3][3] =  1;
    matrix[3][4] =  1;

    matrix[4][0] =  1;
    matrix[4][1] =  1;
    matrix[4][2] =  1;
    matrix[4][3] =  1;
    matrix[4][4] =  1;

    matrix_offset = matrix_size/2;

    for (x=0;x<matrix_size;x++)
        for (y=0;y<matrix_size;y++)
            total_weight += matrix[x][y];



    for (i=0;i<(*image)->height;i++) {
        for (j=0;j<(*image)->width;j++) {

            current_bright.r = 0;
            current_bright.g = 0;
            current_bright.b = 0;
            for (x = 0; x < matrix_size; x++) {
                for (y = 0; y < matrix_size; y++) {

                    t = (x - matrix_offset);
                    r = (y - matrix_offset);

                    if (i + t < 0 || i + t > (*image)->height - 1) t = 0;
                    if (j + r < 0 || j + r > (*image)->width  - 1) t = 0;

                    current_bright.r += (*image)->matrix[i+t][j+r].r * matrix[x][y];
                    current_bright.g += (*image)->matrix[i+t][j+r].g * matrix[x][y];
                    current_bright.b += (*image)->matrix[i+t][j+r].b * matrix[x][y];

                }
            }

            current_bright.r *= matrix_multiplier;
            current_bright.g *= matrix_multiplier;
            current_bright.b *= matrix_multiplier;

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

void aumentar_brilho(Pixel **pixel, int *a, int *l, int *m) {
    int i, j;

    for (i = 0; i < *a; i++) {
        for (j = 0; j < *l; j++) {
            pixel[i][j].r = pixel[i][j].r + 60;
            pixel[i][j].g = pixel[i][j].g + 60;
            pixel[i][j].b = pixel[i][j].b + 60;
        }
    }

    for (i = 0; i < *a; i++) {
        for (j = 0; j < *l; j++) {
            if (pixel[i][j].r > *m) {
                pixel[i][j].r = *m;
            }
            if (pixel[i][j].g > *m) {
                pixel[i][j].g = *m;
            }
            if (pixel[i][j].b > *m) {
                pixel[i][j].b = *m;
            }
        }
    }
}

void diminuir_brilho(Pixel **pixel, int *a, int *l, int *m) {
    int i, j;

    for (i = 0; i < *a; i++) {
        for (j = 0; j < *l; j++) {
            pixel[i][j].r = pixel[i][j].r - 60;
            pixel[i][j].g = pixel[i][j].g - 60;
            pixel[i][j].b = pixel[i][j].b - 60;
        }
    }

    for (i = 0; i < *a; i++) {
        for (j = 0; j < *l; j++) {
            if (pixel[i][j].r < 0) {
                pixel[i][j].r = 0;
            }
            if (pixel[i][j].g < 0) {
                pixel[i][j].g = 0;
            }
            if (pixel[i][j].b < 0) {
                pixel[i][j].b = 0;
            }
        }
    }
}