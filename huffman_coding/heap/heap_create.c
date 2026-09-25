#include "heap.h"
/* malloc */
#include <stdlib.h>

/**
 * heap_create - allocates a new heap_t heap data structure profile
 *
 * @data_cmp: pointer to a comparison function
 * Return: pointer to the created heap_t structure, or NULL on failure
 */
heap_t *heap_create(int (*data_cmp)(void *, void *))
{
	heap_t *new = NULL;

	new = malloc(sizeof(heap_t));
	if (!new)
		return (NULL);

	new->size = 0;
	new->data_cmp = data_cmp;
	new->root = NULL;

	return (new);
}
