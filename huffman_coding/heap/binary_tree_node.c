#include "heap.h"
/* malloc */
#include <stdlib.h>


/**
 * binary_tree_node - creates a generic Binary Tree node
 *
 * @parent: pointer to node assigned as parent of created node
 * @data: data to be stored in the node
 * Return: pointer to the created node, or NULL on failure
 */
binary_tree_node_t *binary_tree_node(binary_tree_node_t *parent, void *data)
{
	binary_tree_node_t *new = NULL;

	new = malloc(sizeof(binary_tree_node_t));
	if (!new)
		return (NULL);

	new->data = data;
	new->left = NULL;
	new->right = NULL;
	new->parent = parent;

	return (new);
}


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
