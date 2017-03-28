#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include "main.h"

Image *load_image(FILE *stream);

int main (int argc, char *argv[]) {

    int c;
    bool reading = true;
    bool read_from_stdin = true;
    char *read_from = NULL;
    Image *image;

    // Os blocos a seguir verificam a existência de parametros de
    // execução do executavel e definem variaveis que alterarão o
    // comportamento do mesmo de acordo com estes parametros
    static struct option long_options[] = {
            {"input",  required_argument, 0, 'o'},
            {"output", required_argument, 0, 'i'},
            {0,        0,                 0,  0 }
    };

    while (reading) {
        int option_index = 0;
        c = getopt_long(argc, argv, "i:o:", long_options, &option_index);

        switch (c) {
            case 'i':
                read_from_stdin = false;
                read_from = malloc((strlen(optarg)+1) * sizeof(char));
                strcpy(read_from,optarg);
                break;
            case -1:
                reading = false;
                break;
            default:
                break;
        }
    }

    if (read_from_stdin){
        image = load_image(stdin);
    } else if (read_from != NULL) {
        FILE *file = fopen(read_from, "r");
        image = load_image(file);
        fclose(file);
    }


//    int a, i, j, l, m;

//    pixel = load_from_file();



    //negativo(pixel,&a,&l,&m);

    //aumentar_brilho(pixel,&a,&l,&m);

//    diminuir_brilho(pixel, &a, &l, &m);

    //matriz_convolucao(pixel ,&a, &l,&m);

//    salva(argc, argv, pixel, &a, &l, &m);

    exit(0);
}


Image *load_image(FILE *stream) {
    int i, j;

    Image *image = NULL;

    char format[5];
    if (stream != NULL) {
        //TODO: Treat invalid stream
        //TODO: Treat comment lines
        //TODO: Treat data too short

        fscanf(stream, "%s\n", format);
        if (strcmp(format,"P3") != 0){
            fprintf(stderr,"Input is not in a valid format. Please use PPM P3.");
            exit(EXIT_FAILURE);
        }

        image = malloc(sizeof (Image));
        fscanf(stream, "%d %d\n", &image->height, &image->width);
        fscanf(stream, "%d\n", &image->max_bright);

        image->matrix = (Pixel**) malloc(image->height * sizeof(Pixel*));
        for (i=0;i<image->height;i++){
            image->matrix[i] = (Pixel*) malloc(image->width * sizeof(Pixel));
            for (j=0;j<image->width;j++){
                fscanf(stream, "%hhu", &image->matrix[i][j].r);
                fscanf(stream, "%hhu", &image->matrix[i][j].g);
                fscanf(stream, "%hhu", &image->matrix[i][j].b);
            }
        }
    }
    return image;
}

void salva(int argc, char *argv[], Pixel **pixel, int *a, int *l, int *m) {
    int i, j;
    FILE *arq;

    arq = fopen(argv[2], "w");

    fprintf(arq, "P3\n");
    fprintf(arq, "%d ", *a);
    fprintf(arq, "%d\n", *l);
    fprintf(arq, "%d\n", *m);

    for (i = 0; i < *a; i++) {
        for (j = 0; j < *l; j++) {
            fprintf(arq, "%d ", pixel[i][j].r);
            fprintf(arq, "%d ", pixel[i][j].g);
            fprintf(arq, "%d\n", pixel[i][j].b);
        }
    }

    fclose(arq);

}

void negativo(Pixel **pixel, int *a, int *l, int *m) {
    int i, j;
    for (i = 0; i < *a; i++) {
        for (j = 0; j < *l; j++) {
            pixel[i][j].r = 255 - pixel[i][j].r;
            pixel[i][j].g = 255 - pixel[i][j].g;
            pixel[i][j].b = 255 - pixel[i][j].b;
        }
    }
}

void matriz_convolucao(Pixel **pixel, int *a, int *l, int *m) {
    int mult[3][3];
    Pixel temp[3][3];
    int i, j, teste = 0;

    mult[0][0] = 0;
    mult[0][1] = 0;
    mult[0][2] = 0;
    mult[1][0] = 0;
    mult[1][1] = 1;
    mult[1][2] = 0;
    mult[2][0] = 0;
    mult[2][1] = 0;
    mult[2][2] = 0;

    for (i = 0; i < *a; i++) {
        for (j = 0; j < *l; j++) {
            if (i == 0 && j == 0 && teste == 0) {
                temp[0][0] = pixel[i][j];
                temp[0][1] = pixel[i][j];
                temp[0][2] = pixel[i][j];
                temp[1][0] = pixel[i][j];
                temp[1][1] = pixel[i][j];
                temp[1][2] = pixel[i][j + 1];
                temp[2][0] = pixel[i][j];
                temp[2][1] = pixel[i + 1][j];
                temp[2][2] = pixel[i + 1][j + 1];
                teste = 1;
            }
            if (i == 0 && j == (*l - 1) && teste == 0) {
                temp[0][0] = pixel[i][j];
                temp[0][1] = pixel[i][j];
                temp[0][2] = pixel[i][j];
                temp[1][0] = pixel[i][j - 1];
                temp[1][1] = pixel[i][j];
                temp[1][2] = pixel[i][j];
                temp[2][0] = pixel[i + 1][j - 1];
                temp[2][1] = pixel[i + 1][j];
                temp[2][2] = pixel[i + 1][j];
                teste = 1;
            }
            if (i == (*a - 1) && j == 0 && teste == 0) {
                temp[0][0] = pixel[i][j];
                temp[0][1] = pixel[i - 1][j];
                temp[0][2] = pixel[i - 1][j + 1];
                temp[1][0] = pixel[i][j];
                temp[1][1] = pixel[i][j];
                temp[1][2] = pixel[i][j + 1];
                temp[2][0] = pixel[i][j];
                temp[2][1] = pixel[i][j];
                temp[2][2] = pixel[i][j];
                teste = 1;
            }
            if (i == (*a - 1) && j == (*l - 1) && teste == 0) {
                temp[0][0] = pixel[i - 1][j - 1];
                temp[0][1] = pixel[i - 1][j];
                temp[0][2] = pixel[i][j];
                temp[1][0] = pixel[i][j - 1];
                temp[1][1] = pixel[i][j];
                temp[1][2] = pixel[i][j];
                temp[2][0] = pixel[i][j];
                temp[2][1] = pixel[i][j];
                temp[2][2] = pixel[i][j];
                teste = 1;
            }
            if (i == 0 && teste == 0) {
                temp[0][0] = pixel[i][j];
                temp[0][1] = pixel[i][j];
                temp[0][2] = pixel[i][j];
                temp[1][0] = pixel[i][j - 1];
                temp[1][1] = pixel[i][j];
                temp[1][2] = pixel[i][j + 1];
                temp[2][0] = pixel[i + 1][j - 1];
                temp[2][1] = pixel[i + 1][j];
                temp[2][2] = pixel[i + 1][j + 1];
                teste = 1;
            }
            if (i == (*a - 1) && teste == 0) {
                temp[0][0] = pixel[i - 1][j - 1];
                temp[0][1] = pixel[i - 1][j];
                temp[0][2] = pixel[i - 1][j + 1];
                temp[1][0] = pixel[i][j - 1];
                temp[1][1] = pixel[i][j];
                temp[1][2] = pixel[i][j + 1];
                temp[2][0] = pixel[i][j];
                temp[2][1] = pixel[i][j];
                temp[2][2] = pixel[i][j];
                teste = 1;
            }
            if (j == 0 && teste == 0) {
                temp[0][0] = pixel[i][j];
                temp[0][1] = pixel[i - 1][j];
                temp[0][2] = pixel[i - 1][j + 1];
                temp[1][0] = pixel[i][j];
                temp[1][1] = pixel[i][j];
                temp[1][2] = pixel[i][j + 1];
                temp[2][0] = pixel[i][j];
                temp[2][1] = pixel[i + 1][j];
                temp[2][2] = pixel[i + 1][j + 1];
                teste = 1;
            }
            if (j == (*l - 1) && teste == 0) {
                temp[0][0] = pixel[i - 1][j - 1];
                temp[0][1] = pixel[i - 1][j];
                temp[0][2] = pixel[i][j];
                temp[1][0] = pixel[i][j - 1];
                temp[1][1] = pixel[i][j];
                temp[1][2] = pixel[i][j];
                temp[2][0] = pixel[i + 1][j - 1];
                temp[2][1] = pixel[i + 1][j];
                temp[2][2] = pixel[i][j];
                teste = 1;
            }
            if (teste != 1) {
                temp[0][0] = pixel[i - 1][j - 1];
                temp[0][1] = pixel[i - 1][j];
                temp[0][2] = pixel[i - 1][j + 1];
                temp[1][0] = pixel[i][j - 1];
                temp[1][1] = pixel[i][j];
                temp[1][2] = pixel[i][j + 1];
                temp[2][0] = pixel[i + 1][j - 1];
                temp[2][1] = pixel[i + 1][j];
                temp[2][2] = pixel[i + 1][j + 1];
                teste = 1;
            }
            //printf("%d %d %d\n",temp[1][1].r,temp[1][1].g,temp[1][1].b);
            pixel[i][j].r = (temp[0][0].r * mult[0][0] + temp[0][1].r * mult[0][1] + temp[0][2].r * mult[0][2] +
                             temp[1][0].r * mult[1][0] + temp[1][1].r * mult[1][1] + temp[1][2].r * mult[1][2] +
                             temp[2][0].r * mult[2][0] + temp[2][1].r * mult[2][1] + temp[2][2].r * mult[2][2]) /
                            (mult[0][0] + mult[0][1] + mult[0][2] + mult[1][0] + mult[1][1] + mult[1][2] + mult[2][0] +
                             mult[2][1] + mult[2][2]);
            pixel[i][j].g = (temp[0][0].g * mult[0][0] + temp[0][1].g * mult[0][1] + temp[0][2].g * mult[0][2] +
                             temp[1][0].g * mult[1][0] + temp[1][1].g * mult[1][1] + temp[1][2].g * mult[1][2] +
                             temp[2][0].g * mult[2][0] + temp[2][1].g * mult[2][1] + temp[2][2].g * mult[2][2]) /
                            (mult[0][0] + mult[0][1] + mult[0][2] + mult[1][0] + mult[1][1] + mult[1][2] + mult[2][0] +
                             mult[2][1] + mult[2][2]);
            pixel[i][j].b = (temp[0][0].b * mult[0][0] + temp[0][1].b * mult[0][1] + temp[0][2].b * mult[0][2] +
                             temp[1][0].b * mult[1][0] + temp[1][1].b * mult[1][1] + temp[1][2].b * mult[1][2] +
                             temp[2][0].b * mult[2][0] + temp[2][1].b * mult[2][1] + temp[2][2].b * mult[2][2]) /
                            (mult[0][0] + mult[0][1] + mult[0][2] + mult[1][0] + mult[1][1] + mult[1][2] + mult[2][0] +
                             mult[2][1] + mult[2][2]);
            teste = 0;
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