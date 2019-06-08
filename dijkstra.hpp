#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "Graph.hpp"

#include <cstdint>
#include <functional>

template <typename V, typename E>
std::tuple<double, std::vector<std::size_t>> dijkstra(
	const Graph<V, E>& g,
	const std::size_t start,
	const std::size_t end,
	const std::function<double(const E&)> f)
{
	return g.dijkstra(start, end, f);
}

#endif /* DIJKSTRA_HPP */
