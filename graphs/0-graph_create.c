#include "graphs.h"

/* malloc */
#include <stdlib.h>

/**
 * graph_create - allocates memory to store a graph_t structure, and
 *   initializes its content
 *
 * Return: pointer to the allocated structure, or NULL on failure
 */
graph_t *graph_create(void)
{
	graph_t *graph = NULL;

	graph = malloc(sizeof(graph_t));
	if (!graph)
		return (NULL);

	graph->nb_vertices = 0;
	graph->vertices = NULL;

	return (graph);
}
