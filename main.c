#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include "main.h"
#include "utils.h"
#include "lists.h"
#include "image.h"
#include "conv_mat.h"
#include "filter.h"

FilterDef *extractFilterDef(char* argument);
void convolution_matrix(Image **image, ConvolutionMatrix filter);
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


    while(filters != NULL) {
        if (filters->content != NULL){

            if (strcmp(((FilterDef*)(filters->content))->name,"TEST")){
                convolution_matrix(&image,get_test_matrix());
            } else if (strcmp(((FilterDef*)(filters->content))->name,"BLUR")){
                convolution_matrix(&image,get_blur_matrix());
            } else if (strcmp(((FilterDef*)(filters->content))->name,"EBLUR")){
                convolution_matrix(&image,get_extra_blur_matrix());
            } else if (strcmp(((FilterDef*)(filters->content))->name,"REALCE")){
                convolution_matrix(&image,get_reacle_matrix());
            } else if (strcmp(((FilterDef*)(filters->content))->name,"BORDER")){
                convolution_matrix(&image,get_border_detect_matrix());
            } else if (strcmp(((FilterDef*)(filters->content))->name,"SHARP")){
                convolution_matrix(&image,get_sharp_matrix());
            }

        }
        filters = filters->next;
    }

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