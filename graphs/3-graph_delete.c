#include "graphs.h"

/* free */
#include <stdlib.h>

/**
 * graph_delete - completely deletes a graph
 *
 * @graph: pointer to the graph to delete
 */
void graph_delete(graph_t *graph)
{
	vertex_t *temp1_v = NULL, *temp2_v = NULL;
	edge_t *temp1_e = NULL, *temp2_e = NULL;

	if (!graph)
		return;

	temp1_v = graph->vertices;
	while (temp1_v)
	{
		temp2_v = temp1_v->next;

		if (temp1_v->content)
			free(temp1_v->content);

		temp1_e = temp1_v->edges;
		while (temp1_e)
		{
			temp2_e = temp1_e->next;
			free(temp1_e);
			temp1_e = temp2_e;
		}

		free(temp1_v);
		temp1_v = temp2_v;
	}

	free(graph);
}
