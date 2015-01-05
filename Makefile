all: practica_v1_EK practica_v1_PR practica_v2_EK

practica_v1_EK: practica_v1_EK.cpp
	g++ practica_v1_EK.cpp -o practica_v1_EK -Wall -std=c++0x -O2

practica_v1_PR: practica_v1_PR.cpp
	g++ practica_v1_PR.cpp -o practica_v1_PR -Wall -std=c++0x -O2

practica_v2_EK: practica_v2_EK.cpp
	g++ practica_v2_EK.cpp -o practica_v2_EK -Wall -std=c++0x -O2

clean: 
	rm -f practica_v1_EK practica_v1_PR practica_v2_EK