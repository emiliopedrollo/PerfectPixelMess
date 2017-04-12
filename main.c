#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include "utils.h"
#include "lists.h"
#include "image.h"
#include "conv_mat.h"
#include "filter.h"

FilterDef *extractFilterDef(char* argument);
ConvolutionMatrix * extractCustomMatrix(unsigned char size, char *arguments);
void display_usage();
void call_apply_filter(void *filter, void *image);

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
    ConvolutionMatrix *customMatrix = NULL;

    // Os blocos a seguir verificam a existência de parametros de
    // execução do executavel e definem variaveis que alterarão o
    // comportamento do mesmo de acordo com estes parametros
    static struct option long_options[] = {
            {"input",  required_argument, 0, 'i'},
            {"output", required_argument, 0, 'o'},
            {"filter", required_argument, 0, 'f'},
            {"custom", required_argument, 0, 'c'},
            {"help",   no_argument      , 0, 'h'},
            {0,        0,                 0,  0 }
    };

    while (reading) {
        int option_index = 0;
        c = getopt_long(argc, argv, "i:o:f:c:h", long_options, &option_index);

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
                    filters = list_insert(filters,extractFilterDef(*(tokens + i)));
                    free(*(tokens + i));
                }
                free(tokens);
                break;
            case 'c':
                tokens = str_split(optarg,',');
                customMatrix = extractCustomMatrix((unsigned char) atoi(*(tokens)), *(tokens + 1));
                if (*(tokens + 2)){
                    customMatrix->multiplier = (double)1/(double)atoi(*(tokens + 2));
                }
                free(tokens);
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

    if (customMatrix != NULL) {
        convolution_matrix(&image,*customMatrix);
        free(customMatrix);
    }

    list_each_extra(filters, call_apply_filter, &image);

    if (output_to_stdout){
        output(stdout,image);
    } else {
        FILE *out_file = fopen(output_to, "w");
        output(out_file,image);
        fclose(out_file);
    }

    exit(0);
}

void call_apply_filter(void *filter, void *image){
    apply_filter((FilterDef *) filter, (Image **) image);
}

ConvolutionMatrix * extractCustomMatrix(unsigned char size, char *arguments) {

    int i, j;
    char** tk1;
    char** tk2;
    ConvolutionMatrix* matrix;

    matrix = malloc(sizeof(ConvolutionMatrix));

    if ((size % 2) == 0) {
        fprintf(stderr,"Custom convolution matrix must have an odd size.");
        exit(EXIT_FAILURE);
    }

    matrix->matrix_size = size;
    matrix->multiplier = 1;

    if (!arguments || countChars(arguments,'=') == 0){
        fprintf(stderr,"Custom convolution matrix has not been defined.");
        exit(EXIT_FAILURE);
    } else {
        tk1 = str_split(arguments,'=');
        matrix->matrix = generate_matrix(size);
        tk2 = str_split(*(tk1+1) , ';');

        for (i=0;i<size;i++){
            for (j=0;j<size;j++){
                if (!*(tk2+(i*size+j))){
                    fprintf(stderr,"Custom convolution matrix informed does not match it's size.");
                    exit(EXIT_FAILURE);
                }
                matrix->matrix[i][j] = (short) atoi(*(tk2+(i*size+j)));
            }
        }

        free(tk1);
        free(tk2);
    }

    return matrix;
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
        filter->params = str_split(params_raw,';');
        free(tokens);
        free(params_raw);
    }

    return filter;
}

void display_usage() {
    printf("Usage: ppm [options]...\n"
    "Apply filters to ppm images\n"
    "\n"
    "Mandatory arguments to long options are mandatory for short options too.\n"
    "  -i --input=FILE              Use FILE as input stream. Otherwise ppm uses stdin.\n"
    "  -o --output=FILE             Outputs resulted image to FILE. Otherwise prints to stdout.\n"
    "  -f --filter=F1,F2=1;7        Sets the filter to use on the input image. Where F1 represent \n"
    "                                 one of the filters listed bellow, F2=1 represent another with 1\n"
    "                                 as an argument. Filters must be separated by commas. Arguments, \n"
    "                                 if more than one must be separated by semicolons.\n"
    "  -c --custom=S,M=1;1;1...[,D] Runs a custom user defined convolution matrix where S is the matrix\n"
    "                                 size (must be an odd number) and M defines the matrix values\n"
    "                                 separated by semicolons. Optionally can be defined a divider to be\n"
    "                                 applied to the sum.\n"
    "\n"
    "\n"
    "The available filters are:\n"
    "  INVERT       Inverts the image\n"
    "  RGB=R[;G;B]  Color adjustment needs 1 or 3 integers ranging from -255 to 255 as parameter\n"
    "  GREYSCALE    Converts a colored image to greyscale\n"
    "  ROTATE=angle Rotates the image, acceptable angles are 90, 180, 270 and 360\n"
    "  FLIP         Inverts the image upside down\n"
    "  MIRROR       Inverts the image sideways\n"
    "  BLUR[=S]     Blurs the image using a S size pixel matrix as sample\n"
    "  BORDER       Border detection\n"
    "  SHARP        Sharpens the image\n"
    "\n");
}