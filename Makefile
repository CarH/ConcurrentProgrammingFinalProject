### MAKEFILE ###

all: compile run

runseq:
	@gcc smooth_seq.c -o smooth_seq
	@ ./smooth_seq < ./in/2.ppm > ./out/2.ppm
	
runpar1:
	@gcc smooth_par1.c -o smooth_par1
	@ ./smooth_par1 < ./in/in.ppm > ./out/in.ppm	

example1:
	@nvcc example1.c -o example1
	@./example1

zip:
	@zip ./"backup/t4_"`date +"%d-%m-%Y_%H:%M"` ./Makefile ./*.cpp ./*.c *.txt
