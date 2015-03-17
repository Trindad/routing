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
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

#define LIMIT 1000

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
	Edge  *edges[LIMIT];
	int degree[LIMIT];
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

void help()
{

}

void InsertEdge(Graph *graph,int source, int target, bool directed)
{
	Edge *e;
	e = ( Edge* ) malloc ( sizeof(Edge) );

	if (e == NULL)
	{
		return;
	}

	e->weight = 1;
	e->capacity = std::numeric_limits<int>::max(); //inicia aresta com capacidade máxima
	e->target = target;
	e->next = graph->edges[source];

	graph->edges[source] = e; 
	graph->degree[source] += 1;
	
	if (directed == false)
	{
		InsertEdge(graph,target,source,true);
	}
	else
	{
		graph->numberOfEdges++;
	}
}

Graph *readGraph(Graph *graph, string _filename)
{

  ifstream file (_filename);
  string line;
  int nNodes = 0;

  if ( file.good() )
  {
    int lineCount = 0;

    while ( getline (file,line) )
    {

      if (lineCount == 0) 
      {

        stringstream s(line);
        s >> nNodes;
        cout<<"nNodes "<<nNodes<<endl;
        nNodes++;

        graph->numberOfNodes = nNodes;
        graph->numberOfEdges = 0;
        
        for (int i = 0; i < LIMIT; i++) 
        {
          graph->edges[i] = NULL;
          graph->degree[i] = 0;
        }

        lineCount++;

        continue;
      }

      stringstream ss(line); // Insert the string into a stream
      
      int source;
      int target;

      ss >> source >> target;

      cout << source << " " << target << endl;

      InsertEdge(graph,source, target, false);

      lineCount++;
    }

    file.close();
  } 
  else
  {
    printf("Could not open graph file.\n");
    exit(EXIT_FAILURE);
  }

  return graph;
}

vector< vector<int> > readTrafficMatrix(int nodes, string _filename)
{
  vector< vector<int> > matrix = vector< vector<int> >(nodes, vector<int>(nodes, -1));

  ifstream file (_filename);
  string line;

  if (file.good())
  {
    while ( getline (file,line) )
    {
      stringstream ss(line); // Insert the string into a stream
      unsigned int source;
      unsigned int target;
      int traffic;

      ss >> source >> target >> traffic;

      matrix[source][target] = traffic;
    }
    file.close();
  } else {
    printf("Could not open traffic file.\n");
    exit(EXIT_FAILURE);
  }

  return matrix;
}

/**
 * Implementação para encontrar um caminho mínimo de u até v
 */
void shortestPath( Graph *graph, int source, int target) 
{
	Edge *p; 					//vetor temporário
	vector<bool> inTree;		//O nó já esta na árvore?
	vector<int> distance;		//armazena distância para source
	int v;						//nó atual
	int w;						//candidato a próximo nó
	int weight;					//peso da aresta
	int dist;					//melhor distância atual para o nó de partida
	vector<int> path;

	inTree = vector<bool> ( graph->numberOfNodes, false);
	distance = vector<int> ( graph->numberOfNodes, std::numeric_limits<int>::max() );
	
	v = source;

	distance[v] = 0;
	inTree[v] = true;
	cout<<"source "<<source<<" target "<<target<<endl;

	while( inTree[target] == false )
	{	

		p = graph->edges[v];//p recebe os nós adjacentes de v

		if (p == NULL)
		{
			cout<<"Grafo com nó "<<v<<" desconexo."<<endl;
			return;
		}

		dist =  std::numeric_limits<int>::max();

		while( p != NULL )
		{
			w = p->target; 
			weight = p->weight;

			/**
			 * Verificação de caminho
			 */
			if ( distance[w] > ( distance[v] + weight ) && inTree[w] == false)
			{
				distance[w] = distance[v] + weight;
			}

			if (target == w)	
			{
				break;
			}

			p = p->next;
		}

		v = 1;
		dist = std::numeric_limits<int>::max();

		for (int i = 1; i < graph->numberOfNodes; i++) 
		{
			if ( (inTree[i] == false) && (dist > distance[i]) ) {
				dist = distance[i];
				v = i;
			}
		}

		inTree[v] = true;
	}

	cout<<"distance em target "<<distance[target]<<"\n\n"<<endl;
	for (unsigned int i = 0; i < path.size(); i++)
	{
		cout<<" "<<path[i];
	}
}

void execute(Graph *graph, vector< vector<int> > traffic) 
{
	for (int u = 1; u < graph->numberOfNodes-1; u++)
  	{
	  	for (int v = u+1; v < graph->numberOfNodes; v++)
	  	{

		  	shortestPath(graph,u,v);//busca caminho mínimo com maior capacidade disponível
	  		
	  	}
  	}


}

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

  readGraph(graph,string(argv[1]));

  vector< vector<int> > trafficMatrix = readTrafficMatrix(graph->numberOfNodes, string(argv[2]));

	
	Edge *p;  

  for (int i = 1; i < graph->numberOfNodes; i++) 
  {
    printf("Node %d: ", i);
   	
   	p = graph->edges[i];

   	while(p != NULL)
   	{
   		cout<<p->target<<"\t";
   		p = p->next;
   	}


    printf("\n---------------\n");
  }

  // for (int i = 1; i < graph->numberOfNodes; i++) {
  //   for (int j = 1; j < graph->numberOfNodes; j++) {
  //     printf("%d ", trafficMatrix[i][j]);
  //   }
  //   printf("\n");
  // }

  return EXIT_SUCCESS;
}