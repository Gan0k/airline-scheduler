practica: Preflow_push.o practica_v1.o Edmonds_Karp.o
	g++ -o practica *.o -Wall -std=c++0x 

Preflow_push.o: Preflow_push.cpp Preflow_push.hpp
	g++ -c Preflow_push.cpp -Wall -std=c++0x 

practica_v1.o: practica_v1.cpp
	g++ -c practica_v1.cpp -Wall -std=c++0x 

Edmonds_Karp.o: Edmonds_Karp.hpp Edmonds_Karp.cpp
	g++ -c Edmonds_Karp.cpp -Wall -std=c++0x 

clean:
	rm *.o practica