#include "huffman.h"
/* malloc free */
#include <stdlib.h>


/**
 * symbol_create - creates a symbol_t data structure
 *
 * @data: character appearing in text
 * @freq: amount of appearances of this character appears in text
 * Return: newly allocated symbol_t struct, or NULL on failure
 */
symbol_t *symbol_create(char data, size_t freq)
{
	symbol_t *new = NULL;

	new = malloc(sizeof(symbol_t));
	if (!new)
		return (NULL);

	new->data = data;
	new->freq = freq;

	return (new);
}
