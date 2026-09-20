# Graphs Module

This folder contains a C implementation of a graph based on an adjacency linked list.

## Data Model

The public header [graphs.h](graphs.h) defines three core structures:

- `graph_t`: owns the vertex list and tracks the total number of vertices.
- `vertex_t`: stores the vertex index, a string payload, its outgoing edges, and the next vertex in the list.
- `edge_t`: links one vertex to another.

The module also defines `edge_type_t` for one-way and two-way connections, and a small queue type used by breadth-first traversal.

## Available Functions

- [graph_create](0-graph_create.c): allocate and initialize an empty graph.
- [graph_add_vertex](1-graph_add_vertex.c): append a new vertex with copied string content.
- [graph_add_edge](2-graph_add_edge.c): connect two existing vertices with a directed or bidirectional edge.
- [graph_delete](3-graph_delete.c): free the entire graph.
- [depth_first_traverse](4-depth_first_traverse.c): walk the graph depth-first, starting from the first vertex in the list.
- [breadth_first_traverse](5-breadth_first_traverse.c): walk the graph breadth-first, also starting from the first vertex in the list.
- [graph_display](graph_display.c): print the graph in a human-readable form.

## Notes

- Vertex content is copied when a vertex is added.
- Vertex indices follow insertion order.
- Traversals use a callback of the form `void (*action)(const vertex_t *v, size_t depth)`.
- Both traversal functions return the deepest depth reached, or `0` on failure.

## Example Use

Create a graph, add vertices, connect them, traverse it, and finally delete it when finished. The traversal callback can be used to print each visited vertex, collect values, or run custom logic.
