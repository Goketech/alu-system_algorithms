#include "_huffman.h"



/**
 * readBit - read a single 0 or 1 bit from the current byte in the read buffer
 *
 * @r_buff: read buffer
 * @r_bit: struct containing indices of current byte and bit in read buffer,
 *     for bit-granular reading
 * @value: modified by reference; 0 or 1 value read from bit
 * Return: 0 on success, 1 on failure
 */
int readBit(unsigned char *r_buff, bit_t *r_bit, unsigned char *value)
{
	if (!r_buff || !r_bit || !value)
		return (1);

	r_bit->byte = r_buff[r_bit->byte_idx];

	if (r_bit->byte & (1 << (7 - r_bit->bit_idx)))
		*value = 1;
	else
		*value = 0;

	r_bit->bit_idx++;
	if (r_bit->bit_idx == 8)
	{
		r_bit->byte_idx++;
		r_bit->bit_idx = 0;
	}

	return (0);
}


/**
 * readByte - reads all 8 bits in r_bit byte from the read buffer in one
 *   operation, setting value to `byte`
 *
 * @r_buff: read buffer
 * @r_bit: struct containing indices of current byte and bit in read buffer,
 *     for bit-granular reading
 * @byte: modified by reference to contain byte read
 * Return: 0 on success, 1 on failure
 */
int readByte(unsigned char *r_buff, bit_t *r_bit, unsigned char *byte)
{
	size_t i;
	unsigned char bit_value;

	if (!r_buff || !r_bit || !byte)
		return (1);

	for (i = 0; i < 8; i++)
	{
		if (readBit(r_buff, r_bit, &bit_value) == 1)
			return (1);

		if (bit_value)
			*byte |= (1 << (7 - i));
		else
			*byte &= ~(1 << (7 - i));
	}

	return (0);
}


/**
 * writeBit - write a single 0 or 1 bit to the current byte in the write buffer
 *
 * @w_buff: write buffer
 * @w_bit: struct containing indices of current byte and bit in write buffer,
 *     for bit-granular writing
 * @toggle: 0 or 1 value to write to bit
 * Return: 0 on success, 1 on failure
 */
int writeBit(unsigned char *w_buff, bit_t *w_bit, unsigned char toggle)
{
	if (!w_buff || !w_bit || (toggle != 0 && toggle != 1))
		return (1);

	if (toggle)
		w_bit->byte |= (1 << (7 - w_bit->bit_idx));
	else
		w_bit->byte &= ~(1 << (7 - w_bit->bit_idx));
	w_bit->bit_idx++;

	if (w_bit->bit_idx == 8)
	{
		w_buff[w_bit->byte_idx] = w_bit->byte;
		w_bit->byte_idx++;
		w_bit->bit_idx = 0;
	}

	return (0);
}


/**
 * writeByte - writes all 8 bits from `byte` individually to the write buffer
 *   in one operation
 *
 * @w_buff: write buffer
 * @w_bit: struct containing indices of current byte and bit in write buffer,
 *     for bit-granular writing
 * @byte: value to write bit by bit
 * Return: 0 on success, 1 on failure
 */
int writeByte(unsigned char *w_buff, bit_t *w_bit, unsigned char byte)
{
	size_t i;

	if (w_buff == NULL || !w_bit)
		return (1);

	for (i = 0; i < 8; i++)
	{
		if (writeBit(w_buff, w_bit,
			     (byte & (1 << (7 - i))) ? 1 : 0) == 1)
			return (1);
	}

	return (0);
}


/**
 * writePartialByte - a "flush" of the current byte in the w_bit struct to the
 *   corresponding byte in the write buffer, for those cases when the main
 *   write operation is finished and there are less than 8 bits left to write
 *   to the buffer; remaining bits set to 0
 *
 * @w_buff: write buffer
 * @w_bit: struct containing indices of current byte and bit in write buffer,
 *     for bit-granular writing
 * Return: 0 on success, 1 on failure
 */
int writePartialByte(unsigned char *w_buff, bit_t *w_bit)
{
	unsigned int orig_bit_i;
	size_t i;

	if (w_buff == NULL || !w_bit)
		return (1);

	if (w_bit->bit_idx != 0)
	{
		/* zero out any bits in w_bit->byte beyond current bit_idx */
		for (i = w_bit->bit_idx; i < 8; i++)
			w_bit->byte &= ~(1 << (7 - i));

		orig_bit_i = w_bit->bit_idx;
		w_bit->bit_idx = 0;

		/* write whole byte, zero-padded by unassigned bits to left */
		writeByte(w_buff, w_bit, w_bit->byte);

		/* reset to original values */
		w_bit->bit_idx = orig_bit_i;
		w_bit->byte_idx -= 1;
	}

	return (0);
}
