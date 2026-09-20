# ALU System Algorithms

This repository contains data-structure and algorithm exercises implemented in C.

## Graphs

The current module lives in [graphs/](graphs/) and implements a graph represented as an adjacency linked list. The module provides:

- Graph creation and deletion
- Vertex insertion
- Edge insertion for both one-way and two-way connections
- Depth-first traversal
- Breadth-first traversal
- A small display helper for debugging and examples

## Layout

- [graphs/graphs.h](graphs/graphs.h) exposes the public API and core structures.
- [graphs/0-graph_create.c](graphs/0-graph_create.c) creates an empty graph.
- [graphs/1-graph_add_vertex.c](graphs/1-graph_add_vertex.c) adds vertices to the adjacency list.
- [graphs/2-graph_add_edge.c](graphs/2-graph_add_edge.c) adds directed or bidirectional edges.
- [graphs/3-graph_delete.c](graphs/3-graph_delete.c) releases all graph memory.
- [graphs/4-depth_first_traverse.c](graphs/4-depth_first_traverse.c) performs depth-first traversal.
- [graphs/5-breadth_first_traverse.c](graphs/5-breadth_first_traverse.c) performs breadth-first traversal.
- [graphs/graph_display.c](graphs/graph_display.c) prints the graph structure.

## Build

There is no project-wide build system in the repository yet. Compile the files you need together with your own test program, for example:

```bash
gcc -Wall -Wextra -Werror -pedantic -std=c89 -Igraphs *.c test_main.c -o test_graphs
```

Adjust the command to match the files you want to exercise.
