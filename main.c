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
void convolution_matrix(Image **image, ConvolutionMatrix filter);
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
                    filters = list_insert(filters,extractFilterDef(*(tokens + i)));
                    free(*(tokens + i));
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