#ifndef ASTAR_HPP
#define ASTAR_HPP

#include "Graph.hpp"

#include <cstdint>
#include <functional>

template <typename V, typename E>
std::tuple<double, std::vector<std::size_t>> astar(
	const Graph<V, E>& g,
	const std::size_t start,
	const std::size_t end,
	const std::function<double(const E&)> f,
	const std::function<
		double(const Graph<V, E>&, const std::size_t, const std::size_t)> h)
{
	return g.astar(start, end, f, h);
}

#endif /* ASTAR_HPP */
