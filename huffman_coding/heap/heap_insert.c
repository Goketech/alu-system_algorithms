#include "heap.h"
/* malloc free */
#include <stdlib.h>


/**
 * BTQueuePush - adds member to rear of a bt_node_queue_t SLL
 *
 * @queue: pointer to head of bt_node_queue_t queue
 * @node: pointer to a node in a binary tree
 * Return: pointer to new node in queue, or NULL on failure
 */
bt_node_queue_t *BTQueuePush(bt_node_queue_t **queue, binary_tree_node_t *node)
{
	bt_node_queue_t *temp, *new = NULL;

	if (!queue || !node)
		return (NULL);

	new = malloc(sizeof(bt_node_queue_t));
	if (!new)
		return (NULL);
	new->node = node;
	new->next = NULL;

	if (!*queue)
		*queue = new;
	else
	{
		temp = *queue;
		while (temp && temp->next)
			temp = temp->next;
		temp->next = new;
	}

	return (new);
}


/**
 * BTQueueAdvance - deletes first member of a bt_node_queue_t SLL
 *
 * @queue: pointer to head of bt_node_queue_t queue
 */
void BTQueueAdvance(bt_node_queue_t **queue)
{
	bt_node_queue_t *temp;

	if (!queue || !*queue)
		return;

	temp = *queue;
	*queue = (*queue)->next;
	free(temp);
}


/**
 * BTQueueClear - deletes all members of a bt_node_queue_t SLL
 *
 * @queue: pointer to head of bt_node_queue_t queue
 */
void BTQueueClear(bt_node_queue_t **queue)
{
	if (!queue)
		return;

	while (*queue)
		BTQueueAdvance(queue);
}


/**
 * BTCompleteInsert - uses queue to insert new leaves into a binary
 *   tree in level order, keeping tree complete

	if (!root)
		return (binary_tree_node(NULL, data));

	queue = BTQueuePush(&queue, root);
	if (!queue)
		return (NULL);

	while (queue)
	{
		if (queue->node->left && queue->node->right)
		{
			if (!BTQueuePush(&queue, queue->node->left) ||
			    !BTQueuePush(&queue, queue->node->right))
			{
				BTQueueClear(&queue);
				return (NULL);
			}

			BTQueueAdvance(&queue);
		}
		else
		{
			new = binary_tree_node(queue->node, data);
			if (!new)
				return (NULL);

			if (!queue->node->left)
				queue->node->left = new;
			else
				queue->node->right = new;

			BTQueueClear(&queue);
		}
	}

	return (new);
}


/**
 * minHeapSiftUp - Intended for use after inserting a new node into a heap.
 *   Restores Min Binary Heap properties of a tree by sifting swapped data
 *   up towards the root position.
 *
 * @new: pointer to newly level-order inserted node in Binary Heap
 * @data_cmp: pointer to function used for data comparison
 * Return: pointer to node in tree that will store new value after sifting
 */
binary_tree_node_t *minHeapSiftUp(binary_tree_node_t *new,
				  int (data_cmp)(void *, void *))
{
	binary_tree_node_t *temp = NULL;
	void *swap;

	if (!data_cmp)
		return (new);

	temp = new;
	while (temp && temp->parent &&
	       data_cmp(temp->data, temp->parent->data) < 0)
	{
		swap = temp->data;
		temp->data = temp->parent->data;
		temp->parent->data = swap;
		temp = temp->parent;
	}

	return (temp);
}


/**
 * heapInsert - inserts a value into a Min Binary Beap
 *
 * @root: pointer to Min Binary Heap root node
 * @data_cmp: data comparsion function pointer
 * @data: pointer containing data to store in the inserted node
 * Return: pointer to the inserted node, or NULL on failure
 */
binary_tree_node_t *heapInsert(binary_tree_node_t **root,
			       int (data_cmp)(void *, void *), void *data)
{
	binary_tree_node_t *new = NULL;

	if (!root || !data_cmp || !data)
		return (NULL);

	/* find next available leaf position in level order */
	new = BTCompleteInsert(*root, data);
	if (!new)
		return (NULL);
	if (!*root)
		*root = new;

	/* sort data towards root if *(temp->data) < *(temp->parent->data) */
	new = minHeapSiftUp(new, data_cmp);

	return (new);
}


/**
 * heap_insert - wraps heapInsert to interface with Binary Heaps contained
 *   in a `heap_t` structure
 *
 * @heap: pointer to a binary heap profile
 * @data: pointer containing data to store in the inserted node
 * Return: pointer to the inserted node, or NULL on failure
 */
binary_tree_node_t *heap_insert(heap_t *heap, void *data)
{
	binary_tree_node_t *new = NULL;

	if (!heap || !data)
		return (NULL);

	new = heapInsert(&(heap->root), heap->data_cmp, data);
	if (new)
		heap->size++;

	return (new);
}
