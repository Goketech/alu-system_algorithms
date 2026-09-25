#include "heap.h"
/* free */
#include <stdlib.h>


/**
 * binaryTreeSize - measures the size of a binary tree
 *
 * @tree: root node from which to measure
 * Return: total of `tree` and all nodes below it, or 0 if `tree` is NULL
 */
size_t binaryTreeSize(const binary_tree_node_t *tree)
{
	if (!tree)
		return (0);

	if (!tree->left && !tree->right)
		return (1);

	return (1 + binaryTreeSize(tree->left) +
		binaryTreeSize(tree->right));
}


/**
 * binaryTreeIsComplete - recursively checks if binary tree is complete (nodes
 *   at each level except 2nd from last all have 2 children, level before last
 *   can have less, but all leaves must be as far left as possible)
 *
 * @tree: root node from which to measure
 * @node_i: iterator to mimic an array index while traversing tree
 * @tree_sz: total number of nodes in tree
 * Return: 1 if `tree` is complete, 0 if not or `tree` is NULL
 *
 * note: this scales `i` in a way that mimics the index values in an array
 *   expression of the tree:
 *
 *         0
 *       /   \
 *      1     2
 *     / \   /
 *    3   4 5
 *
 *   Since the indicies increment top to bottom, left to right, if there are
 *   any in a position that is greater than or equal to (index starts at 0,
 *   size at 1) the size, then we know they are not justified to the left.
 */
int binaryTreeIsComplete(const binary_tree_node_t *tree,
			  size_t node_i, size_t tree_sz)
{
	if (!tree)
		return (1);

	if (node_i >= tree_sz)
		return (0);

	return (binaryTreeIsComplete(tree->left, (2 * node_i) + 1, tree_sz) &&
		binaryTreeIsComplete(tree->right, (2 * node_i) + 2, tree_sz));
}


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
