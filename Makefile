### MAKEFILE ###

runseq:
	@gcc smooth_seq.c -o smooth_seq
	@ ./smooth_seq < ./in/P3.ppm >./out/P3.ppm
	
runpar:
	@rm -f ./out/*.ppm *.tar.gz
	@nvcc smooth_cuda.cu -o smooth_cuda
	@./smooth_cuda 1 < ./in/2.ppm > ./out/2.ppm
	@tar -cvzf ./out/out.tar.gz ./out/*.ppm
	
cuda:
	@cp smooth_cuda.c smooth_cuda.cu
	scp -P 2010 Makefile ./*.cu grupo-08a@andromeda.lasdpc.icmc.usp.br:/home/grupo-08a/

imagesu:
	scp -P 2010 ./in/in.ppm grupo-08a@andromeda.lasdpc.icmc.usp.br:/home/grupo-08a/in/

imagesz:
	tar -cvzf ./out/out.tar.gz ./out/*.ppm
	
imagesd:
	scp -P 2010 grupo-08a@andromeda.lasdpc.icmc.usp.br:/home/grupo-08a/out/*.tar.gz ./out/
	tar -zxvf ./out/out.tar.gz
	
zip:
	@zip ./"backup/t4_"`date +"%d-%m-%Y_%H:%M"` ./Makefile ./*.c ./*.cu *.txt
