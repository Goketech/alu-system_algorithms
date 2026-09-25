#ifndef _HUFFMAN_H
#define _HUFFMAN_H

/* binary_tree_node_t */
#include "heap.h"
/* FILE */
#include <stdio.h>
/* struct stat */
/* stat */
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 4096
/* 128 for ASCII text; 256 allows for extended ASCII or any byte sequence */
#define CHAR_RANGE 256


/**
 * struct bit_s - struct to keep track of index positions for bit-granular
 *   read/write operations
 *
 * @byte_idx: index of local byte in file/buffer
 * @byte: contents of local byte
 * @bit_idx: index of bit in byte, from left/MSB
 */
typedef struct bit_s
{
	size_t byte_idx;
	unsigned char byte;
	unsigned int bit_idx;
} bit_t;

/*
 * pads out to 12 bytes if not `__attribute__((packed))`
 * offset byte can be short int: serialized tree not that big for non-Unicode
 *
 *typedef struct huffman_header_s
 *{
 *	unsigned char huff_id[4];
 *	unsigned int hc_byte_offset;
 *	unsigned char hc_first_bit_i;
 *	unsigned char hc_last_bit_i;
 *} __attribute__((packed)) huffman_header_t;
 */

/**
 * struct huffman_header_s - contains header information encoded into start of
 *   compressed file
 *
 * @huff_id: "\177HUF": similar to the ELF magic number, used as file type
 *   identifier
 * @hc_byte_offset: compressed file byte index of start of encoded content;
 *   header and serialized Huffman Tree stored in indices lower than this
 * @hc_first_bit_i: bit index in byte at hc_byte_offset of start of encoded
 *   content; header and serialized Huffman Tree stored in indices lower than
 *   this
 * @hc_last_bit_i: last bit index in final byte of file that contains encoded
 *   content; prevents misinterpretation of trailing 0 bits
 */
typedef struct huffman_header_s
{
	unsigned char huff_id[4];
	unsigned short int hc_byte_offset;
	unsigned char hc_first_bit_i;
	unsigned char hc_last_bit_i;
} huffman_header_t;


/* huffman.c */
FILE *openInputFile(char *input_path, struct stat *st);
FILE *openOutputFile(char *output_path);
/* int main(int argc, char *argv[]) */

/* huffmanCompress.c */
size_t *tallyFrequencies(FILE *in_file, size_t in_file_size);
int prepareTreeInputs(size_t *freqs, char **data,
		      size_t **freq, size_t *freq_size);
binary_tree_node_t *huffmanTreeFromText(FILE *in_file, size_t *freq_size,
					size_t in_file_size);
int huffmanCompress(FILE *in_file, FILE *out_file, long int in_file_size);

/* huffmanDecompress.c */
void freeChar(void *data);
int huffmanDecompress(FILE *in_file, FILE *out_file, long int input_file_size);

/* huffmanEncode.c */
void freeCodes(char **codes, size_t freq_size);
int encodeText(char **codes, size_t freq_size, unsigned char *r_buff,
	       size_t read_size, unsigned char *w_buff, bit_t *w_bit);
void buildHuffmanCodes(binary_tree_node_t *h_tree, size_t depth,
		       size_t *i, char *code, char **codes);
int huffmanEncode(FILE *in_file, binary_tree_node_t *h_tree, size_t freq_size,
		  unsigned char *w_buff, bit_t *w_bit);

/* huffmanDecode.c */
char *decodeSingleChar(binary_tree_node_t *h_tree,
		       unsigned char *r_buff, bit_t *r_bit);
int huffmanDecode(FILE *out_file, huffman_header_t *header,
		  binary_tree_node_t *h_tree, size_t in_file_size,
		  unsigned char *r_buff, bit_t *r_bit);

/* serialization.c */
void huffmanSerialize(binary_tree_node_t *huffman_tree,
		      unsigned char *buff, bit_t *w_bit);
binary_tree_node_t *huffmanDeserialize(unsigned char *buff, bit_t *r_bit,
				       binary_tree_node_t *parent);

/* read_write.c */
int readBit(unsigned char *buff, bit_t *r_bit, unsigned char *value);
int readByte(unsigned char *buff, bit_t *r_bit, unsigned char *byte);
int writeBit(unsigned char *buff, bit_t *w_bit, unsigned char toggle);
int writeByte(unsigned char *buff, bit_t *w_bit, unsigned char byte);
int writePartialByte(unsigned char *buff, bit_t *w_bit);


#endif /* _HUFFMAN_H */
