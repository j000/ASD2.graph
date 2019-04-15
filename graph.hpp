#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <stack>
#include <vector>

template <typename V, typename E>
class Graph {
public:
	class VerticesIterator {
	private:
		VerticesIterator(
			Graph<V, E>& graph,
			std::size_t current_vertex_id = 0u);

	public:
		// ...

		friend class Graph<V, E>;

	private:
		// ...

	public:
		bool operator==(const VerticesIterator& vi) const;
		bool operator!=(const VerticesIterator& vi) const
		{
			return !(*this == vi);
		}
		VerticesIterator& operator++();
		VerticesIterator operator++(int);
		V& operator*() const;
		V* operator->() const;
	};

	class EdgesIterator {
	private:
		EdgesIterator(
			Graph<V, E>& graph,
			std::size_t nm_row,
			std::size_t nm_col);

	public:
		// ...

		friend class Graph<V, E>;

	private:
		// ...

	public:
		bool operator==(const EdgesIterator& ei) const;
		bool operator!=(const EdgesIterator& ei) const
		{
			return !(*this == ei);
		}
		EdgesIterator& operator++();
		EdgesIterator operator++(int);
		E& operator*() const;
		E* operator->() const;
	};

public:
	Graph();
	// ...

private:
	// ...

public:
	VerticesIterator insertVertex(const V& vertex_data);
	std::pair<EdgesIterator, bool> insertEdge(
		std::size_t vertex1_id,
		std::size_t vertex2_id,
		const E& label = E(),
		bool replace = true);
	bool removeVertex(std::size_t vertex_id);
	bool removeEdge(std::size_t vertex1_id, std::size_t vertex2_id);
	bool edgeExist(std::size_t vertex1_id, std::size_t vertex2_id) const;
	std::size_t nrOfVertices() const;
	std::size_t nrOfEdges() const;
	void printNeighborhoodMatrix() const;
	VerticesIterator vertex(std::size_t vertex_id);
	EdgesIterator edge(std::size_t vertex1_id, std::size_t vertex2_id);
	VerticesIterator begin()
	{
		return beginVertices();
	}
	VerticesIterator end()
	{
		return endVertices();
	}
	VerticesIterator beginVertices();
	VerticesIterator endVertices();
	EdgesIterator beginEdges();
	EdgesIterator endEdges();
};

