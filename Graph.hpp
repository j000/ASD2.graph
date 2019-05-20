#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <queue>
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
	class BFSIterator;
	class DFSIterator;

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

	void bfs(std::size_t) const;
	void dfs(std::size_t) const;

	BFSIterator beginBFS(std::size_t = 0) const;
	BFSIterator endBFS() const;

	DFSIterator beginDFS(std::size_t = 0) const;
	DFSIterator endDFS() const;

private:
	std::vector<std::pair<V, std::vector<std::optional<E>>>> m_data{};
};

////////////////////////////////////////
// Iterators
////////////////////////////////////////

#include "EdgesIterator.hpp"
#include "VerticesIterator.hpp"

template <typename V, typename E>
class Graph<V, E>::BFSIterator {
	friend class Graph<V, E>;

public:
	~BFSIterator() = default;

	BFSIterator(const BFSIterator&) = default;
	BFSIterator(BFSIterator&&) = default;
	BFSIterator& operator=(const BFSIterator&) = default;
	BFSIterator& operator=(BFSIterator&&) = default;

	bool operator==(const BFSIterator& rhs) const
	{
		return m_current == rhs.m_current;
	}
	bool operator!=(const BFSIterator& rhs) const
	{
		return !(*this == rhs);
	}

	BFSIterator& operator++()
	{
		std::size_t tmp;
		do {
			if (m_queue.empty()) {
				m_current = m_graph.nrOfVertices();
				return *this;
			}
			tmp = m_queue.front();
			m_queue.pop();
		} while (m_visited[tmp]);
		m_visited[tmp] = true;
		for (std::size_t i = 0; i < m_graph.m_data[tmp].second.size(); ++i) {
			if (m_graph.edgeExist(tmp, i)) {
				m_queue.push(i);
			}
		}
		m_current = tmp;
		return *this;
	}
	BFSIterator operator++(int)
	{
		auto tmp = *this;
		this->operator++();
		return tmp;
	}

	const V& operator*() const
	{
		return m_graph.m_data[m_current].first;
	}
	const V* operator->() const
	{
		return m_graph.m_data[m_current].first;
	}

private:
	BFSIterator(const Graph& graph, std::size_t node)
		: m_graph{graph}, m_current{node}
	{
		m_visited.resize(graph.nrOfVertices());
		m_queue.push(node);
		++*this;
	}

	BFSIterator(const Graph& graph)
		: m_graph{graph}, m_current{graph.nrOfVertices()}
	{
	}

	const Graph& m_graph;
	std::size_t m_current{0};
	std::queue<std::size_t> m_queue{};
	std::vector<bool> m_visited{};
};

template <typename V, typename E>
class Graph<V, E>::DFSIterator {
	friend class Graph<V, E>;

public:
	~DFSIterator() = default;

	DFSIterator(const DFSIterator&) = default;
	DFSIterator(DFSIterator&&) = default;
	DFSIterator& operator=(const DFSIterator&) = default;
	DFSIterator& operator=(DFSIterator&&) = default;

	bool operator==(const DFSIterator& rhs) const
	{
		return m_current == rhs.m_current;
	}
	bool operator!=(const DFSIterator& rhs) const
	{
		return !(*this == rhs);
	}

	DFSIterator& operator++()
	{
		std::size_t tmp;
		do {
			if (m_stack.empty()) {
				m_current = m_graph.nrOfVertices();
				return *this;
			}
			tmp = m_stack.top();
			m_stack.pop();
		} while (m_visited[tmp]);
		m_visited[tmp] = true;
		for (std::size_t i = m_graph.m_data[tmp].second.size(); i > 0; --i) {
			if (m_graph.edgeExist(tmp, i - 1)) {
				m_stack.push(i - 1);
			}
		}
		m_current = tmp;
		return *this;
	}
	DFSIterator operator++(int)
	{
		auto tmp = *this;
		this->operator++();
		return tmp;
	}

	const V& operator*() const
	{
		return m_graph.m_data[m_current].first;
	}
	const V* operator->() const
	{
		return m_graph.m_data[m_current].first;
	}

private:
	DFSIterator(const Graph& graph, std::size_t node)
		: m_graph{graph}, m_current{node}
	{
		m_visited.resize(graph.nrOfVertices());
		m_stack.push(node);
		++*this;
	}

	DFSIterator(const Graph& graph)
		: m_graph{graph}, m_current{graph.nrOfVertices()}
	{
	}

	const Graph& m_graph;
	std::size_t m_current{0};
	std::stack<std::size_t> m_stack{};
	std::vector<bool> m_visited{};
};

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
		return std::make_pair(EdgesIterator{*this, 0, 0}, false);
	m_data[vertex1_id].second[vertex2_id] = label;
	return std::make_pair(EdgesIterator{*this, vertex1_id, vertex2_id}, true);
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

template <typename V, typename E>
void Graph<V, E>::bfs(std::size_t start) const
{
	std::vector<bool> visited(m_data.size());
	std::queue<std::size_t> queue{};
	queue.push(start);
	while (!queue.empty()) {
		auto tmp = queue.front();
		queue.pop();
		if (visited[tmp])
			continue;
		visited[tmp] = true;
		for (std::size_t i = 0; i < m_data[tmp].second.size(); ++i) {
			if (edgeExist(tmp, i)) {
				queue.push(i);
			}
		}
		std::cout << m_data[tmp].first << ", ";
	}
	std::cout << std::endl;
}

template <typename V, typename E>
void Graph<V, E>::dfs(std::size_t start) const
{
	std::vector<bool> visited(m_data.size());
	std::stack<std::size_t> stack{};
	stack.push(start);
	while (!stack.empty()) {
		auto tmp = stack.top();
		stack.pop();
		if (visited[tmp])
			continue;
		visited[tmp] = true;
		for (std::size_t i = m_data[tmp].second.size(); i > 0; --i) {
			if (edgeExist(tmp, i - 1)) {
				stack.push(i - 1);
			}
		}
		std::cout << m_data[tmp].first << ", ";
	}
	std::cout << std::endl;
}

template <typename V, typename E>
typename Graph<V, E>::BFSIterator Graph<V, E>::beginBFS(std::size_t node) const
{
	return BFSIterator{*this, node};
}

template <typename V, typename E>
typename Graph<V, E>::BFSIterator Graph<V, E>::endBFS() const
{
	return BFSIterator{*this};
}

template <typename V, typename E>
typename Graph<V, E>::DFSIterator Graph<V, E>::beginDFS(std::size_t node) const
{
	return DFSIterator{*this, node};
}

template <typename V, typename E>
typename Graph<V, E>::DFSIterator Graph<V, E>::endDFS() const
{
	return DFSIterator{*this};
}

#endif /* GRAPH_HPP */
