/**
 * ------------------------------------------------------------------------
 *
 *
 * 
 * Alunos: Silvana Trindade e Maurício André Cinelli
 * Ano: 2015
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

using namespace std;

typedef struct _e
{
	int target;
	int weight;
	int capacity;
	struct _e *next;
}Edge;

typedef struct
{
	Edge  **edges;
	vector<int> degree;
	int numberOfEdges;
	int numberOfNodes;

}Graph;

typedef struct _p
{
	vector<Edge> nodes;
	int cost;
}Path;

typedef struct _t
{
	vector<Path> shortestPaths;

}Traffic;

static vector< int > parent; 

/**
 * Implementação para encontrar um caminho mínimo de u até v
 */
void shortestPath( Graph *graph, int start) 
{
	Edge *p; 			//vetor temporário
	vector<bool> inTree;		//O nó já esta na árvore?
	vector<int> distance;		//armazena distância para start
	int v;						//nó atual
	int w;						//candidato a próximo nó
	double weight;				//peso da aresta
	double dist;				//melhor distância atual para o nó de partida


	inTree = vector<bool> ( graph->numberOfNodes, false);
	distance = vector<int> ( graph->numberOfNodes, std::numeric_limits<int>::max());
	parent = vector<int> ( graph->numberOfNodes, -1);

	distance[start] = 0;
	v = start;

	while( inTree[v] == false)
	{
		inTree[v] = true;
		p = graph->edges[v];

		while( p != NULL )
		{
			w = p->target; 
			weight = p->weight;

			/**
			 * Verificação de caminho
			 */
			if ( distance[w] > ( distance[w] + weight ) )
			{
				distance[w] = distance[w] + weight;
				parent[w] = v;
			}

			p = p->next;
		}

		v = 0;

		dist = std::numeric_limits<int>::max();

		for (int i = 0; i < graph->numberOfNodes; i++)
		{
			if ( ( inTree[i] == false ) && ( dist > distance[i] ) )
			{
				dist = distance[i];
				v = i;		
			}
		}
	}
}

void minimumCostFlow(Graph graph, vector< vector<int> > traffic) 
{

}

int main(void)
{
	
	return 0;
}