#include "_huffman.h"
/* binary_tree_node binary_tree_node_t */
#include "heap.h"
/* symbol_t */
#include "huffman.h"
/* malloc */
#include <stdlib.h>


/**
 * huffmanSerialize - recursively traverses a Huffman tree to serialize its
 *   structure into a write buffer
 *
 * @h_tree: head of Huffman tree (min binary heap of symbol frequencies)
 * @w_buff: write buffer
 * @w_bit: struct containing indices of current byte and bit in write buffer,
 *     for bit-granular writing
 */
void huffmanSerialize(binary_tree_node_t *h_tree,
		      unsigned char *w_buff, bit_t *w_bit)
{
	if (!h_tree || !w_buff || !w_bit)
		return;

	if (!h_tree->left && !h_tree->right)
	{
		writeBit(w_buff, w_bit, 1);
		writeByte(w_buff, w_bit,
			  ((symbol_t *)(h_tree->data))->data);
		return;
	}

	writeBit(w_buff, w_bit, 0);

	huffmanSerialize(h_tree->left, w_buff, w_bit);
	huffmanSerialize(h_tree->right, w_buff, w_bit);
}


/**
 * huffmanDeserialize - reads custom serialization of a Huffman tree from a
 *   compressed file and recursively reconstructs the tree data structure
 *
 * @r_buff: read buffer
 * @r_bit: struct containing indices of current byte and bit in read buffer,
 *     for bit-granular reading
 * @parent: parent tree node from previous frame of recursion
 * Return: new tree node, or NULL on failure
 */
binary_tree_node_t *huffmanDeserialize(unsigned char *r_buff, bit_t *r_bit,
				       binary_tree_node_t *parent)
{
	binary_tree_node_t *new = NULL;
	unsigned char bit, *c = NULL;

	if (!r_buff || !r_bit)
		return (NULL);

	readBit(r_buff, r_bit, &bit);

	if (bit == 1)
	{
		c = malloc(sizeof(unsigned char));
		if (!c)
			return (NULL);
		readByte(r_buff, r_bit, c);
		new = binary_tree_node(parent, c);
		return (new);
	}

	new = binary_tree_node(parent, NULL);
	if (!new)
		return (NULL);

	new->left = huffmanDeserialize(r_buff, r_bit, new);
	new->right = huffmanDeserialize(r_buff, r_bit, new);

	return (new);
}
