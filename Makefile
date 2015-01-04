all: practica_v1_EK practica_v1_PR

practica_v1_EK: practica_v1_EK.cpp
	g++ practica_v1_EK.cpp -o practica_v1_EK -Wall -std=c++0x -O3

practica_v1_PR: practica_v1_PR.cpp
	g++ practica_v1_PR.cpp -o practica_v1_PR -Wall -std=c++0x -O3

clean: 
	rm practica_v1_EK practica_v1_PR