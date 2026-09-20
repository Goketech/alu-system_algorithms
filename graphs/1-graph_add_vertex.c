#include "graphs.h"

/* malloc */
#include <stdlib.h>
/* strlen strcpy */
#include <string.h>


/**
 * graph_add_vertex - adds a vertex to an existing graph; stores copy of `str`
 *   in new vertex unless `str` already appears in another vertex
 *
 * @graph: pointer to the graph to which vertex is added
 * @str: string to store in the new vertex
 * Return: pointer to the created vertex, or NULL on failure
 */
vertex_t *graph_add_vertex(graph_t *graph, const char *str)
{
	vertex_t *temp = NULL, *tail = NULL, *new = NULL;
	size_t i, str_len;
	char *str_cpy = NULL;

	if (graph == NULL || str == NULL)
		return (NULL);
	/* check if string already exists in graph */
	for (i = 0, temp = graph->vertices; i < graph->nb_vertices; i++)
	{
		if (!temp || strcmp(temp->content, str) == 0)
			return (NULL);
		if (i == graph->nb_vertices - 1 && !temp->next)
			tail = temp;
		temp = temp->next;
	}
	/* new vertex */
	new = malloc(sizeof(vertex_t));
	if (!new)
		return (NULL);
	/* stores copy of string, not original */
	str_len = strlen(str);
	str_cpy = malloc(sizeof(char) * str_len + 1);
	if (!str_cpy)
	{
		free(new);
		return (NULL);
	}
	str_cpy = strcpy(str_cpy, str);
	new->index    = i;
	new->content  = str_cpy;
	new->nb_edges = 0;
	new->edges    = NULL;
	new->next     = NULL;
	/* set vertex as tail of adjacency list, or head if none exists */
	if (tail)
		tail->next = new;
	else
		graph->vertices = new;
	graph->nb_vertices++;
	return (new);
}
