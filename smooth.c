#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIDE 5

typedef unsigned char byte;

int max(int a, int b){
	if (a>b){return a;}
	return b;
	// return a>b?a:b;
}

int min(int a, int b){
	if (a<b){return a;}
	return b;
	// return a<b?a:b;
}

int main(){
	byte** rgb[3], **result[3];
	char format[3];
	int i, j, k, m, n, width, height, maxValue, value;
	
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
	
	/// alocar rgb
	for(i=0; i < 3; i++){
		rgb[i] = (byte**)calloc(height, sizeof(byte*));
		result[i] = (byte**)calloc(height, sizeof(byte*));
		for(j=0; j < height; j++){
			rgb[i][j] = (byte*)calloc(width, sizeof(byte));
			result[i][j] = (byte*)calloc(width, sizeof(byte));
		}
	}
	
	/// le os valores
	for(i=0; i < height; i++){
		for(j=0; j < width; j++){
			for(k=0; k < 3; k++){
				scanf("%d",  &value);
				rgb[k][i][j] = (byte)value;
			}
		}
	}
	
	/// exibe na tela
	// for(i=0; i < height; i++){
	// 	for(j=0; j < width; j++){
	// 		for(k=0; k < 3; k++){
	// 			printf("%3d ", (byte)rgb[k][i][j]);
	// 		}
	// 	}
	// 	printf("\n");
	// }


	printf("P3\n%d %d\n%d\n", width, height, maxValue);
	unsigned int soma = 0;
	
	/// Calculo das médias:
	for(i=0; i < height; i++){
		for(j=0; j < width; j++){
			for(k=0; k < 3; k++){
				
				unsigned int limitX = j+min(3, width-j);
				unsigned int limitY = i+min(3, height-i);
				
				for(m = max(0, j-2); m < limitX; m++){ // para calcular a media
					for(n = max(0, i-2); n < limitY; n++){
						soma += rgb[k][n][m];
					}
				}
				
				///printf("%2d %2d - %2d %2d\n",inicX, limitX, inicY, limitY);
				/// printf("%2d %2d : %d\n", j, i, soma);
				
				result[k][i][j] = (byte)((unsigned int)soma/25);
				printf("%-3d ", (byte)result[k][i][j]);
			}
		}
		printf("\n");
	}

	/// desalocar rgb
	 for(i=0; i < 3; i++){
		 for(j=0; j < height; j++){
			 free(rgb[i][j]);
			 free(result[i][j]);
		 }
		 free(rgb[i]);
		 free(result[i]);
	 }
	return 0;
}
