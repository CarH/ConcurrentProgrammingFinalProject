### MAKEFILE ###

all: compile run

run:
	@ ./smooth < ./in/1.ppm
	
compile: smooth.c
	@gcc smooth.c -o smooth
