#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned char byte;

int max(int a, int b){
	if (a>b){return a;}
	return b;
}

int min(int a, int b){
	if (a<b){return a;}
	return b;
}

int main(){
	byte** rgb[3], **result[3];
	char format[3];
	int i, j, k, m, n, width, height,width2, height2, maxValue, value, soma;
	
	scanf("%s", format);
	scanf("%d %d %d", &width, &height, &maxValue);
	
	/// erro: formato incorreto, nao eh rgb.
	if(strcmp(format, "P3")){
		return -1;
	}
	
	/// erro: nao eh byte.
	if(maxValue > 255){
		return -1;
	}
	
	/// nova dimensao da imagem extendida
	width2 = width+4;
	height2 = height+4;
	
	/// alocar as matrizes rgb
	for(i=0; i < 3; i++){
		rgb[i] = (byte**)calloc(height2, sizeof(byte*));
		result[i] = (byte**)calloc(height, sizeof(byte*));
		for(j=0; j < height; j++){
			result[i][j] = (byte*)calloc(width, sizeof(byte));
		}
		for(j=0; j < height2; j++){
			rgb[i][j] = (byte*)calloc(width2, sizeof(byte));
		}
	}
	
	/// le os valores rgb de cada pixel da imagem
	for(i=0; i < height; i++){
		for(j=0; j < width; j++){
			for(k=0; k < 3; k++){
				scanf("%d",  &value);
				rgb[k][i+2][j+2] = (byte)value;
			}
		}
	}
	
	
	/// exibe na tela
	 /*for(i=0; i < height; i++){
	 	for(j=0; j < width; j++){
	 		for(k=0; k < 3; k++){
	 			printf("%3d ", (byte)rgb[k][i+2][j+2]);
	 		}
	 	}
	 	printf("\n");
	 }*/

	
	/// Calculo da média de cada bloco de 5x5 pixel
	for(k=0; k < 3; k++){
		for(i=0; i < height; i++){
			unsigned int inicY = i-2;
			unsigned int limitY = i+3;
			
			for(j=0; j < width; j++){
				unsigned int inicX = j-2;
				unsigned int limitX = j+3;
				
				soma=0;	
				for(m = inicX; m < limitX; m++){ // para calcular a media
					for(n = inicY; n < limitY; n++){
						soma += rgb[k][2+n][2+m];
					}
				}
				result[k][i][j] = (byte)((unsigned int)soma/25);
			}
		}
	}
	
	/// Exibe o cabecalho
	printf("P3\n%d %d\n%d\n", width, height, maxValue);
	
	/// Exibe os valores rgb de cada pixel da imagem
	for(i=0; i < height; i++){
		for(j=0; j < width; j++){
			for(k=0; k < 3; k++){
				printf("%-3d ", (byte)result[k][i][j]);
			}
		}
		printf("\n");
	}

	/// desalocar as matrizes rgb
	 for(i=0; i < 3; i++){
		 for(j=0; j < height2; j++){
			 free(rgb[i][j]);
		 }
		 for(j=0; j < height; j++){
			 free(result[i][j]);
		 }
		 free(rgb[i]);
		 free(result[i]);
	 }
	return 0;
}
