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

int sumDemand; 

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

	e->weight = 0;//peso inicial das arestas é zero
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
	sumDemand = 0;

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

		  sumDemand = sumDemand + traffic;
		}

		file.close();
	} 
	else
	{
		printf("Could not open traffic file.\n");
		exit(EXIT_FAILURE);
	}

	return matrix;
}

/**
 * Implementação para encontrar um caminho mínimo de u até v
 */
vector<int> shortestPath( Graph *graph, int source, int target) 
{
	Edge *p; 					//vetor temporário
	vector<bool> inTree;		//O nó já esta na árvore?
	vector<int> capacities;		//armazena distância para source
	int v;						//nó atual
	int w;						//candidato a próximo nó
	int capacity;				//peso da aresta
	int cap;					//melhor distância atual para o nó de partida
	vector<int> parent = vector<int> (graph->numberOfNodes, -1);

	inTree = vector<bool> ( graph->numberOfNodes, false);
	capacities = vector<int> ( graph->numberOfNodes, std::numeric_limits<int>::max() );
	
	v = source;

	capacities[v] = 0;
	inTree[v] = true;
	cout<<"source "<<source<<" target "<<target<<endl;

	while( inTree[target] == false )
	{	
		cout<<"novo source "<<v<<endl;
		p = graph->edges[v];//p recebe os nós adjacentes de v

		if (p == NULL)
		{
			cout<<"Grafo com nó "<<v<<" desconexo."<<endl;
			exit(1);
		}

		while( p != NULL )
		{
			w = p->target; 
			capacity = p->weight;

			/**
			 * Verificação de caminho
			 */
			if ( capacities[w] > ( capacities[v] + capacity ) && inTree[w] == false)
			{
				capacities[w] = capacities[v] + capacity;
				parent[v] = v;
			}

			if (target == w)	
			{
				break;
			}

			p = p->next;
		}

		v = 1;
		cap = std::numeric_limits<int>::max();

		for (int i = 1; i < graph->numberOfNodes; i++) 
		{
			if ( ( inTree[i] == false ) && (cap > capacities[i]) ) 
			{
				cap = capacities[i];
				v = i;
			}
		}

		inTree[v] = true;
	}

	vector<int> path;
	
	cout << "\n";

	/**
	 * insere caminho mínimo encontrado pelo algoritmo de Dijkstra
	 */
	for (unsigned int i = 0; i < parent.size(); i++)
	{
		if (parent[i] > -1) {
			path.push_back(i);
			cout << i << " ";
		}
	}

	path.push_back(target);

	return path;
}

void updateWeight(Graph *graph, vector< vector<int> > trafficMatrix)
{
	/**
	 * Preenche com a demanda para nós adjacentes e
	 * em seguida atualizada a capacidade das arestas
	 */
	for (int i = 1; i < graph->numberOfNodes; i++)
	{
		Edge *p;

		p = graph->edges[i];

		while(p != NULL)
		{
			int target = p->target;

			if ( trafficMatrix[i][target] >= 0 )
			{
				cout<<"source"<<i<<"target"<<target<<" ddemanda "<<trafficMatrix[i][target]<<endl;
				p->weight = trafficMatrix[i][target];
				p->capacity = ( p->capacity - p->weight );//diminiu a capacidade da ligação
			}

			p = p->next;
		}

		cout<<"\n\n";

		if (graph->edges[i] == NULL)
		{
			exit(EXIT_FAILURE);
		}
	}

}

void updateCapacityAndWeightByPath(Graph *graph,vector<vector<int>> traffic,vector<int> path)
{
	int prev = -1;

	Edge *p;

	int weight = traffic[path[0]][path[path.size() - 1]];

	for (unsigned int j = 0; j < path.size(); j++) 
	{
		cout << "path[j] " << path[j] << "weight " << weight << endl;
		if (prev > 0) 
		{
			p = graph->edges[prev];

			while (p->target != path[j]) 
			{
				p = p->next;
			}

			p->capacity -= weight;

			p = graph->edges[path[j]];

			while (p->target != prev) 
			{
				p = p->next;
			}

			if (traffic[path[j]][prev] > -1)  
			{
				p->capacity -= weight;
			}
		}

		prev = path[j];
	}
}

void execute(Graph *graph, vector< vector<int> > traffic) 
{

	/**
	 * Inicializa capacidade das ligações
	 */
	Edge *p;

	int maxCapacity = sumDemand*0.7;//capacidade máxima considerada é 70% da soma total das demandas

	for (int i = 1; i < graph->numberOfNodes; i++)
	{
		p = graph->edges[i];

		while(p != NULL)
		{
			p->capacity = maxCapacity;

			p = p->next;
		}
	}

	updateWeight(graph,traffic);//atualiza peso das arestas adjacentes no estado inicial

	for (int u = 1; u < graph->numberOfNodes-1; u++)
  	{
	  	for (int v = u+1; v < graph->numberOfNodes; v++)
	  	{

		  	vector<int> path = shortestPath(graph,u,v);//busca caminho mínimo com maior capacidade disponível

	  		updateCapacityAndWeightByPath(graph,traffic,path);
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

  for (int i = 1; i < graph->numberOfNodes; i++) 
  {
    for (int j = 1; j < graph->numberOfNodes; j++) 
    {
      printf("%d ", trafficMatrix[i][j]);
    }
    printf("\n");
  }

  execute(graph,trafficMatrix);//roteamento

  return EXIT_SUCCESS;
}