/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"

using namespace std;

template<class T> class Edge;
template<class T> class Graph;
template<class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template<class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // outgoing edges
	bool visited;          // auxiliary field
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;
	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	friend class Graph<T> ;
	friend class MutablePriorityQueue<Vertex<T>> ;
};

template<class T>
Vertex<T>::Vertex(T in) :
		info(in) {
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template<class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template<class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template<class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template<class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

/********************** Edge  ****************************/

template<class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T> ;
	friend class Vertex<T> ;
};

template<class T>
Edge<T>::Edge(Vertex<T> *d, double w) :
		dest(d), weight(w) {
}

/*************************** Graph  **************************/

template<class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	// Fp05 - single source
	void dijkstraShortestPath(const T &s);
	void dijkstraShortestPathOld(const T &s);
	void unweightedShortestPath(const T &s);
	void bellmanFordShortestPath(const T &s);
	vector<T> getPath(const T &origin, const T &dest) const;

	// Fp05 - all pairs
	void floydWarshallShortestPath();
	vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;

};

template<class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template<class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template<class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template<class T>
bool Graph<T>::addVertex(const T &in) {
	if (findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template<class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2, w);
	return true;
}

/**************** Single Source Shortest Path algorithms ************/

/* alinea a) */

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {

	/* Distancia de todos os pontos a origem é infinito e o caminho é nulo */
	for (int i = 0; i < (int) vertexSet.size(); i++) {
		vertexSet.at(i)->dist = INF;
		vertexSet.at(i)->path = NULL;
	}

	/* A distancia na origem é 0*/
	auto it = findVertex(origin);
	it->dist = 0;

	/* Fila de Prioridade mutavel vazia*/
	MutablePriorityQueue<Vertex<T>> q;

	/* Inserir a origem*/
	q.insert(it);

	while (!q.empty())
	{
		/* E ganacioso porque divide o caminho em caminhos mais pequenos e pressupoe que a melhor
		 * solucao destes caminhos e a melhor solucao geral*/
		auto v = q.extractMin();

		/* Adj é um vetor com os vertices que se ligam a v*/
		for(auto w : v->adj) {

			Vertex<T> * tmp = w.dest;

			if(tmp->dist > v->dist + w.weight)
			{
				/* Se a distancia é igual a INF é porque nunca foi inserido na fila
				 * (só pode haver uma instancia de cada vertice na fila) */
				bool is_not_in_queue = (tmp->dist == INF);

				tmp->dist = v->dist + w.weight;
				tmp->path = v;

				if(is_not_in_queue)
					q.insert(tmp);
				else
					q.decreaseKey(tmp);
			}

		}
	}

}

/* Alinea b) */
template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest) const {
	vector<T> res;

	/*vai do destino até à origem (a função acima
	 * foi previamente chamada e foi obtido o melhor
	 * caminho para cada vertice para a origem*/
	for (auto v = findVertex(dest); v != NULL; v = v->path) {
		res.push_back(v->info);

		/* Se chegarmos a origem, acabou*/
		if (v->info == origin)
			break;
	}

	/* o caminho esta ao contrario */
	reverse(res.begin(), res.end());

	return res;
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
	// TODO
}

template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
	// TODO
}

/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
	// TODO
}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const {
	vector<T> res;
	// TODO
	return res;
}

#endif /* GRAPH_H_ */
