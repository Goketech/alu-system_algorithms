#include "heap.h"
/* free */
#include <stdlib.h>

/**
 * binaryTreeDelete - Recursively traverses `binary_tree_node_t` binary tree
 *   to delete all nodes. If free_data is not NULL, it will be used to free
 *   the `data` of each node.
 *
 * @tree: pointer to root node of tree or subtree
 * @free_data: pointer to function to use in freeing `data` of each node
*/
void binaryTreeDelete(binary_tree_node_t *tree, void (*free_data)(void *))
{
	if (!tree)
		return;

	binaryTreeDelete(tree->left, free_data);
	binaryTreeDelete(tree->right, free_data);

	if (free_data)
		free_data(tree->data);
	free(tree);
}


/**
 * heap_delete - frees a heap_t binary heap profile, its member binary heap
 *   `root`, plus the member `data` of every node in `root` if `free_data`
 *   is not NULL
 *
 * @heap: pointer to binary heap profile structure to free
 * @free_data: pointer to data freeing function for `data` members of nodes
 *   in `root`
 */
void heap_delete(heap_t *heap, void (*free_data)(void *))
{
	if (!heap)
		return;

	binaryTreeDelete(heap->root, free_data);

	free(heap);
}
