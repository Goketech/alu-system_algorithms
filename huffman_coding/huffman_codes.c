/* includes heap.h */
#include "huffman.h"
/* malloc calloc free */
#include <stdlib.h>
/* printf */
#include <stdio.h>


/**
 * printAllHuffmanCodes - recursively traverses a Huffman tree to print the
 *   Huffman code for the symbol (byte value) at each leaf
 *
 * @tree: root node of Huffman tree
 * @depth: level of tree, or implicitly the frame of recursion
 * @data: array of byte values
 * @freq: array of corresponding frequencies for the byte values in `data`
 * @size: amount of members in both `data` and `freq`
 * @h_code: array holding partially built Huffman code
 */
void printAllHuffmanCodes(binary_tree_node_t *tree, size_t depth,
			  char *data, size_t *freq, size_t size,
			  char *h_code)
{
	symbol_t *symbol = NULL;
	size_t i;

	if (!tree || !h_code)
		return;

	if (depth)
		h_code[depth - 1] = (tree == tree->parent->left) ? '0' : '1';

	symbol = (symbol_t *)(tree->data);
	for (i = 0; i < size; i++)
	{
		if (symbol->data == data[i] && symbol->freq == freq[i])
		{
			printf("%c: %s\n", data[i], h_code);
			return;
		}
	}

	printAllHuffmanCodes(tree->left, depth + 1,
			     data, freq, size, h_code);
	printAllHuffmanCodes(tree->right, depth + 1,
			     data, freq, size, h_code);
}


/**
 * deriveHuffmanCode - builds a Huffamn code for a single symbol by recusively
 *   traversing a Huffman tree
 *
 * @tree: Huffman tree, or Min Binary Heap of symbols sorted by frequency
 * @data: symbol (byte value) to encode
 * @freq: amount of appearances in source file that `data` byte appears, value
 *   that orders Huffman tree
 * @depth: current level of tree, and implicitly frame of recursion
 * Return: string containing bit values of Huffman code as '1's and '0's, or
 *   NULL on failure
 */
char *deriveHuffmanCode(binary_tree_node_t *tree,
			char data, size_t freq, size_t depth)
{
	symbol_t *symbol = NULL;
	char *h_code = NULL, *L_return = NULL, *R_return = NULL;

	if (!tree)
		return (NULL);

	symbol = (symbol_t *)(tree->data);
	if (symbol->data == data && symbol->freq == freq)
	{
		h_code = malloc(sizeof(char) * (depth + 1));
		if (!h_code)
			return (NULL);
		h_code[depth] = '\0';
		if (depth)
			h_code[depth - 1] =
				(tree == tree->parent->left) ? '0' : '1';
		return (h_code);
	}

	L_return = deriveHuffmanCode(tree->left, data, freq, depth + 1);
	if (L_return)
	{
		if (depth)
			L_return[depth - 1] =
				(tree == tree->parent->left) ? '0' : '1';
		return (L_return);
	}

	R_return = deriveHuffmanCode(tree->right, data, freq, depth + 1);
	if (R_return)
	{
		if (depth)
			R_return[depth - 1] =
				(tree == tree->parent->left) ? '0' : '1';
		return (R_return);
	}

	return (NULL);
}


/**
 * huffman_codes - builds a Huffman tree and prints the resulting Huffman codes
 *   for each symbol
 *
 * @data: array of byte values
 * @freq: array of corresponding frequencies for the byte values in `data`
 * @size: amount of members in both `data` and `freq`
 * Return: 1 on success, 0 on failure
 */
int huffman_codes(char *data, size_t *freq, size_t size)
{
	binary_tree_node_t *h_tree = NULL;
	char *h_code = NULL;

	if (!data || !freq || size == 0)
		return (0);

	h_tree = huffman_tree(data, freq, size);
	if (!h_tree)
		return (0);

	h_code = calloc(size, sizeof(char));
	if (!h_code)
		return (0);

	printAllHuffmanCodes(h_tree, 0, data, freq, size, h_code);
	free(h_code);

	binaryTreeDelete(h_tree, freeSymbol);

	return (1);
}
