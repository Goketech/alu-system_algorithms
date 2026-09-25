#include "_huffman.h"
/* binary_tree_node_t */
#include "heap.h"
/* FILE fopen printf */
#include <stdio.h>
/* free */
#include <stdlib.h>
/* strncmp */
#include <string.h>


/**
 * freeChar - meant to be used as free_data parameter to heap_delete or
 *   binaryTreeDelete; frees memory allocated for a char
 *
 * @data: void pointer intended to be cast into char *
 */
void freeChar(void *data)
{
	if (data)
		free((char *)data);
}


/* currently only supporting input files of less than BUF_SIZE bytes */
/**
 * huffmanDecompress - reads input file to reconstruct the Huffman tree used
 *   during encoding, and then uses this tree to decode the remaining bits
 *   back into the original bytes of the source file before compression
 *
 * @in_file: intput file stream
 * @out_file: output file stream
 * @in_file_size: input file size, in bytes
 * Return: 0 on success, 1 on failure
 */
int huffmanDecompress(FILE *in_file, FILE *out_file, long int in_file_size)
{
	unsigned char r_buff[BUF_SIZE] = {0};
	bit_t r_bit = {0, 0, 0};
	huffman_header_t header;
	binary_tree_node_t *h_tree = NULL;
	size_t read_bytes;

	if (!in_file || !out_file || in_file_size < 0)
		return (1);

	if (fread(&header, sizeof(huffman_header_t), 1, in_file) != 1)
		return (1);

	/* check custom "magic" file identifier */
	if (strncmp((char *)(header.huff_id), "\177HUF", 4) != 0)
	{
		printf("Input is not a file compressed by this program!\n");
		return (1);
	}

	/* serialized trees should be <= 2*CHAR_RANGE bytes in file */
	read_bytes = fread(r_buff, sizeof(unsigned char),
			   BUF_SIZE, in_file);
	/* currently only supporting input files of less than BUF_SIZE bytes */
	if (read_bytes != in_file_size - sizeof(huffman_header_t))
		return (1);

	h_tree = huffmanDeserialize(r_buff, &r_bit, NULL);
	if (!h_tree)
		return (1);

	if (huffmanDecode(out_file, &header, h_tree, (size_t)in_file_size,
			  r_buff, &r_bit) == 1)
	{
		binaryTreeDelete(h_tree, freeChar);
		return (1);
	}
	binaryTreeDelete(h_tree, freeChar);

	return (0);
}
