all: practica_EK practica_PR 

practica_EK: practica_EK.cpp
	g++ practica_EK.cpp -o practica_EK -Wall -std=c++0x -O2

practica_PR: practica_PR.cpp
	g++ practica_PR.cpp -o practica_PR -Wall -std=c++0x -O2

clean: 
	rm -f practica_EK practica_PR