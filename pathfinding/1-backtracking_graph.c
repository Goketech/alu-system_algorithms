#include "pathfinding.h"
/* malloc free */
#include <stdlib.h>
/* printf */
#include <stdio.h>
/* strcmp strdup */
#include <string.h>


/**
  * backtrackPath - deletes record of path traveled from dead end back to last
  *   junction point
  *
  * @path: queue of visited graph node content, represents current backtracking
  *   solution candidate
  * @last_fork: vertex at last junction point
  */
void backtrackPath(queue_t *path, const vertex_t *last_fork)
{
	queue_node_t *node = NULL;
	char *vertex_content = NULL;

	if (!path || !path->front || !path->back || !last_fork)
		return;

	node = path->back;
	if (node)
		vertex_content = (char *)node->ptr;

	while (node && vertex_content &&
	       strcmp(vertex_content, last_fork->content) != 0)
	{
		/* dequeue from back */
		if (node && node->prev)
			node->prev->next = NULL;
		path->back = node->prev;
		if (path->back == NULL)
			path->front = NULL;

		free(node);
		free(vertex_content);

		node = path->back;
		if (node)
			vertex_content = (char *)node->ptr;
	}
}


/**
  * isVisitedVertex - checks a queue with `void *` data points, assuming
  *   they are all castable to `vertex_t *`, to find a vertex with content
  *   matching `vertex`
  *
  * @path: queue to search
  * @vertex: vertex containing data to search for
  * Return: first queue node containing matching data, or NULL on failure
  */
queue_node_t *isVisitedVertex(queue_t *path, const vertex_t *vertex)
{
	queue_node_t *temp = NULL;
	char *vertex_content = NULL;

	if (!path || !path->front || !path->back || !vertex)
		return (NULL);

	temp = path->front;
	while (temp)
	{
		if (temp->ptr)
		{
			vertex_content = (char *)temp->ptr;

			if (strcmp(vertex->content, vertex_content) == 0)
				return (temp);
		}

		temp = temp->next;
	}

	return (NULL);
}


/**
  * graphDFS - helper to backtracking_graph, searches for the
  *   first path from a starting point to a target point within a
  *   graph using a recursive depth-first search
  *
  * @path: queue of nodes visited, represents current candidate solution
  * @curr: current vertex in graph
  * @target: target vertex in graph
  * Return: 1 if target found at or below current recursion frame,
  *   0 if not or failure
  */
int graphDFS(queue_t *path, const vertex_t *curr, const vertex_t *target)
{
	char *content_copy = NULL;
	int target_found = 0;
	edge_t *temp_e = NULL;

	if (!path || !curr || !target)
		return (0);

	printf("Checking %s\n", curr->content);

	content_copy = strdup(curr->content);
	if (!content_copy)
	{
		perror("graphDFS: strdup");
		return (0);
	}
	if (!queue_push_back(path, (void *)content_copy))
	{
		free(content_copy);
		return (0);
	}

	if (strcmp(curr->content, target->content) == 0)
		return (1);

	for (temp_e = curr->edges; !target_found && temp_e;
	     temp_e = temp_e->next)
	{
		if (!isVisitedVertex(path, temp_e->dest))
		{
			target_found |= graphDFS(path, temp_e->dest, target);

			if (!target_found)
				backtrackPath(path, curr);
		}
	}

	return (target_found);
}


/**
  * backtracking_graph - searches for the first path from a starting point to
  *   a target point in a graph
  *
  * @graph: pointer to the graph to traverse
  * @start: pointer to starting vertex
  * @target: pointer to target vertex
  * Return: returns a queue in which each node is a `char *` corresponding to a
  *   vertex, forming a path from start to target, or NULL on failure
  */
queue_t *backtracking_graph(graph_t *graph, vertex_t const *start,
			    vertex_t const *target)
{
	queue_t *path = NULL;

	if (!graph || !graph->nb_vertices || !graph->vertices ||
	    !start || !target)
		return (NULL);

	path = queue_create();
	if (!path)
		return (NULL);

	if (!graphDFS(path, start, target))
	{
		while (path->front)
			free(dequeue(path));
		queue_delete(path);
		return (NULL);
	}

	return (path);
}
