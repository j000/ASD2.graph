#ifndef VERTICESITERATOR_HPP
#define VERTICESITERATOR_HPP

#include "Graph.hpp"

template <typename V, typename E>
class Graph<V, E>::VerticesIterator {
	friend class Graph<V, E>;

public:
	VerticesIterator(const VerticesIterator&) = default;
	VerticesIterator(VerticesIterator&&) = default;
	VerticesIterator& operator=(const VerticesIterator&) = default;
	VerticesIterator& operator=(VerticesIterator&&) = default;

	bool operator==(const VerticesIterator& vi) const;
	bool operator!=(const VerticesIterator& vi) const;

	VerticesIterator& operator++();
	VerticesIterator operator++(int);

	const V& operator*() const;
	const V* operator->() const;

private:
	VerticesIterator(const Graph& graph, std::size_t current_vertex_id = 0u);

	const Graph& m_graph;
	typename decltype(std::declval<Graph>().m_data)::const_iterator m_iterator;
};

////////////////////////////////////////
// VerticesIterator implementation
////////////////////////////////////////

template <typename V, typename E>
bool Graph<V, E>::VerticesIterator::
operator==(const typename Graph<V, E>::VerticesIterator& vi) const
{
	return m_iterator == vi.m_iterator;
}

template <typename V, typename E>
bool Graph<V, E>::VerticesIterator::
operator!=(const typename Graph<V, E>::VerticesIterator& vi) const
{
	return !(*this == vi);
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator& Graph<V, E>::VerticesIterator::
operator++()
{
	++m_iterator;
	return *this;
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::VerticesIterator::
operator++(int)
{
	auto tmp = *this;
	this->operator++();
	return tmp;
}

template <typename V, typename E>
const V& Graph<V, E>::VerticesIterator::operator*() const
{
	return m_iterator->first;
}

template <typename V, typename E>
const V* Graph<V, E>::VerticesIterator::operator->() const
{
	return m_iterator->first;
}

template <typename V, typename E>
Graph<V, E>::VerticesIterator::VerticesIterator(
	const Graph& graph,
	std::size_t current_vertex_id)
	: m_graph{graph}, m_iterator{m_graph.m_data.begin() + current_vertex_id}
{
}

#endif /* VERTICESITERATOR_HPP */
