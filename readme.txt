Per a compilar i executar els programes cal executar sobre el directori
on es troba aquest arxiu:

$ make

que generarà dos programes:
practica_EK
practica_PR

que corresponen a les diferents versions del problema que sol·luciona el 
programa seguit del nom del algorisme usat (EK -> Edmonds-Karp,
PR -> Push-Relabel).

Si volem mirar el temps que tarda en executar-se el programa en comptes 
de l'output normal, executarem el programa amb el flag -t:

$ ./practica_v1_PR -t < input.air

Si volem executar la versió 2 del eninciat, ho farem amb el flag -v2:

$ ./practica_v1_PR -t -v2 < input.air