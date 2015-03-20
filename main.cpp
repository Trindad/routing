/**
 * ------------------------------------------------------------------------
 * Dada uma topologia e sua matriz de trafego, o programa deve gerar como
 * saída a capacidade necessária para cada ligação.
 *
 * Compilar : g++ main.cpp -o main -std=c++11 -g -Wall
 * 
 * Alunos: Silvana Trindade e Maurício André Cinelli
 * Ano: 03/2015
 * ------------------------------------------------------------------------
 */
#include "roteamento.hpp"

int main(int argc, const char * argv[])
{
	if(argc <= 2)
	{
		help();
		return EXIT_FAILURE;
	}

	string arquivoTrafego = argv[2];

	Graph *graph;

	graph = (Graph*) malloc ( sizeof(Graph) );

	if (graph == NULL)
	{
		return EXIT_FAILURE;
	}

	readGraph(graph,string(argv[1]));//faz a leitura e inserção das arestas no grafo

	vector< vector<int> > trafficMatrix = readTrafficMatrix(graph->numberOfNodes, string(argv[2]));

	execute(graph,trafficMatrix);//roteamento

	/**
	 * Escreve o resultado no arquivo
	 */
	string temp = "_output_file.txt";
	string fileName = argv[1]+temp;

	write(graph,fileName);

	cout << "\t\x1b[32mSimulação completa! Confira o arquivo \"" << fileName << "\" para ver os resultados da simulação.\x1b[0m" << endl; 

	Edge *p;

	for (int i = 1; i < graph->numberOfNodes; i++)
	{
		p = graph->edges[i];

		while(p != NULL)
		{
			Edge *t = p;

			p = p->next;

			free(t);
		}
	}

	free(graph);

	return EXIT_SUCCESS;
}