### MAKEFILE ###

all: compile run

run:
	@ ./smooth < ./in/in.ppm
	
compile: smooth.c
	@gcc smooth.c -o smooth
