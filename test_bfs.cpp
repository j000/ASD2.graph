#include "Graph.hpp"

#include <iostream>

int main(int, char**) {
	// Graph<std::string, int> g;
	// g.insertVertex("V1");
	// g.insertVertex("V2");
	// g.insertVertex("V3");
	// g.insertVertex("V4");
	// g.insertEdge(0, 0, 1);
	// g.insertEdge(0, 1, 2);
	// g.insertEdge(1, 2, 3);
	// g.insertEdge(2, 2, 4);
	// g.insertEdge(3, 2, 5);
	// g.insertEdge(3, 0, 6);
	// g.insertEdge(0, 3, 7);
	// g.insertEdge(1, 3, 8);
	// g.printNeighborhoodMatrix();
	Graph<std::string, double> g;

	for (std::size_t i = 0u; i < 6u; ++i) {
		g.insertVertex("data " + std::to_string(i));
	}

	for (std::size_t i = 0u; i < g.nrOfVertices(); ++i) {
		for (std::size_t j = 0u; j < g.nrOfVertices(); ++j) {
			if ((i + j) & 1u || i & 1u) {
				g.insertEdge(i, j, ((i != j) ? (i + j) / 2. : 1.));
			}
		}
	}
	g.insertEdge(0, 2, 4.);
	std::cout << (g.removeVertex(1) ? "Udalo sie" : "Nie udalo sie")
			  << std::endl;
	std::cout << (g.removeEdge(2, 2) ? "Udalo sie" : "Nie udalo sie")
			  << std::endl;
	std::cout << (g.removeEdge(2, 3) ? "Udalo sie" : "Nie udalo sie")
			  << std::endl;
	std::cout << (g.removeEdge(4, 3) ? "Udalo sie" : "Nie udalo sie")
			  << std::endl;
	std::cout << "Nr of vertices: " << g.nrOfVertices() << std::endl;
	std::cout << "Nr of edges: " << g.nrOfEdges() << std::endl;
	std::cout << std::endl;
	g.printNeighborhoodMatrix();
	std::cout << std::endl;
	std::cout << "Vertices data:" << std::endl;
	for (auto v_it = g.beginVertices(); v_it != g.endVertices(); ++v_it) {
		std::cout << *v_it << ", ";
	}
	std::cout << std::endl << std::endl;
	std::cout << "Edges data:" << std::endl;
	for (auto e_it = g.beginEdges(); e_it != g.endEdges(); ++e_it) {
		std::cout << *e_it << ", ";
	}
	std::cout << std::endl;

	std::cout << std::endl;
	for (int i = 0; i < g.nrOfVertices(); ++i) {
		std::cout << "dfs starting from " << i << std::endl;
		g.dfs(i);
	}

	std::cout << std::endl;
	for (int i = 0; i < g.nrOfVertices(); ++i) {
		std::cout << "bfs starting from " << i << std::endl;
		g.bfs(i);
		for (auto j = g.beginBFS(i); j != g.endBFS(); ++j) {
			std::cout << *j << ", ";
		}
		std::cout << std::endl;
	}

}
