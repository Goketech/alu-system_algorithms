/* includes heap.h */
#include "huffman.h"
/* free */
#include <stdlib.h>

#include <stdio.h>

/**
 * huffmanSumNode - sums frequencies of two symbols stored in two extracted
 *    nodes, and creates a new node to contain the sum
 *
 * @ex_node1: first nested node extracted from priority queue
 * @ex_node2: second nested node extracted from priority queue
 * Return: new node containing sum of frequencies, or NULL on failure
 */
binary_tree_node_t *huffmanSumNode(binary_tree_node_t *ex_node1,
				   binary_tree_node_t *ex_node2)
{
	binary_tree_node_t *sum_node = NULL;
	symbol_t *sum_sym = NULL;
	size_t sum_freq;

	if (!ex_node1 || !ex_node2)
		return (NULL);

	sum_freq = ((symbol_t *)(ex_node1->data))->freq +
		((symbol_t *)(ex_node2->data))->freq;

	sum_sym = symbol_create(-1, sum_freq);
	if (!sum_sym)
		return (NULL);

	sum_node = binary_tree_node(NULL, sum_sym);
	if (!sum_node)
	{
		free(sum_sym);
		return (NULL);
	}

	return (sum_node);
}


/**
 * huffman_extract_and_insert - Sums values in a Huffman tree represented by a
 *   priority queue.
 *
 *     At the outset, the queue should be a Min Binary Heap containing as data
 *   at each node a second untethered node which itself contains symbol data.
 *   The priority queue should be sorted by the frequencies in these nested
 *   node symbols.
 *
 *     Two nodes are extracted from the queue, and then one is inserted. The
 *   new queue node contains a new nested node, with a frequency value the sum
 *   of the two extraced nodes' frequencies. The nodes nested in the two
 *   extracted nodes are then linked to the new nested as its children. By
 *   iterating this process a Huffman tree is constructed among the nested
 *   nodes.
 *
 * @priority_queue: pointer to Min Binary Heap which acts as a priority queue
 *   by representing nodes used in construction of a Huffman tree
 * Return: 1 on success or 0 on failure
 */
int huffman_extract_and_insert(heap_t *priority_queue)
{
	void *ex_data1 = NULL, *ex_data2 = NULL;
	binary_tree_node_t *sum_node = NULL,
		*ex_node1 = NULL, *ex_node2 = NULL;

	if (!priority_queue ||/* !priority_queue->data_cmp || */
	    !priority_queue->root || !priority_queue->root->data)
		return (0);

	/* expecting nestedSymbolCompare */
	priority_queue->data_cmp = nestedSymbolCompare;

	/* extract nested nodes */
	ex_data1 = heap_extract(priority_queue);
	ex_node1 = (binary_tree_node_t *)ex_data1;
	ex_data2 = heap_extract(priority_queue);
	ex_node2 = (binary_tree_node_t *)ex_data2;
	if (!ex_node1 || !ex_node2)
		return (0);

	/* create sum node */
	sum_node = huffmanSumNode(ex_node1, ex_node2);
	if (!sum_node)
		return (0);

	/* nest sum node into new node in queue */
	if (!heap_insert(priority_queue, sum_node))
	{
		free(sum_node->data);
		free(sum_node);
		return (0);
	}

	/* knit sum node into Huffman tree position */
	sum_node->left = ex_node1;
	ex_node1->parent = sum_node;
	sum_node->right = ex_node2;
	ex_node2->parent = sum_node;

	return (1);
}
