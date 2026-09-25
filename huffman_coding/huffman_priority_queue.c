/* includes heap.h */
#include "huffman.h"
/* free */
#include <stdlib.h>


/**
 * symbolCompare - compares the frequency of two symbols
 *
 * @p1: void * expected to castable to symbol_t *
 * @p2: void * expected to castable to symbol_t *
 * Return: Positive if frequency 1 is greater, 0 if same values, or negative if
 *   frequency 2 is greater
 */
int symbolCompare(void *p1, void *p2)
{
	symbol_t *sym1, *sym2;
	size_t freq1, freq2;

	sym1 = (symbol_t *)p1;
	sym2 = (symbol_t *)p2;

	freq1 = sym1 ? sym1->freq : 0;
	freq2 = sym2 ? sym2->freq : 0;

	return (freq1 - freq2);
}


/**
 * nestedSymbolCompare - compares the frequency of two symbols stored in
 *   nested nodes
 *
 * @p1: void * expected to castable to binary_tree_node_t * containing data
 *   castable to symbol_t *
 * @p2: void * expected to castable to binary_tree_node_t * containing data
 *   castable to symbol_t *
 * Return: Positive if frequency 1 is greater, 0 if same values, or negative if
 *   frequency 2 is greater
 */
int nestedSymbolCompare(void *p1, void *p2)
{
	binary_tree_node_t *node1, *node2;
	symbol_t *sym1, *sym2;
	size_t freq1, freq2;

	node1 = (binary_tree_node_t *)p1;
	node2 = (binary_tree_node_t *)p2;

	sym1 = node1 ? (symbol_t *)(node1->data) : NULL;
	sym2 = node2 ? (symbol_t *)(node2->data) : NULL;

	freq1 = sym1 ? sym1->freq : 0;
	freq2 = sym2 ? sym2->freq : 0;

	return (freq1 - freq2);
}



/**
 * freeSymbol - meant to be used as free_data parameter to heap_delete;
 *   frees memory allocated for a symbol_t symbol
 *
 * @data: void pointer intended to be cast into data pointer
 */
void freeSymbol(void *data)
{
	symbol_t *s_data = NULL;

	s_data = (symbol_t *)data;

	if (s_data)
		free(s_data);
}


/**
 * freeNestedNode - meant to be used as free_data parameter to heap_delete;
 *   frees memory allocated for a binary_tree_node_t node containing a
 *   symbol_t struct
 *
 * @data: void pointer intended to be cast into binary_tree_node_t pointer
 */
void freeNestedNode(void *data)
{
	binary_tree_node_t *n_data = NULL;
	symbol_t *s_data = NULL;

	n_data = (binary_tree_node_t *)data;

	if (n_data)
	{
		s_data = (symbol_t *)(n_data->data);

		if (s_data)
			free(s_data);

		free(n_data);
	}
}


/**
 * huffman_priority_queue - Creates a priority queue for use in building a
 *   Huffman Tree.
 *
 *     The priority queue begins as a Binary Heap that contains as data at each
 *   node a pointer to another initially unnetworked `binary_tree_node_t`
 *   node, which itself contains a `symbol_t` as data. The priority_queue tree
 *   is sorted by the frequency values of the symbols in these nested data
 *   nodes.
 *
 *     Eventually a Huffman Tree will be constructed as values are extracted,
 *   summed and reinserted into the priority queue. The queue serves to keep
 *   track of how many operations remain in the Huffman Tree construction, and
 *   how to assign the parent/left/right pointers of the nested nodes during
 *   construction.
 *
 * @data: array of characters to store in Huffman tree
 * @freq: frequency of appearance of character at each corresponding index
 *   in `data`
 * @size: amount of members in both `data` and `freq`
 * Return: heap_t containing priority queue that refers to an unsummed Huffman
 *   Tree, or NULL on failure
 */
heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size)
{
	binary_tree_node_t *ht_node = NULL, *pq_node = NULL;
	heap_t *priority_queue = NULL;
	symbol_t *symbol = NULL;
	size_t i;

	if (!data || !freq || size == 0)
		return (NULL);

	priority_queue = heap_create(nestedSymbolCompare);
	if (!priority_queue)
		return (NULL);

	for (i = 0; i < size; i++)
	{
		symbol = symbol_create(data[i], freq[i]);
		ht_node = binary_tree_node(NULL, symbol);
		pq_node = heap_insert(priority_queue, ht_node);

		if (!symbol || !ht_node || !pq_node)
		{
			heap_delete(priority_queue, freeNestedNode);
			return (NULL);
		}
	}

	return (priority_queue);
}
