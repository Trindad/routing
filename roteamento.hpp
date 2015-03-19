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
#include <iostream>
#include <deque>
#include <iterator>
#include <algorithm> 
#include <limits> //limites numéricos
#include <stdexcept> // for std::runtime_error
#include <vector>
#include <random>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>

#define LIMIT 1000

using namespace std;

typedef struct _e
{
	int target;					//destino
	int weight; 				//peso atualizado da aresta
	int capacity; 				//capacidade da aresta
	struct _e *next;			//ponteiro para o próximo nó adjacente
	unsigned int dirty; 		//bit correspodente a aresta ativa ou não
}Edge;

typedef struct
{
	Edge  *edges[LIMIT];	//matriz de arestas
	int degree[LIMIT];		//grau do nó
	int numberOfEdges;		//número de arestas
	int numberOfNodes;		//número de nós

}Graph;

void help();

/**
 * Insere arestas entre nós adjacentes
 */
void InsertEdge(Graph *graph,int source, int target, bool directed);

/**
 * Faz a leitura do grafo a partir do arquivo 
 */
Graph *readGraph(Graph *graph, string _filename);

vector< vector<int> > readTrafficMatrix(int nodes, string _filename);

/**
 * Insere caminho mínimo ao inverso
 */
void insertShortestPath(vector<int> parent, vector<int> &path, int target);

/**
 * Implementação para encontrar um caminho mínimo de u até v
 */
vector<int> shortestPath( Graph *graph, int source, int target);

/**
 * Atualiza peso das arestas adjacentes
 * recebendo o valor da demanda de u <-> v
 */
void updateWeight(Graph *graph, vector< vector<int> > trafficMatrix);

/**
 * Atualiza o peso e a capacidade das arestas pertencentes 
 * ao caminho mínimo passado como parâmetro ( path )
 */
void updateCapacityAndWeightByPath(Graph *graph,vector<vector<int>> traffic,vector<int> path);

/**
 * Verifica nós adjacentes
 */
bool isAdjacent(Graph *graph,int source,int target);

/**
 * Realiza o roteamento da rede buscando obter o menor custo
 */
void execute(Graph *graph, vector< vector<int> > traffic);

/**
 * Escreve resultado em um arquivo de saida
 */
void write(Graph *graph, string fileName);