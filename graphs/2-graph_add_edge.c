#include "graphs.h"

/* malloc */
#include <stdlib.h>
/* strlen strcmp */
#include <string.h>


/**
 * graph_add_single_edge - adds a single new edge between two vertices in an
 * existing graph
 *
 * @src: node originating the connection
 * @dest: node receiving the connection
 * Return: pointer to new edge on success, or NULL on failure
 */
edge_t *graph_add_single_edge(vertex_t *src, vertex_t *dest)
{
	edge_t *new = NULL, *temp = NULL;

	if (!src || !dest)
		return (NULL);

	new = malloc(sizeof(edge_t));
	if (!new)
		return (NULL);
	new->dest = dest;
	new->next = NULL;

	temp = src->edges;
	while (temp && temp->next)
		temp = temp->next;

	if (temp)
		temp->next = new;
	else
		src->edges = new;
	src->nb_edges++;

	return (new);
}


/**
 * graph_add_edge - adds an edge between two vertices in an existing graph
 *
 * @graph: pointer to the graph to which edge is added
 * @src: string identifying the starting vertex in the connection
 * @dest: string identifying the ending vertex in the connection
 * @type: type of edge, valid values are:
 *          UNIDIRECTIONAL: only one edge is created from src to dest
 *          BIDIRECTIONAL: two edges created: from src to dest AND dest to src
 * Return: 1 on success, or 0 on failure
 */
int graph_add_edge(graph_t *graph, const char *src,
		   const char *dest, edge_type_t type)
{
	vertex_t *temp_v = NULL, *src_v = NULL, *dest_v = NULL;

	if (!graph || !src || !dest ||
	    type < UNIDIRECTIONAL || type > BIDIRECTIONAL)
		return (0);

	for (temp_v = graph->vertices; temp_v; temp_v = temp_v->next)
	{
		if (strcmp(temp_v->content, src) == 0)
			src_v = temp_v;
		else if (strcmp(temp_v->content, dest) == 0)
			dest_v = temp_v;
	}

	if (!src_v || !dest_v)
		return (0);

	if (graph_add_single_edge(src_v, dest_v) == NULL)
		return (0);

	if (type == BIDIRECTIONAL)
	{
		if (graph_add_single_edge(dest_v, src_v) == NULL)
			return (0);
	}

	return (1);
}
