#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <stack>
#include <vector>

#define USE_FASTER_REMOVAL 0

////////////////////////////////////////
// Graph
////////////////////////////////////////

template <typename V, typename E>
class Graph {
public:
	class VerticesIterator;
	class EdgesIterator;

public:
	Graph() = default;
	Graph(const Graph&) = default;
	Graph(Graph&&) = default;
	Graph& operator=(const Graph&) = default;
	Graph& operator=(Graph&&) = default;

	std::size_t nrOfVertices() const;
	VerticesIterator begin() const;
	VerticesIterator end() const;
	VerticesIterator beginVertices() const;
	VerticesIterator endVertices() const;
	VerticesIterator vertex(std::size_t vertex_id) const;
	VerticesIterator insertVertex(const V& vertex_data);
	bool removeVertex(std::size_t vertex_id);

	std::size_t nrOfEdges() const;
	EdgesIterator beginEdges() const;
	EdgesIterator endEdges() const;
	EdgesIterator edge(std::size_t vertex1_id, std::size_t vertex2_id) const;
	bool edgeExist(std::size_t vertex1_id, std::size_t vertex2_id) const;
	std::pair<EdgesIterator, bool> insertEdge(
		std::size_t vertex1_id,
		std::size_t vertex2_id,
		const E& label = E(),
		bool replace = true);
	bool removeEdge(std::size_t vertex1_id, std::size_t vertex2_id);

	void printNeighborhoodMatrix() const;

private:
	std::vector<std::pair<V, std::vector<std::optional<E>>>> m_data{};
};

////////////////////////////////////////
// Iterators
////////////////////////////////////////

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

////////////////////////////////////////
// Graph implementation
////////////////////////////////////////

template <typename V, typename E>
std::size_t Graph<V, E>::nrOfVertices() const
{
	return m_data.size();
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::begin() const
{
	return beginVertices();
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::end() const
{
	return endVertices();
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::beginVertices() const
{
	return VerticesIterator(*this);
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::endVertices() const
{
	return ++VerticesIterator(*this, m_data.size() - 1);
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator
Graph<V, E>::vertex(std::size_t vertex_id) const
{
	return VerticesIterator(*this, vertex_id);
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator
Graph<V, E>::insertVertex(const V& vertex_data)
{
	for (auto& vertex : m_data) {
		vertex.second.push_back({});
	}
	m_data.push_back(std::make_pair(
		vertex_data, std::vector<std::optional<E>>(m_data.size() + 1)));

	return VerticesIterator(*this, m_data.size() - 1);
}

template <typename V, typename E>
bool Graph<V, E>::removeVertex(std::size_t vertex_id)
{
	if (vertex_id < 0 || vertex_id >= m_data.size())
		// throw std::out_of_range{"Index out of range"};
		return false;

	using std::swap;
#if USE_FASTER_REMOVAL
	swap(m_data[vertex_id], m_data.back());
	m_data.pop_back();
	for (auto& list : m_data) {
		swap(list.second[vertex_id], list.second.back());
		list.second.pop_back();
	}
#else
	m_data.erase(m_data.begin() + vertex_id);
	for (auto& list : m_data) {
		list.second.erase(list.second.begin() + vertex_id);
	}
#endif
	return true;
}

template <typename V, typename E>
std::size_t Graph<V, E>::nrOfEdges() const
{
	std::size_t out{0};
	for (const auto& i : m_data)
		for (const auto& j : i.second)
			if (j.has_value())
				++out;
	return out;
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::beginEdges() const
{
	return EdgesIterator(*this, 0, 0);
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::endEdges() const
{
	auto tmp = EdgesIterator(*this, 0, 0);
	tmp.m_row = m_data.end();
	return tmp;
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator
Graph<V, E>::edge(std::size_t vertex1_id, std::size_t vertex2_id) const
{
	return EdgesIterator{*this, vertex1_id, vertex2_id};
}

template <typename V, typename E>
bool Graph<V, E>::edgeExist(std::size_t vertex1_id, std::size_t vertex2_id)
	const
{
	return m_data[vertex1_id].second[vertex2_id].has_value();
}

template <typename V, typename E>
std::pair<typename Graph<V, E>::EdgesIterator, bool> Graph<V, E>::insertEdge(
	std::size_t vertex1_id,
	std::size_t vertex2_id,
	const E& label,
	bool replace)
{
	if (edgeExist(vertex1_id, vertex2_id) && !replace)
		return std::make_pair(EdgesIterator(*this, 0, 0), false);
	m_data[vertex1_id].second[vertex2_id] = label;
	return std::make_pair(EdgesIterator(*this, vertex1_id, vertex2_id), true);
}

template <typename V, typename E>
bool Graph<V, E>::removeEdge(std::size_t vertex1_id, std::size_t vertex2_id)
{
	if (vertex1_id < 0 || vertex2_id >= m_data.size() || vertex2_id < 0
		|| vertex2_id >= m_data.size())
		// throw std::out_of_range{"Index out of range"};
		return false;

	auto& tmp = m_data[vertex1_id].second[vertex2_id];
	if (!tmp.has_value())
		return false;
	tmp.reset();
	return true;
}

template <typename V, typename E>
void Graph<V, E>::printNeighborhoodMatrix() const
{
	constexpr int width{8};
	std::cout << std::setw(width) << " ";
	for (const auto& i : m_data)
		std::cout << std::setw(width) << i.first;
	std::cout << std::endl;

	for (const auto& i : m_data) {
		std::cout << std::setw(width) << i.first;
		for (const auto& j : i.second) {
			if (j)
				std::cout << std::setw(width) << *j;
			else
				std::cout << std::setw(width) << "-";
		}
		std::cout << std::endl;
	}
}

#endif /* GRAPH_HPP */
