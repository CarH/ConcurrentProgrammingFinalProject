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
		rgb[i] = (byte**)calloc(width, sizeof(byte*));
		result[i] = (byte**)calloc(width, sizeof(byte*));
		for(j=0; j < width; j++){
			rgb[i][j] = (byte*)calloc(height, sizeof(byte));
			result[i][j] = (byte*)calloc(height, sizeof(byte));
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

	// byte c=0;
	// byte x=1, y=4.3;
	// c = (byte)x+y;
	// 			printf("%d ", c);
	unsigned int soma=0;
	printf("P3\n%d %d\n%d", width, height, maxValue);
	/// Calculo das médias:
	for(i=0; i < height; i++){
		for(j=0; j < width; j++){
			for(k=0; k < 3; k++){
				soma = 0;
				for(m=j+max(0, j-3); m<j+min(3, width-j); m++){ // para calcular a media
					for(n=i+max(0, i-3); n<i+min(3, height-i); n++){
						soma += rgb[k][n][m];
					}
				}
				// printf(" SOMA: %d\n", soma);
				result[k][i][j] = (byte)((unsigned int)soma/(SIDE*SIDE));
				printf("%3d ", (byte)result[k][i][j]);
			}
		}
		printf("\n");
	}

	/// desalocar rgb
	// for(i=0; i < 3; i++){
		// for(j=0; j < width; j++){
			// free(rgb[i][j]);
			// free(result[i][j]);
		// }
		// free(rgb[i]);
		// free(result[i]);
	// }
	return 0;
}