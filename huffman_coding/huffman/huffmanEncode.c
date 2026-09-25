#include "_huffman.h"
/* binary_tree_node_t */
#include "heap.h"
/* symbol_t */
#include "huffman.h"
/* malloc calloc free */
#include <stdlib.h>
/* strlen strcpy */
#include <string.h>


/**
 * freeCodes - frees memory allocated for Huffman code array
 *
 * @codes: array for Huffman codes; index 0 contains symbol, remainder of
 *   string is code
 * @freq_size: count of unqiue byte values appearing in input file, and thus
 *   amount of frequency values
 */
void freeCodes(char **codes, size_t freq_size)
{
	size_t i;

	if (codes == NULL)
		return;

	for (i = 0; i < freq_size; i++)
		if (codes[i])
			free(codes[i]);
	free(codes);
}


/**
 * encodeText - uses array of Huffman codes to transcode bytes from the read
 *   buffer to bits in the write buffer
 *
 * @codes: array for Huffman codes; index 0 contains symbol, remainder of
 *   string is code
 * @freq_size: count of unqiue byte values appearing in input file, and thus
 *   amount of frequency values
 * @r_buff: read buffer
 * @read_size: amount of bytes in read buffer to process
 * @w_buff: write buffer
 * @w_bit: struct containing indices of current byte and bit in write buffer,
 *    for bit-granular writing
 * Return: 0 on success, 1 on failure
 */
int encodeText(char **codes, size_t freq_size, unsigned char *r_buff,
	       size_t read_size, unsigned char *w_buff, bit_t *w_bit)
{
	size_t i, j, k;
	char *code = NULL;
	int code_found;

	if (!codes || !r_buff || !w_buff || !w_bit)
		return (1);

	for (i = 0; i < read_size; i++)
	{
		code_found = 0;
		for (j = 0; j < freq_size; j++)
		{
			if (r_buff[i] == codes[j][0])
			{
				code_found = 1;
				code = codes[j] + 1;

				for (k = 0; code[k]; k++)
				{
					if (code[k] == '0')
						writeBit(w_buff, w_bit, 0);
					else
						writeBit(w_buff, w_bit, 1);
				}

				break;
			}
		}

		if (!code_found)
			return (1);
	}

	return (0);
}


/**
 * buildHuffmanCodes - recursively traverses a Huffman Tree to derive the
 *   Huffman codes for symbols at each leaf
 *
 * @h_tree: head of Huffman tree (min binary heap of byte value frequencies)
 * @depth: current level of Huffman tree, and by implication recursion frame
 * @i: modified by reference, current index in `codes`
 * @code: array populated during pre-order recursion with 0 for branching left
 *   and 1 for branching right; the state of this string when the recursion
 *   reaches a leaf determines the Huffman code for that leaf's symbol; first
 *   position in array contains symbol
 * @codes: array of finished Huffman codes
 */
void buildHuffmanCodes(binary_tree_node_t *h_tree, size_t depth,
		       size_t *i, char *code, char **codes)
{
	symbol_t *symbol = NULL;
	char *char_code = NULL;

	if (!h_tree || i == NULL || code == NULL || codes == NULL)
		return;

	if (depth)
		code[depth - 1] = (h_tree == h_tree->parent->left) ? '0' : '1';

	if (!h_tree->left && !h_tree->right)
	{
		symbol = (symbol_t *)(h_tree->data);

		char_code = malloc(sizeof(char) * (depth + 2));
		if (!char_code)
		{
			printf("buildHuffmanCodes: failed to assign code\n");
			return;
		}

		char_code[0] = symbol->data;
		strncpy(char_code + 1, code, depth);
		char_code[depth + 1] = '\0';
		codes[*i] = char_code;
		(*i)++;
		return;
	}

	buildHuffmanCodes(h_tree->left, depth + 1, i, code, codes);
	buildHuffmanCodes(h_tree->right, depth + 1, i, code, codes);
}


/**
 * huffmanEncode - derives Huffman codes from a Huffman tree and uses them to
 *   write the encoded data to a write buffer
 *
 * @in_file: input file stream
 * @h_tree: head of Huffman tree (min binary heap of byte value frequencies)
 * @freq_size: count of unqiue byte values appearing in input file, and thus
 *   amount of frequency values
 * @w_buff: write buffer
 * @w_bit: struct containing indices of current byte and bit in write buffer,
 *     for bit-granular writing
 * Return: 0 on success, 1 on failure
 */
int huffmanEncode(FILE *in_file, binary_tree_node_t *h_tree, size_t freq_size,
		  unsigned char *w_buff, bit_t *w_bit)
{
	char **codes = NULL;
	char *code = NULL;
	size_t i = 0, read_bytes;
	unsigned char r_buff[BUF_SIZE] = {0};

	if (!in_file || !h_tree || !w_buff || !w_bit)
		return (1);

	code = calloc(freq_size, sizeof(char));
	if (code == NULL)
		return (1);
	codes = calloc(freq_size + 1, sizeof(char *));
	if (codes == NULL)
	{
		free(code);
		return (1);
	}

	buildHuffmanCodes(h_tree, 0, &i, code, codes);
	free(code);
	if (i != freq_size)
	{
		freeCodes(codes, freq_size);
		return (1);
	}

	/* currently only supports files up to BUF_SIZE bytes */
	read_bytes = fread(r_buff, sizeof(unsigned char),
			   BUF_SIZE, in_file);

	if (encodeText(codes, freq_size, r_buff, read_bytes,
		       w_buff, w_bit) == 1)
	{
		freeCodes(codes, freq_size);
		return (1);
	}

	freeCodes(codes, freq_size);
	return (0);
}
