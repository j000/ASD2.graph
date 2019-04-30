#ifndef EDGESITERATOR_HPP
#define EDGESITERATOR_HPP

#include "Graph.hpp"

template <typename V, typename E>
class Graph<V, E>::EdgesIterator {
	friend class Graph<V, E>;

public:
	EdgesIterator(const EdgesIterator&) = default;
	EdgesIterator(EdgesIterator&&) = default;
	EdgesIterator& operator=(const EdgesIterator&) = default;
	EdgesIterator& operator=(EdgesIterator&&) = default;

	bool operator==(const EdgesIterator& ei) const;
	bool operator!=(const EdgesIterator& ei) const;
	EdgesIterator& operator++();
	EdgesIterator operator++(int);
	const E& operator*() const;
	const E* operator->() const;

private:
	EdgesIterator(const Graph& graph, std::size_t nm_row, std::size_t nm_col);

	const Graph& m_graph;
	decltype(m_graph.m_data.begin()) m_row;
	decltype(m_row->second.begin()) m_column;
};

////////////////////////////////////////
// EdgesIterator implementation
////////////////////////////////////////

template <typename V, typename E>
bool Graph<V, E>::EdgesIterator::
operator==(const typename Graph<V, E>::EdgesIterator& ei) const
{
	return m_row == ei.m_row
		&& (m_row == m_graph.m_data.end() || m_column == ei.m_column);
}

template <typename V, typename E>
bool Graph<V, E>::EdgesIterator::operator!=(const EdgesIterator& ei) const
{
	return !(*this == ei);
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator& Graph<V, E>::EdgesIterator::operator++()
{
	// past-the-end
	if (m_row == m_graph.m_data.end()) {
		return *this;
	}

	do {
		++m_column;
		if (m_column == m_row->second.end()) {
			++m_row;
			if (m_row == m_graph.m_data.end()) {
				return *this;
			}
			m_column = m_row->second.begin();
		}
	} while (!m_column->has_value());
	return *this;
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::EdgesIterator::operator++(int)
{
	auto tmp = *this;
	this->operator++();
	return tmp;
}

template <typename V, typename E>
const E& Graph<V, E>::EdgesIterator::operator*() const
{
	return m_column->value();
}

template <typename V, typename E>
const E* Graph<V, E>::EdgesIterator::operator->() const
{
	return m_column->value();
}

template <typename V, typename E>
Graph<V, E>::EdgesIterator::EdgesIterator(
	const Graph& graph,
	std::size_t nm_row,
	std::size_t nm_col)
	: m_graph{graph}
	, m_row{m_graph.m_data.begin() + nm_row}
	, m_column{m_row->second.begin() + nm_col}
{
	if (m_row != m_graph.m_data.end() && !m_column->has_value())
		++(*this);
}

#endif /* EDGESITERATOR_HPP */
