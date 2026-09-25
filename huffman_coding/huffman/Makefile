# -*- MakeFile -*-
# 0x02. C - Huffman coding - project 286


CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -I../heap/ -I../


# task 10. Huffman - Compression & Decompression

heap_create.o: ../heap/heap.h ../heap/heap_create.c
	$(CC) -c $(CFLAGS) ../heap/heap_create.c

binary_tree_node.o: ../heap/heap.h ../heap/binary_tree_node.c
	$(CC) -c $(CFLAGS) ../heap/binary_tree_node.c

heap_insert.o: ../heap/heap.h ../heap/heap_insert.c
	$(CC) -c $(CFLAGS) ../heap/heap_insert.c

heap_extract.o: ../heap/heap.h ../heap/heap_extract.c
	$(CC) -c $(CFLAGS) ../heap/heap_extract.c

heap_delete.o: ../heap/heap.h ../heap/heap_delete.c
	$(CC) -c $(CFLAGS) ../heap/heap_delete.c

symbol.o: ../huffman.h ../symbol.c
	$(CC) -c $(CFLAGS) ../symbol.c

huffman_priority_queue.o: ../huffman.h ../huffman_priority_queue.c
	$(CC) -c $(CFLAGS) ../huffman_priority_queue.c

huffman_extract_and_insert.o: ../huffman.h ../huffman_extract_and_insert.c
	$(CC) -c $(CFLAGS) ../huffman_extract_and_insert.c

huffman_tree.o: ../huffman.h ../huffman_tree.c
	$(CC) -c $(CFLAGS) ../huffman_tree.c

huffman.o: ../heap/heap.h _huffman.h huffman.c
	$(CC) -c $(CFLAGS) huffman.c

huffmanCompress.o: ../heap/heap.h _huffman.h huffmanCompress.c
	$(CC) -c $(CFLAGS) huffmanCompress.c

huffmanDecompress.o: ../heap/heap.h _huffman.h huffmanDecompress.c
	$(CC) -c $(CFLAGS) huffmanDecompress.c

huffmanEncode.o: ../heap/heap.h _huffman.h huffmanEncode.c
	$(CC) -c $(CFLAGS) huffmanEncode.c

huffmanDecode.o: ../heap/heap.h _huffman.h huffmanDecode.c
	$(CC) -c $(CFLAGS) huffmanDecode.c

serialization.o: ../heap/heap.h _huffman.h serialization.c
	$(CC) -c $(CFLAGS) serialization.c

read_write.o: ../heap/heap.h _huffman.h read_write.c
	$(CC) -c $(CFLAGS) read_write.c

# only use during testing
_binary_tree_print.o: ../_binary_tree_print.c
	$(CC) -c $(CFLAGS) ../_binary_tree_print.c

all: ../heap/heap.h ../huffman.h _huffman.h \
	heap_create.o \
	binary_tree_node.o \
	heap_insert.o \
	heap_extract.o \
	heap_delete.o \
	symbol.o \
	huffman_priority_queue.o \
	huffman_extract_and_insert.o \
	huffman_tree.o \
	huffman.o \
	huffmanCompress.o \
	huffmanDecompress.o \
	huffmanEncode.o \
	huffmanDecode.o \
	serialization.o \
	_binary_tree_print.o \
	read_write.o
	$(CC) $(CFLAGS) ../heap/heap.h ../huffman.h _huffman.h \
	heap_create.o \
	binary_tree_node.o \
	heap_insert.o \
	heap_extract.o \
	heap_delete.o \
	symbol.o \
	huffman_priority_queue.o \
	huffman_extract_and_insert.o \
	huffman_tree.o \
	huffman.o \
	huffmanCompress.o \
	huffmanDecompress.o \
	huffmanEncode.o \
	huffmanDecode.o	\
	serialization.o \
	_binary_tree_print.o \
	read_write.o \
	-o huffman


.PHONY: clean oclean fclean re

clean:
	$(RM) *~ huffman

oclean:
	$(RM) *.o

fclean: clean oclean

re: oclean huffman
