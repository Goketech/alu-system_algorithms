#include "heap.h"
/* free */
#include <stdlib.h>


/**
 * lastInLevelOrder - uses queue to return last level order node in a complete
 *   binary tree
 *
 * @root: pointer to root node of tree to traverse
 * Return: pointer to the last node, or NULL on failure
 */
binary_tree_node_t *lastInLevelOrder(binary_tree_node_t *root)
{
	binary_tree_node_t *last = NULL;
	bt_node_queue_t *queue = NULL;

	if (!root ||
	    !binaryTreeIsComplete(root, 0, binaryTreeSize(root)))
		return (NULL);

	queue = BTQueuePush(&queue, root);
	if (!queue)
		return (NULL);

	while (queue)
	{
		last = queue->node;

		if (queue->node->left)
			BTQueuePush(&queue, queue->node->left);
		if (queue->node->right)
			BTQueuePush(&queue, queue->node->right);

		BTQueueAdvance(&queue);
	}

	return (last);
}


/**
 * minHeapSiftDown - Intended for use after swapping the data in the root and
 *   last level order node of a Binary Heap in preparation for extraction.
 *   Restores Min Binary Heap properties of a tree by sifting swapped data
 *   down from root position.
 *
 * @root: pointer to Min Binary Heap root node
 * @data_cmp: data comparsion function pointer
 */
void minHeapSiftDown(binary_tree_node_t *root,
		     int (data_cmp)(void *, void *))
{
	binary_tree_node_t *temp = NULL, *next = NULL;
	void *swap;

	if (!data_cmp)
		return;

	temp = root;
	while (temp && temp->left)
	{
		/* defaults to sifting down if parent == min child */
		/* and to left if left == right */
		if (!temp->right ||
		    data_cmp(temp->left->data, temp->right->data) <= 0)
		{
			if (data_cmp(temp->data, temp->left->data) >= 0)
				next = temp->left;
			else
				break;
		}
		else if (data_cmp(temp->data, temp->right->data) >= 0)
			next = temp->right;
		else
			break;

		if (next)
		{
			swap = temp->data;
			temp->data = next->data;
			next->data = swap;
		}

		temp = next;
	}
}


/**
 * heapExtract - extacts the value at the root of a Min Binary Heap, expects
 *   heap to already be a complete binary tree
 *
 * @root: pointer to Min Binary Heap root node
 * @data_cmp: data comparsion function pointer
 * Return: pointer to data from root node of binary heap
 */
void *heapExtract(binary_tree_node_t **root,
		  int (data_cmp)(void *, void *))
{
	binary_tree_node_t *last = NULL;
	void *extract = NULL;

	if (!root || !data_cmp)
		return (NULL);

	last = lastInLevelOrder(*root);
	if (!last)
		return (NULL);

	/* data from last node in level order copied into root node */
	extract = (*root)->data;
	(*root)->data = last->data;

	/* sift this copied data down to heapify back to Min Binary Heap */
	minHeapSiftDown(*root, data_cmp);

	/* once swapped and re-heapified, delete last level order node */
	if (last->parent)
	{
		if (last->parent->left == last)
			last->parent->left = NULL;
		else if (last->parent->right == last)
			last->parent->right = NULL;
	}

	if (last == *root)
		*root = NULL;
	free(last);

	return (extract);
}


/**
 * heap_extract - wraps heapExtract to interface with Binary Heaps contained
 *   in a `heap_t` structure
 *
 * @heap: pointer to a heap_t binary heap profile structure
 * Return: pointer to data from root node of binary heap
 */
void *heap_extract(heap_t *heap)
{
	void *extract = NULL;

	if (!heap || !heap->data_cmp || !heap->root)
		return (NULL);

	extract = heapExtract(&(heap->root), heap->data_cmp);
	if (extract != NULL)
		heap->size--;

	return (extract);
}
