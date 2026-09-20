#include "graphs.h"

/* memset */
#include <string.h>
/* malloc free */
#include <stdlib.h>

#define _MAX(a, b) ((a) > (b) ? (a) : (b))


/**
 * DFS_recursion - recursive helper to depth_first_traverse; recursively
 *   traverses a graph using the depth-first algorithm to return its maximum
 *   depth
 *
 * @curr: pointer to the current vertex (starting vertex is abitrary, but
 *   depth_first_traverse expects to start with first vertex in adjacency list)
 * @visited: array of bytes indicating traversal history: if byte at a given
 *   index is non-zero, then the vertex at the same index in the adjacency list
 *   has already been visited
 * @curr_depth: depth of current vertex from the starting node
 * @action: pointer to a function to be called for each visited vertex, takes
 *   the parameters:
 *      v: const pointer to the visited vertex
 *      depth: depth of v, from the starting vertex
 * Return: largest vertex depth, or `curr_depth` on failure
 */
size_t DFS_recursion(vertex_t *curr, unsigned char *visited, size_t curr_depth,
		     void (*action)(const vertex_t *v, size_t depth))
{
	edge_t *temp_e = NULL;
	size_t max_depth, subtree_depth;

	if (!curr || !visited || !action)
		return (curr_depth);

	/* mark vertex as visited */
	visited[curr->index] = 1;

	/* process node */
	action(curr, curr_depth);

	/* visit edges leading from current that are not yet visited */
	max_depth = curr_depth;
	temp_e = curr->edges;
	while (temp_e)
	{
		if (!visited[temp_e->dest->index])
		{
			subtree_depth = DFS_recursion(temp_e->dest, visited,
						      curr_depth + 1, action);
			max_depth = _MAX(max_depth, subtree_depth);
		}

		temp_e = temp_e->next;
	}

	/* return depth of deepest "subtree" when graph is flattened */
	return (max_depth);

}


/**
 * depth_first_traverse - walks through a graph using the depth-first algorithm
 *
 * @graph: pointer to the graph to traverse. (Traversal must start from the
 *   first vertex in the vertices list.)
 * @action: pointer to a function to be called for each visited vertex, takes
 *   the parameters:
 *      v: const pointer to the visited vertex
 *      depth: depth of v, from the starting vertex
 * Return: biggest vertex depth, or 0 on failure
 */
size_t depth_first_traverse(const graph_t *graph,
			    void (*action)(const vertex_t *v, size_t depth))
{
	unsigned char *visited = NULL;
	size_t total_depth;

	if (!graph || !action)
		return (0);

	visited = malloc(sizeof(unsigned char) * graph->nb_vertices);
	if (!visited)
		return (0);
	memset(visited, 0, graph->nb_vertices);

	total_depth = DFS_recursion(graph->vertices, visited, 0, action);
	free(visited);
	return (total_depth);
}
