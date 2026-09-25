#include "_huffman.h"
/* binary_tree_node_t */
#include "heap.h"
/* readBit */
#include "huffman.h"
/* fwrite */
#include <stdio.h>
/* memset */
#include <string.h>


/**
 * decodeSingleChar - recurses down a Huffman tree based on bits read from an
 *   encoded file, ending in the tree leaf designated for a given symbol
 *
 * @h_tree: Huffman tree to interpret Huffman code read from buffer
 * @r_buff: read buffer
 * @r_bit: struct containing indices of current byte and bit in read buffer,
 *     for bit-granular reading
 * Return: pointer to byte decoded from last read Huffman code,
 *   or NULL on failure
 */
char *decodeSingleChar(binary_tree_node_t *h_tree,
		       unsigned char *r_buff, bit_t *r_bit)
{
	unsigned char bit;

	if (!h_tree || !r_buff || !r_bit)
		return (NULL);

	if (!h_tree->left && !h_tree->right)
		return ((char *)(h_tree->data));

	if (readBit(r_buff, r_bit, &bit) == 1)
		return (NULL);

	if (bit == 0)
		return (decodeSingleChar(h_tree->left, r_buff, r_bit));

	return (decodeSingleChar(h_tree->right, r_buff, r_bit));
}


/**
 * huffmanDecode - transcodes bits from read buffer using a reconstituted
 *   Huffman tree into bytes which are used for buffered writing to the output
 *   file
 *
 * @out_file: output file stream
 * @header: header read from input file, containing start and stop indexes for
 *   encoded data
 * @h_tree: Huffman tree extracted from serialization in input file
 * @in_file_size: input file size, in bytes
 * @r_buff: read buffer
 * @r_bit: struct containing indices of current byte and bit in read buffer,
 *     for bit-granular reading
 * Return: 0 on success, 1 on failure
 */
int huffmanDecode(FILE *out_file, huffman_header_t *header,
		  binary_tree_node_t *h_tree, size_t in_file_size,
		  unsigned char *r_buff, bit_t *r_bit)
{
	unsigned char w_buff[BUF_SIZE] = {0};
	char *c = NULL;
	size_t i = 0, size_minus_header, read_bytes;

	if (!out_file || !h_tree || !header || r_buff == NULL || !r_bit)
		return (1);
	if (r_bit->byte_idx != header->hc_byte_offset -
	    sizeof(huffman_header_t))
		return (1);

	size_minus_header = in_file_size - sizeof(huffman_header_t);
	while (r_bit->byte_idx < size_minus_header - 1 ||
	       (r_bit->byte_idx == size_minus_header - 1 &&
		r_bit->bit_idx < header->hc_last_bit_i))
	{
		c = decodeSingleChar(h_tree, r_buff, r_bit);
		if (c == NULL)
			return (1);

		w_buff[i] = *c;
		i++;

		if (i == BUF_SIZE)
		{
			if (fwrite(w_buff, sizeof(unsigned char),
				   BUF_SIZE, out_file) != BUF_SIZE)
				return (1);
			i = 0;
			memset(w_buff, 0, BUF_SIZE);
		}
	}
	if (i != BUF_SIZE) /* file bytes % BUF_SIZE */
	{
		read_bytes = fwrite(w_buff, sizeof(unsigned char),
				    i, out_file);
		if (read_bytes != i)
			return (1);
	}
	return (0);
}
