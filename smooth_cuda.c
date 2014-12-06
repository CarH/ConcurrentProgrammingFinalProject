#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define NTEST 10

__global__ void kernel(int width, int height, unsigned char *rgb_, unsigned char *out_) {
    /**
     * threadIdx.x contém o Id da thread a ser executada
     * blockIdx.x contém o Id do bloco
     * blockDim.x cte. que contém o no. de threads utilizadas em cada bloco
     * gridDim.x cte que contém o numero de blocos utilizados em um grid
     */
     
     int n, m;
     
	/// calula a coordenada do pixel atual na imagem
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
		
	/// retorna caso o limite da imagem seja ultrapassado
	if(i >= width)
		return;
	if(j >= height)
		return;
			
	/// calcula o somatório do valor dos pixels que estão dentro do bloco 5x5 do pixel (i, j)
	int soma=0;
	for(n = j-2; n < j+3; n++){
		for(m = i-2; m < i+3; m++){ 
			soma += rgb_[(n + 2) * (width + 4) + m + 2];
		}
	}
		
	/// calcula o valor final do pixel (i, j)
	out_[j*width + i] = (unsigned char)((unsigned int)soma/25);
	
	/*if(threadIdx.x < blockDim.x-1 && threadIdx.y < blockDim.y-1){
		out_[j*width+i] = (unsigned char)((unsigned int)soma/25);
	}else{
		out_[j*width+i] = (unsigned char)0;
	}*/
}


/// executa o smooth paralelo na gpu, para cada matriz de cor (r, g, b)
void execPar(int width, int height, int block_x, unsigned char *rgb_[3], unsigned char *out_[3]) {
	int k;
	
	for(k=0; k < 3; k++){
		/// define o número de threads por bloco, 32 x 32 = 1024 threads
		dim3 threadsPerBlock(block_x, block_x);
		/// define o número de blocos necessários para executar o kernel
		dim3 numBlocks(ceil((float)width / threadsPerBlock.x), ceil((float)height / threadsPerBlock.y));
								
		/// executa o 'kernel' na gpu para cada matriz de cor (r, g, b)
		kernel<<<numBlocks, threadsPerBlock>>>(width, height, rgb_[k], out_[k]);
	}
}

/// executa smooth sequencial na cpu, para cada matriz de cor (r, g, b)
void execSeq(int width, int height, unsigned char *rgb_[3], unsigned char *out_[3]) {
	int i, j, m, n, k, soma;
	int width2 = width + 4;
	
	for(k=0; k < 3; k++){
		for(i=0; i < height; i++){
			unsigned int inicY = i-2;
			unsigned int limitY = i+3;
									
			for(j=0; j < width; j++){
				unsigned int inicX = j-2;
				unsigned int limitX = j+3;
										
				soma=0;	
				for(n = inicY; n < limitY; n++){
					for(m = inicX; m < limitX; m++){
						soma += rgb_[k][(n + 2) * width2 + m + 2];
					}
				}
				out_[k][i*width + j] = (unsigned char)((unsigned int)soma/25);
			}
		}
	}
}

double deviation_calc(double* trials, double mean){
	int i;
	double local_std_dev=0.0;
	for(i=0; i < NTEST; i++){
		local_std_dev += ((trials[i] - mean) * (trials[i] - mean))/NTEST;
	}
	return sqrt(local_std_dev);
}

int main(int argv, const char* args[]){
	char format[3];
	int i, j, k, m, n, width, height, maxValue, value, test = 0, cuda;
	unsigned char* rgb[3], *out[3], *rgb_[3],*out_[3];
	double time_start, time_end,
			avg_time_[2][32],
			trials_[2][NTEST];
	struct timeval tv;
	struct timezone tz;
	
	if(argv > 1){
		test = atoi(args[1]);
	}
	
	/// leitura do cabecario do arquivo
	scanf("%s", format);
	scanf("%d %d %d", &width, &height, &maxValue);
	
	/// erro: formato incorreto, nao é rgb.
	if(strcmp(format, "P3")){
		return -1;
	}
	
	/// erro: nao é unsigned char.
	if(maxValue > 255){
		return -1;
	}
	
	int width2 = width+4;
	int height2 = height+4;
	
	/// alocar as matrizes rgb
	for(i=0; i < 3; i++){
		rgb[i] = (unsigned char*)calloc(height2*width2, sizeof(unsigned char));
		out[i] = (unsigned char*)calloc(height*width, sizeof(unsigned char));
	}
	
	/// le os valores rgb de cada pixel da imagem e coloca na matriz correspondente
	for(i=0; i < height; i++){
		for(j=0; j < width; j++){
			for(k=0; k < 3; k++){
				scanf("%d",  &value);
				rgb[k][(i+2)*width2 + j + 2] = (unsigned char)value;
			}
		}
	}

	for(i=0; i < 3; i++){
		/// aloca a memória necessária para a matriz na gpu.
		cudaMalloc(( void**)&(out_[i]), width * height * sizeof(unsigned char));
		cudaMalloc(( void**)&(rgb_[i]), width2 * height2 * sizeof(unsigned char));
		
		/// copia os dados da matriz para a memória alocada na gpu
		cudaMemcpy( rgb_[i], rgb[i], width2 * height2 * sizeof(unsigned char),cudaMemcpyHostToDevice);
	}
	
	for(k = 0; k < 32; k++){
		avg_time_[0][k] = 0;
		avg_time_[1][k] = 0;
	}
	
	if(test == 1){
		/// executa os testes, comparando o tempo de execução da versão cuda com a sequencial.
		for(cuda=0; cuda <= 1; cuda++){
			for(n = 0; n < NTEST; n++){
				
				/// testa com blocos de tamanho diferente
				for(m = 0; m < 32; m++){
							
					if(cuda == 1){
						cudaDeviceSynchronize();
						gettimeofday(&tv, &tz);
						time_start = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
					
						/// executa o versão paralela
						execPar(width, height, m+1, rgb_, out_);
						cudaDeviceSynchronize();	
					}else{
						gettimeofday(&tv, &tz);
						time_start = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
						
						/// executa a versão sequencial
						execSeq(width, height, rgb, out);
					}
					
					gettimeofday(&tv, &tz);
					time_end = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
					
					trials_[cuda][m] = (double) (time_end - time_start);
					avg_time_[cuda][m] += (double) (time_end - time_start)/NTEST;
				}
			}
		}
	}else{
		/// executa o versão paralela
		execPar(width, height, 32, rgb_, out_);
	}
	
	
	
	/// copia as matrizes resultado da gpu para a heap
	for(i=0; i < 3; i++){
		cudaMemcpy(out[i], out_[i], width * height * sizeof(unsigned char),cudaMemcpyDeviceToHost);
	}
	
	if(test == 0){
		/// Exibe o cabecalho
		printf("P3\n%d %d\n%d\n", width, height, maxValue);
		
		/// exibe as matrizes resultado na tela
		 for(i=0; i < height; i++){
			for(j=0; j < width; j++){
				for(k=0; k < 3; k++){
					printf("%d ", (unsigned char)out[k][i*width + j]);
				}
			}
			printf("\n");
		 }
	}else{
		/// Exibe o resultado dos testes
		for(i=0; i < 32; i++){
			printf("%2d %.10f %.10f %.10f %.10f %.10f\n", i+1, avg_time_[0][i], deviation_calc(trials_[0], avg_time_[0][i]), avg_time_[1][i], deviation_calc(trials_[1], avg_time_[1][i]), avg_time_[0][i]/avg_time_[1][i]);
		}
	}
	 
	 /// desaloca as matrizes da gpu
	for(i=0; i < 3; i++){
		cudaFree(rgb_[i]);
		cudaFree(out_[i]);
	}
	
	/// desaloca as matrizes da heap
	 for(i=0; i < 3; i++){
		free(rgb[i]);
		free(out[i]);
	 }
	 
	return 0;
}
