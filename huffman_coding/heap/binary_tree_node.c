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
