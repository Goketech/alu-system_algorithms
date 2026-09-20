#include "graphs.h"

/* fprintf */
#include <stdio.h>
/* memset */
#include <string.h>
/* malloc free */
#include <stdlib.h>


/**
 * inQueue - linear search to see if a vertex is in the current queue
 *
 * @queue: double pointer to the current head of a queue of vertices
 * @v: pointer to the vertex to search for
 * Return: 1 if vertex is found in queue, 0 if not or failure
 */
int inQueue(BFS_queue_t *queue, vertex_t *v)
{
	BFS_queue_t *temp;

	if (!queue || !v)
		return (0);

	temp = queue;
	while (temp)
	{
		if (temp->v == v)
			return (1);
		temp = temp->next;
	}

	return (0);
}


/**
 * nextInQueue - frees head of a queue of vertices and moves up next in line
 *
 * @queue: double pointer to the current head of a queue of vertices
 */
void nextInQueue(BFS_queue_t **queue)
{
	BFS_queue_t *temp;

	if (!queue || !(*queue))
		return;

	temp = *queue;
	*queue = (*queue)->next;
	free(temp);
}


/**
 * addToQueue - adds a vertex to a queue, and records its depth
 *
 * @queue: double pointer to the current head of a queue of vertices
 * @depth: degrees of separation from the starting vertex of current traversal
 * @next_v: pointer to the vertex to enqueue
 * Return: pointer to added tail of queue on success, or NULL on failure
 */
BFS_queue_t *addToQueue(BFS_queue_t **queue, size_t depth, vertex_t *next_v)
{
	BFS_queue_t *tail = NULL, *temp;

	if (!queue || !next_v)
		return (NULL);

	tail = malloc(sizeof(BFS_queue_t));
	if (!tail)
		return (NULL);
	tail->v = next_v;
	tail->depth = depth;
	tail->next = NULL;

	if (!(*queue))
		*queue = tail;
	else
	{
		temp = *queue;
		while (temp && temp->next)
			temp = temp->next;
		temp->next = tail;
	}

	return (tail);
}


/**
 * BFS_visit - helper to breadth_first_traverse; performs `action` on each
 *   member of queue of vertices, and then adds any connected edges that are
 *   not already visited or in the queue
 *
 * @queue: pointer to the current head of a queue of vertices
 * @visited: array of bytes indicating traversal history: if byte at a given
 *   index is non-zero, then the vertex at the same index in the adjacency list
 *   has already been visited
 * @action: pointer to a function to be called for each visited vertex, takes
 *   the parameters:
 *      v: const pointer to the visited vertex
 *      depth: depth of v, from the starting vertex
 * Return: 0 on success, or 1 on failure
 */
int BFS_visit(BFS_queue_t **queue, unsigned char *visited,
	      void (*action)(const vertex_t *v, size_t depth))
{
	edge_t *temp_e = NULL;
	vertex_t *curr_v = NULL;
	size_t curr_depth;

	if (!queue || !(*queue) || !(*queue)->v || !visited || !action)
		return (1);

	curr_v = (*queue)->v;
	curr_depth = (*queue)->depth;

	visited[curr_v->index] = 1;
	action(curr_v, curr_depth);

	nextInQueue(queue);

	temp_e = curr_v->edges;
	while (temp_e)
	{
		if (!visited[temp_e->dest->index] &&
		    !inQueue(*queue, temp_e->dest))
		{
			if (!addToQueue(queue, curr_depth + 1, temp_e->dest))
				return (1);
		}

		temp_e = temp_e->next;
	}

	return (0);
}



/**
 * breadth_first_traverse - walks through a graph using the breadth-first
 *   algorithm
 *
 * @graph: pointer to the graph to traverse. (Traversal must start from the
 *   first vertex in the vertices list.)
 * @action: pointer to a function to be called for each visited vertex, takes
 *   the parameters:
 *      v: const pointer to the visited vertex
 *      depth: depth of v, from the starting vertex
 * Return: biggest vertex depth, or 0 on failure
 */
size_t breadth_first_traverse(const graph_t *graph,
			      void (*action)(const vertex_t *v, size_t depth))
{
	unsigned char *visited = NULL;
	BFS_queue_t *queue = NULL;
	size_t depth = 0;

	if (!graph || !action)
		return (0);

	visited = malloc(sizeof(unsigned char) * graph->nb_vertices);
	if (!visited)
		return (0);
	memset(visited, 0, graph->nb_vertices);

	/* start queue with first vertex in adjacency list */
	addToQueue(&queue, 0, graph->vertices);
	if (!queue)
	{
		free(visited);
		return (0);
	}

	while (queue)
	{
		depth = queue->depth;
		if (BFS_visit(&queue, visited, action) == 1)
		{
			free(visited);
			while (queue)
				nextInQueue(&queue);
			return (0);
		}
	}

	free(visited);
	return (depth);
}
