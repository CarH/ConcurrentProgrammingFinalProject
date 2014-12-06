# Makefile
# Para compilar:
# 	mpicc aquivo.c -o arquivo

# Para executar:
# 	mpirun -np <n> arquivo

# Para passar os hosts na execução:
# 	mpirun --hosts node01,node02,...,node0n -np <n> arquivo

# Para passar o arquivo de hosts na execução:
# 	mpirun --hostfile <arquivo de hosts> -np <n> arquivo

all:
	mpicc smooth_mpi.c -o smooth_mpi