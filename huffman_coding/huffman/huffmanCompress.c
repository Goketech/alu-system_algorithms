#include "_huffman.h"
/* binary_tree_node_t */
#include "heap.h"
/* freeSymbol huffman_tree */
#include "huffman.h"
/* malloc */
#include <stdlib.h>
/* fread feof fclose */
#include <stdio.h>
/* memset */
#include <string.h>


/**
 * tallyFrequencies - reads input file to tally amount of appearances of each
 *   byte value from 0 to CHAR_RANGE; in the resulting array, the value at each
 *   index represents the amount of appearances of the byte value equal to that
 *   index
 *
 * @in_file: input file stream
 * @in_file_size: input file size in bytes
 * Return: array of frequencies, or NULL on failure
 */
size_t *tallyFrequencies(FILE *in_file, size_t in_file_size)
{
	size_t *freqs = NULL;
	size_t i, read_bytes, remainder;
	unsigned char r_buff[BUF_SIZE] = {0};

	if (!in_file)
		return (NULL);

	freqs = malloc(sizeof(size_t) * CHAR_RANGE);
	if (!freqs)
		return (NULL);
	memset(freqs, 0, sizeof(size_t) * CHAR_RANGE);

	remainder = in_file_size % BUF_SIZE;
	do {
		read_bytes = fread(r_buff, sizeof(unsigned char),
				   BUF_SIZE, in_file);

		if (!(read_bytes == BUF_SIZE || read_bytes == remainder))
		{
			free(freqs);
			return (NULL);
		}

		for (i = 0; i < read_bytes; i++)
			freqs[r_buff[i]]++;

	} while (read_bytes == BUF_SIZE);

	if (!feof(in_file))
	{
		free(freqs);
		return (NULL);
	}

	rewind(in_file);

	return (freqs);
}


/**
 * prepareTreeInputs - translates array returned by tallyFrequencies into the
 *   two arrays expected by huffman_tree: one of byte values, one of byte
 *   frequencies
 *
 * @freqs: size_t array with the value at each index representing the frequency
 *   in the imput file of the byte value equal to that index
 * @data: char array modified by reference; byte values appearing in the input
 *   file
 * @freq: size_t array modified by reference; byte value frequencies in the
 *   input file, corresponding by index to `data`
 * @freq_size: pointer to size_t modified by reference; count of unique byte
 *   values appearing in the input file, and thus the size of `data`/`freq`
 * Return: 0 on success, 1 on failure
 */
int prepareTreeInputs(size_t *freqs, char **data,
		      size_t **freq, size_t *freq_size)
{
	size_t i, j;

	*freq_size = 0;
	for (i = 0; i < CHAR_RANGE; i++)
		if (freqs[i])
			(*freq_size)++;

	*data = malloc(sizeof(char) * *freq_size);
	if (!(*data))
		return (1);

	*freq = malloc(sizeof(size_t) * *freq_size);
	if (!(*freq))
	{
		free((*data));
		return (1);
	}

	for (i = 0, j = 0; i < CHAR_RANGE && j < *freq_size; i++)
	{
		if (freqs[i])
		{
			(*freq)[j] = freqs[i];
			(*data)[j] = (char)i;
			j++;
		}
	}

	return (0);
}


/**
 * huffmanTreeFromText - tallies frequencies of byte values appearing in the
 *   input file, and creates a Huffman Tree data structure, or a min binary
 *   heap sorted by frequency
 *
 * @in_file: input file stream
 * @freq_size: pointer to size_t to contain count of unqiue byte values
 *   appearing in input file, and thus amount of frequency values
 * @in_file_size: input file size in bytes
 * Return: head of a binary tree structured as a Huffman Tree, or NULL on
 *   failure
 */
binary_tree_node_t *huffmanTreeFromText(FILE *in_file, size_t *freq_size,
					size_t in_file_size)
{
	size_t *freqs = NULL, *freq = NULL;
	char *data = NULL;
	binary_tree_node_t *h_tree = NULL;

	if (!in_file)
		return (NULL);

	freqs = tallyFrequencies(in_file, in_file_size);
	if (!freqs)
		return (NULL);

	if (prepareTreeInputs(freqs, &data, &freq, freq_size) == 1)
	{
		free(freqs);
		return (NULL);
	}
	free(freqs);

	h_tree = huffman_tree(data, freq, *freq_size);

	free(data);
	free(freq);

	return (h_tree);
}



/*
 * Note: max possible leaves in tree 256 (frequecies for 00-ff byte values,) so
 * serialized tree not likely more than roughly 2*256 bytes in output file
 */
/**
 * huffmanCompress - compresses input file text via Huffman Coding, and writes
 *   to an output file a custom file header, the serialized Huffman Tree, and
 *   the encoded content
 *
 * @in_file: input file stream
 * @out_file: output file stream
 * @in_file_size: input file size in bytes
 * Return: 0 on success, 1 on failure
 */
int huffmanCompress(FILE *in_file, FILE *out_file, long int in_file_size)
{
	unsigned char w_buff[BUF_SIZE] = {0};
	bit_t w_bit = {0, 0, 0};
	huffman_header_t header = {{'\177', 'H', 'U', 'F'}, 0, 0, 0};
	binary_tree_node_t *h_tree = NULL;
	size_t freq_size = 0;

	if (!in_file || !out_file || in_file_size < 0)
		return (1);

	h_tree = huffmanTreeFromText(in_file, &freq_size,
				     (size_t)in_file_size);
	if (!h_tree)
		return (1);
	huffmanSerialize(h_tree, w_buff, &w_bit);
	if (writePartialByte(w_buff, &w_bit) == 1)
	{
		binaryTreeDelete(h_tree, freeSymbol);
		return (1);
	}

	/* huffman code starts at bit after serialized tree */
	header.hc_byte_offset = sizeof(huffman_header_t) + w_bit.byte_idx;
	header.hc_first_bit_i = w_bit.bit_idx;
	if (huffmanEncode(in_file, h_tree, freq_size, w_buff, &w_bit) == 1 ||
	    writePartialByte(w_buff, &w_bit) == 1)
	{
		binaryTreeDelete(h_tree, freeSymbol);
		return (1);
	}
	header.hc_last_bit_i = w_bit.bit_idx;
	binaryTreeDelete(h_tree, freeSymbol);

	/* write header to file */
	if (fwrite(&header, sizeof(huffman_header_t), 1, out_file) != 1)
		return (1);
	/* write serialized tree and encoded content to file */
	if (fwrite(w_buff, sizeof(unsigned char), w_bit.byte_idx + 1,
		   out_file) != w_bit.byte_idx + 1)
		return (1);
	return (0);
}
