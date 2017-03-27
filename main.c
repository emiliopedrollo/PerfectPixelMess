#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{
int r;
int g;
int b;
}p;

p** abre(int argc, char* argv[], int* a, int* l,int* m){
  int i,j;
  p** pixel;
  FILE* arq;
  arq=fopen(argv[1],"r");
  char tst[5];
  if(arq != NULL){
    fscanf(arq,"%s",tst);
    fscanf(arq,"%d",a);
    fscanf(arq,"%d",l);
    fscanf(arq,"%d",m);
    pixel= (p**)malloc((*a)*sizeof(p));
    
    for(i=0;i<*a;i++){
       pixel[i]=(p*)malloc((*l)*sizeof(p));
       for(j=0;j<*l;j++){
        	fscanf(arq,"%d",&pixel[i][j].r);
        	fscanf(arq,"%d",&pixel[i][j].g);
        	fscanf(arq,"%d",&pixel[i][j].b);
       }
	}
  }
  fclose(arq);
  return pixel;
}

void salva(int argc, char* argv[],p** pixel ,int* a, int* l,int* m){
  int i,j;
  FILE* arq;
  
  arq = fopen(argv[2], "w");

    fprintf (arq, "P3\n");
    fprintf (arq, "%d ", *a);
    fprintf (arq, "%d\n", *l);
    fprintf (arq, "%d\n", *m);

    for (i = 0; i < *a; i++) {
        for (j = 0; j < *l; j++) {
            fprintf(arq, "%d ", pixel[i][j].r);
            fprintf(arq, "%d ", pixel[i][j].g);
            fprintf(arq, "%d\n", pixel[i][j].b);
        }
    }

    fclose(arq);
    
}

void negativo(p** pixel ,int* a, int* l,int* m){
	int i,j;
	for(i=0;i<*a;i++){
       for(j=0;j<*l;j++){
           pixel[i][j].r=255-pixel[i][j].r;
		   pixel[i][j].g=255-pixel[i][j].g;
		   pixel[i][j].b=255-pixel[i][j].b;
       }
   }
}

void matriz_convolucao(p** pixel ,int* a, int* l,int* m){
	int mult[3][3];
	p temp[3][3];
	int i,j,teste=0;
	
	mult[0][0]=0;
	mult[0][1]=0;
	mult[0][2]=0;
	mult[1][0]=0;
	mult[1][1]=1;
	mult[1][2]=0;
	mult[2][0]=0;
	mult[2][1]=0;
	mult[2][2]=0;
	
	for(i=0;i<*a;i++){
       for(j=0;j<*l;j++){
           if(i==0 && j==0 && teste==0){
           		temp[0][0]=pixel[i][j];
				temp[0][1]=pixel[i][j];
				temp[0][2]=pixel[i][j];
				temp[1][0]=pixel[i][j];
				temp[1][1]=pixel[i][j];
				temp[1][2]=pixel[i][j+1];
				temp[2][0]=pixel[i][j];
				temp[2][1]=pixel[i+1][j];
				temp[2][2]=pixel[i+1][j+1];
				teste=1;
		   }
		   if(i==0 && j==(*l-1) && teste==0){
           		temp[0][0]=pixel[i][j];
				temp[0][1]=pixel[i][j];
				temp[0][2]=pixel[i][j];
				temp[1][0]=pixel[i][j-1];
				temp[1][1]=pixel[i][j];
				temp[1][2]=pixel[i][j];
				temp[2][0]=pixel[i+1][j-1];
				temp[2][1]=pixel[i+1][j];
				temp[2][2]=pixel[i+1][j];
				teste=1;
		   }
		   if(i==(*a-1) && j==0 && teste==0){
           		temp[0][0]=pixel[i][j];
				temp[0][1]=pixel[i-1][j];
				temp[0][2]=pixel[i-1][j+1];
				temp[1][0]=pixel[i][j];
				temp[1][1]=pixel[i][j];
				temp[1][2]=pixel[i][j+1];
				temp[2][0]=pixel[i][j];
				temp[2][1]=pixel[i][j];
				temp[2][2]=pixel[i][j];
				teste=1;
		   }
		   if(i==(*a-1) && j==(*l-1) && teste==0){
           		temp[0][0]=pixel[i-1][j-1];
				temp[0][1]=pixel[i-1][j];
				temp[0][2]=pixel[i][j];
				temp[1][0]=pixel[i][j-1];
				temp[1][1]=pixel[i][j];
				temp[1][2]=pixel[i][j];
				temp[2][0]=pixel[i][j];
				temp[2][1]=pixel[i][j];
				temp[2][2]=pixel[i][j];
				teste=1;
		   }
		   if(i==0 && teste==0){
           		temp[0][0]=pixel[i][j];
				temp[0][1]=pixel[i][j];
				temp[0][2]=pixel[i][j];
				temp[1][0]=pixel[i][j-1];
				temp[1][1]=pixel[i][j];
				temp[1][2]=pixel[i][j+1];
				temp[2][0]=pixel[i+1][j-1];
				temp[2][1]=pixel[i+1][j];
				temp[2][2]=pixel[i+1][j+1];
				teste=1;
		   }
		   if(i==(*a-1) && teste==0){
           		temp[0][0]=pixel[i-1][j-1];
				temp[0][1]=pixel[i-1][j];
				temp[0][2]=pixel[i-1][j+1];
				temp[1][0]=pixel[i][j-1];
				temp[1][1]=pixel[i][j];
				temp[1][2]=pixel[i][j+1];
				temp[2][0]=pixel[i][j];
				temp[2][1]=pixel[i][j];
				temp[2][2]=pixel[i][j];
				teste=1;
		   }
		   if(j==0 && teste==0){
           		temp[0][0]=pixel[i][j];
				temp[0][1]=pixel[i-1][j];
				temp[0][2]=pixel[i-1][j+1];
				temp[1][0]=pixel[i][j];
				temp[1][1]=pixel[i][j];
				temp[1][2]=pixel[i][j+1];
				temp[2][0]=pixel[i][j];
				temp[2][1]=pixel[i+1][j];
				temp[2][2]=pixel[i+1][j+1];
				teste=1;
		   }
		   if(j==(*l-1) && teste==0){
           		temp[0][0]=pixel[i-1][j-1];
				temp[0][1]=pixel[i-1][j];
				temp[0][2]=pixel[i][j];
				temp[1][0]=pixel[i][j-1];
				temp[1][1]=pixel[i][j];
				temp[1][2]=pixel[i][j];
				temp[2][0]=pixel[i+1][j-1];
				temp[2][1]=pixel[i+1][j];
				temp[2][2]=pixel[i][j];
				teste=1;
		   }
		   if(teste != 1){
           		temp[0][0]=pixel[i-1][j-1];
				temp[0][1]=pixel[i-1][j];
				temp[0][2]=pixel[i-1][j+1];
				temp[1][0]=pixel[i][j-1];
				temp[1][1]=pixel[i][j];
				temp[1][2]=pixel[i][j+1];
				temp[2][0]=pixel[i+1][j-1];
				temp[2][1]=pixel[i+1][j];
				temp[2][2]=pixel[i+1][j+1];
				teste=1;
		   }
		    //printf("%d %d %d\n",temp[1][1].r,temp[1][1].g,temp[1][1].b);
			pixel[i][j].r=(temp[0][0].r*mult[0][0]+temp[0][1].r*mult[0][1]+temp[0][2].r*mult[0][2]+temp[1][0].r*mult[1][0]+temp[1][1].r*mult[1][1]+temp[1][2].r*mult[1][2]+temp[2][0].r*mult[2][0]+temp[2][1].r*mult[2][1]+temp[2][2].r*mult[2][2])/(mult[0][0]+mult[0][1]+mult[0][2]+mult[1][0]+mult[1][1]+mult[1][2]+mult[2][0]+mult[2][1]+mult[2][2]);
			pixel[i][j].g=(temp[0][0].g*mult[0][0]+temp[0][1].g*mult[0][1]+temp[0][2].g*mult[0][2]+temp[1][0].g*mult[1][0]+temp[1][1].g*mult[1][1]+temp[1][2].g*mult[1][2]+temp[2][0].g*mult[2][0]+temp[2][1].g*mult[2][1]+temp[2][2].g*mult[2][2])/(mult[0][0]+mult[0][1]+mult[0][2]+mult[1][0]+mult[1][1]+mult[1][2]+mult[2][0]+mult[2][1]+mult[2][2]);
			pixel[i][j].b=(temp[0][0].b*mult[0][0]+temp[0][1].b*mult[0][1]+temp[0][2].b*mult[0][2]+temp[1][0].b*mult[1][0]+temp[1][1].b*mult[1][1]+temp[1][2].b*mult[1][2]+temp[2][0].b*mult[2][0]+temp[2][1].b*mult[2][1]+temp[2][2].b*mult[2][2])/(mult[0][0]+mult[0][1]+mult[0][2]+mult[1][0]+mult[1][1]+mult[1][2]+mult[2][0]+mult[2][1]+mult[2][2]);
			teste=0;
       }
	}
    for(i=0;i<*a;i++){
     	for(j=0;j<*l;j++){
     		if(pixel[i][j].r<0){
				pixel[i][j].r=0;
			}
       		if(pixel[i][j].g<0){
		   		pixel[i][j].g=0;
			}
			if(pixel[i][j].b<0){
		   		pixel[i][j].b=0;
			}
       		if(pixel[i][j].r>*m){
				pixel[i][j].r=*m;
			}
       		if(pixel[i][j].g>*m){
		   		pixel[i][j].g=*m;
			}
			if(pixel[i][j].b>*m){
		   		pixel[i][j].b=*m;
			}
       	}
    }
}

void aumentar_brilho(p** pixel ,int* a, int* l,int* m){
	int i,j;
		
	for(i=0;i<*a;i++){
       for(j=0;j<*l;j++){
            pixel[i][j].r=pixel[i][j].r+60;
		    pixel[i][j].g=pixel[i][j].g+60;
		    pixel[i][j].b=pixel[i][j].b+60;
       }
   }
   
   for(i=0;i<*a;i++){
     	for(j=0;j<*l;j++){
       		if(pixel[i][j].r>*m){
				pixel[i][j].r=*m;
			}
       		if(pixel[i][j].g>*m){
		   		pixel[i][j].g=*m;
			}
			if(pixel[i][j].b>*m){
		   		pixel[i][j].b=*m;
			}
       	}
    }
}

void diminuir_brilho(p** pixel ,int* a, int* l,int* m){
	int i,j;
		
	for(i=0;i<*a;i++){
       for(j=0;j<*l;j++){
            pixel[i][j].r=pixel[i][j].r-60;
		    pixel[i][j].g=pixel[i][j].g-60;
		    pixel[i][j].b=pixel[i][j].b-60;
       }
   }
   
   for(i=0;i<*a;i++){
     	for(j=0;j<*l;j++){
			if(pixel[i][j].r<0){
				pixel[i][j].r=0;
			}
       		if(pixel[i][j].g<0){
		   		pixel[i][j].g=0;
			}
			if(pixel[i][j].b<0){
		   		pixel[i][j].b=0;
			}
       	}
    }
}


main(int argc, char* argv[]){
  p** pixel;
  int a,i,j,l,m;
  
  pixel=abre(argc,argv,&a,&l,&m);
  
  //negativo(pixel,&a,&l,&m);
  
  //aumentar_brilho(pixel,&a,&l,&m);
  
  diminuir_brilho(pixel,&a,&l,&m);

  //matriz_convolucao(pixel ,&a, &l,&m);
  
  salva(argc,argv,pixel,&a,&l,&m);
}
