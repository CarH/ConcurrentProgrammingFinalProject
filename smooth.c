#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;

int main(){
	byte** rgb[3];
	char format[3];
	int i, j, k,width, height, max, value;
	
	scanf("%s", format);
	scanf("%d %d %d", &width, &height, &max);
	
	/// erro: formato incorreto, nao eh rgb.
	if(strcmp(format, "P3")){
		return -1;
	}
	
	/// erro: nao eh byte.
	if(max > 255){
		return -1;
	}
	
	/// alocar rgb
	for(i=0; i < 3; i++){
		rgb[i] = (byte**)calloc(width, sizeof(byte*));
		for(j=0; j < width; j++){
			rgb[i][j] = (byte*)calloc(height, sizeof(byte));
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
	for(i=0; i < height; i++){
		for(j=0; j < width; j++){
			for(k=0; k < 3; k++){
				printf("%3d ", (unsigned char)rgb[k][i][j]);
			}
		}
		printf("\n");
	}
	
	/// desalocar rgb
	for(i=0; i < 3; i++){
		for(j=0; j < width; j++){
			free(rgb[i][j]);
		}
		free(rgb[i]);
	}
	
	return 0;
}
