#include "pathfinding.h"
/* malloc free qsort */
#include <stdlib.h>
/* printf */
#include <stdio.h>
/* strcmp strdup */
#include <string.h>
/* ULONG_MAX */
#include <limits.h>


/**
  * pathFromDijkstraQueue - once a Dijkstra priority queue has been populated
  *   by traversing the graph, this function works backwards through the
  *   priority queue to build a solution queue by following the path_via
  *   pointers from target to start
  *
  * @d_queue: finished Dijkstra priority queue
  * @target_i: index of target vertex in priority queue
  * Return: queue containing the content strings from each node in the graph
  *   from start to target, or NULL on failure
  */
queue_t *pathFromDijkstraQueue(dijkstra_vertex_t *d_queue,
			       size_t target_i)
{
	queue_t *path = NULL;
	vertex_t *via = NULL;
	char *via_name = NULL;
	long int i;

	if (!d_queue)
		return (NULL);

	path = queue_create();
	if (!path)
		return (NULL);

	via_name = strdup(d_queue[target_i].vertex->content);
	queue_push_front(path, (void *)via_name);
	via = d_queue[target_i].path_via;

	for (i = target_i; i >= 0; i--)
	{
		if (d_queue[i].vertex == via)
		{
			via_name = strdup(d_queue[i].vertex->content);
			queue_push_front(path, (void *)via_name);
			via = d_queue[i].path_via;
		}
	}

	if (via != NULL) /* no contiguous path from target to start */
	{
		while (path->front)
			free(dequeue(path));
		queue_delete(path);
		path = NULL;
	}

	return (path);
}


/**
  * compareWeights - intended for use with qsort(), sorts members of the
  *   Dijkstra priority queue by their cumulative weights (made into stable
  *   sort by using pointer values as tiebreaker)
  *
  * @param1: void * due to function prototype expected by qsort(),
  *   expected to be castable to dijkstra_vertex *
  * @param2: void * due to function prototype expected by qsort(),
  *   expected to be castable to dijkstra_vertex *
  * Return: 1 if param1 should be ordered first, -1 if param2 is first,
  *   or 0 if they are the same
  */
int compareWeights(const void *param1, const void *param2)
{
	const dijkstra_vertex_t *dv1 = (const dijkstra_vertex_t *)param1;
	const dijkstra_vertex_t *dv2 = (const dijkstra_vertex_t *)param2;

	if (dv1->cml_weight < dv2->cml_weight)
		return (-1);
	if (dv1->cml_weight > dv2->cml_weight)
		return (1);

	/* tiebreaker for stable sort */
	if (dv1->vertex < dv2->vertex)
		return (-1);
	if (dv1->vertex > dv2->vertex)
		return (1);

	return (0);
}


/**
  * assessEdges - checks the edges of the vertex at the head of the priority
  *   queue, updating the cumulative weight and path_via of each neighbor if a
  *   shorter path is found through the head, then re-sorts the remaining queue
  *
  * @d_queue: array of structs containing current vertex, cumulative weight,
  *   previous vertex in current optimal route
  * @nb_vertices: total amount of vertices in graph
  * @dq_head_i: index in d_queue that marks the current head of the priority
  *   queue
  */
void assessEdges(dijkstra_vertex_t *d_queue, size_t nb_vertices,
		 size_t dq_head_i)
{
	dijkstra_vertex_t dq_head;
	edge_t *temp_e = NULL;
	size_t i;

	if (!d_queue)
		return;

	dq_head = d_queue[dq_head_i];

	for (temp_e = dq_head.vertex->edges; temp_e; temp_e = temp_e->next)
	{
		for (i = dq_head_i; i < nb_vertices; i++)
		{
			if (temp_e->dest == d_queue[i].vertex)
			{
				if (dq_head.cml_weight + temp_e->weight <
				    d_queue[i].cml_weight)
				{
					d_queue[i].cml_weight =
						dq_head.cml_weight +
						temp_e->weight;

					d_queue[i].path_via = dq_head.vertex;
				}
			}
		}
	}

	dq_head_i++;

	qsort((void *)(d_queue + dq_head_i), nb_vertices - dq_head_i,
	      sizeof(dijkstra_vertex_t), compareWeights);
}


/**
  * dijkstraGraph - updates an array-based priority queue to find smallest
  *   cumulative weights and optimal previous vertex for each vertex in a graph
  *
  * @d_queue: array of structs containing current vertex, cumulative weight,
  *   previous vertex in current optimal route
  * @nb_vertices: total amount of vertices in graph
  * @start: pointer to starting vertex
  * @target: pointer to target vertex
  * @dq_head_i: index in d_queue that marks the current head of the priority
  *   queue; any indexes lower than this in the array have finished assessment
  * @target_i: once assessment of the graph is complete and the target is
  *   found, this is updated to the final index of the target vertex in d_queue
  * Return: 0 on success, 1 on failure or if target not found
  */
int dijkstraGraph(dijkstra_vertex_t *d_queue, size_t nb_vertices,
		   const vertex_t *start, const vertex_t *target,
		   size_t dq_head_i, size_t *target_i)
{
	dijkstra_vertex_t dq_head;

	if (!d_queue || !start || !target || !target_i)
		return (1);

	dq_head = d_queue[dq_head_i];
	printf("Checking %s, distance from %s is %lu\n",
	       dq_head.vertex->content, start->content, dq_head.cml_weight);

	assessEdges(d_queue, nb_vertices, dq_head_i);

	if (strcmp(target->content, dq_head.vertex->content) == 0)
	{
		*target_i = dq_head_i;
		return (0);
	}

	/* no more to assess */
	if (dq_head_i == nb_vertices - 1)
		return (1);

	/* no more viable paths remain */
	if (d_queue[dq_head_i + 1].cml_weight == ULONG_MAX ||
	    d_queue[dq_head_i + 1].path_via == NULL)
		return (1);

	return (dijkstraGraph(d_queue, nb_vertices, start, target,
			     dq_head_i + 1, target_i));
}


/**
  * dijkstra_graph - searches for the shortest path from a starting point to a
  *   target point in a graph, based on edge weights
  *
  * @graph: pointer to the graph to traverse
  * @start: pointer to starting vertex
  * @target: pointer to target vertex
  * Return: returns a queue in which each node is a `char *` corresponding to
  *   vertex, forming a path from start to target, or NULL on failure
  */
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target)
{
	dijkstra_vertex_t *dijkstra_queue = NULL;
	vertex_t *temp_v = NULL;
	size_t i, target_i;
	queue_t *path = NULL;

	if (!graph || !graph->nb_vertices || !graph->vertices ||
	    !start || !target)
		return (NULL);

	dijkstra_queue = malloc(sizeof(dijkstra_vertex_t) * graph->nb_vertices);
	if (!dijkstra_queue)
		return (NULL);

	for (i = 0, temp_v = graph->vertices; i < graph->nb_vertices;
	     i++, temp_v = temp_v->next)
	{
		dijkstra_queue[i].vertex = temp_v;

		if (strcmp(start->content, temp_v->content) == 0)
			dijkstra_queue[i].cml_weight = 0;
		else
			dijkstra_queue[i].cml_weight = ULONG_MAX;

		dijkstra_queue[i].path_via = NULL;
	}

	qsort((void *)dijkstra_queue, graph->nb_vertices,
	      sizeof(dijkstra_vertex_t), compareWeights);

	if (dijkstraGraph(dijkstra_queue, graph->nb_vertices,
			  start, target, 0, &target_i) != 0)
	{
		free(dijkstra_queue);
		return (NULL);
	}

	/* assemble path backwards from dijkstra_queue */
	path = pathFromDijkstraQueue(dijkstra_queue, target_i);
	free(dijkstra_queue);
	return (path);
}
