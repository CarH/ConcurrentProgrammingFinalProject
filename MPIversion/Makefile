# Makefile
# Para compilar:
# 	mpicc aquivo.c -o arquivo

# Para executar:
# 	mpirun -np <n> arquivo

# Para passar os hosts na execução:
# 	mpirun --hosts node01,node02,...,node0n -np <n> arquivo

# Para passar o arquivo de hosts na execução:
# 	mpirun --hostfile <arquivo de hosts> -np <n> arquivo

all: compile run

compile:
	@mpicc smooth_mpi.c -o smooth_mpi


run:
	@mpirun -np 10 smooth_mpi


upload:
	scp -P 22200 *.c *.h Makefile grupo08a@cluster.lasdpc.icmc.usp.br:/home/grupo08a/proj3