#include "Graph.hpp"

#include <cmath>
#include <functional>
#include <iostream>

using namespace std;

template <typename V, typename E>
void test(const Graph<V, E>& g, std::size_t from, std::size_t target)
{
	auto h = [](decltype(g), std::size_t current, std::size_t end) -> double {
		// return 0.0;
		const double dx = static_cast<double>(current / 4) - static_cast<double>(end / 4);
		const double dy = static_cast<double>(current % 4) - static_cast<double>(end % 4);
		return 0.2 * (std::abs(dx) + std::abs(dy));
		return std::sqrt(dx * dx + dy * dy);
	};
	auto f = [](const double& e) -> double {
		return e;
	};

	auto [shortest_path_distance, shortest_path] = g.a_star(
		from, target, f, h);
	std::cout << "Distance from " << from << " to " << target << ": "
			  << shortest_path_distance << std::endl;
	std::cout << "Path from " << from << " to " << target << ":" << std::endl;
	std::cout << *g.vertex(from) << " -> " << *g.vertex(target) << std::endl;
	for (auto& v_id : shortest_path) {
		std::cout << *g.vertex(v_id) << ", ";
	}
	std::cout << std::endl;

	std::tie(shortest_path_distance, shortest_path)
		= g.dijkstra(from, target, [](const double& e) -> double { return e; });
	std::cout << "Distance from " << from << " to " << target << ": "
			  << shortest_path_distance << std::endl;
	std::cout << "Path from " << from << " to " << target << ":" << std::endl;
	std::cout << *g.vertex(from) << " -> " << *g.vertex(target) << std::endl;
	for (auto& v_id : shortest_path) {
		std::cout << *g.vertex(v_id) << ", ";
	}
	std::cout << std::endl << std::endl;
}

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */

int main()
{
	constexpr std::size_t vertices = 4 * 4;
	Graph<std::string, double> g;

	for (std::size_t i = 0u; i < vertices; ++i) {
		g.insertVertex(
			"(" + std::to_string(i / 4) + ", " + std::to_string(i % 4) + ")");
	}

	for (std::size_t i = 0; i < vertices; ++i) {
		if (i < 12) {
			g.insertEdge(i, i + 4, 1.0);
			g.insertEdge(i + 4, i, 1.0);
			if (i % 4 != 3) {
				g.insertEdge(i, i + 5, 1.4);
				g.insertEdge(i + 5, i, 1.4);
			}
			if (i % 4 != 0) {
				g.insertEdge(i, i + 3, 1.4);
				g.insertEdge(i + 3, i, 1.4);
			}
		}
		if (i % 4 != 3) {
			g.insertEdge(i, i + 1, 1.0);
			g.insertEdge(i + 1, i, 1.0);
		}
	}

	std::cout << "Nr of vertices: " << g.nrOfVertices() << std::endl;
	std::cout << "Nr of edges: " << g.nrOfEdges() << std::endl;
	std::cout << std::endl;
	g.printNeighborhoodMatrix();
	std::cout << std::endl;

	// test(g, 2, 4);
	test(g, 0, 10);
	// test(g, 3, 1);
	test(g, 0, 15);
	return 0;
}

