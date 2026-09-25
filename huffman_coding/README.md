# Huffman Coding Module

This folder contains a C implementation of Huffman coding, built on top of a
min binary heap used as a priority queue.

## Data Model

The public header [huffman.h](huffman.h) defines:

- `symbol_t`: pairs a character (`data`) with its frequency (`freq`).

It builds on the heap module's [heap.h](heap/heap.h), which defines:

- `heap_t`: a min binary heap, tracking its `size`, a `data_cmp` comparison
  function, and a pointer to the `root` node.
- `binary_tree_node_t`: a generic binary tree node used both as a heap node
  and as a node of the resulting Huffman tree.

## Available Functions

### Heap ([heap/](heap/))

- [heap_create](heap/heap_create.c): allocate and initialize an empty heap
  given a comparison function.
- [binary_tree_node](heap/binary_tree_node.c): allocate a single binary tree
  node with a given parent and data.
- [heap_insert](heap/heap_insert.c): insert data into the heap, keeping it
  complete and heap-ordered.
- [heap_extract](heap/heap_extract.c): remove and return the root data,
  re-balancing the heap afterward.
- [heap_delete](heap/heap_delete.c): free the entire heap and its nodes.

### Symbols and Huffman tree

- [symbol_create](symbol.c): allocate a `symbol_t` from a character and its
  frequency.
- [huffman_priority_queue](huffman_priority_queue.c): build a min heap of
  symbols, ordered by ascending frequency.
- [huffman_extract_and_insert](huffman_extract_and_insert.c): pop the two
  lowest-frequency nodes from the priority queue, merge them into a new
  parent node holding their summed frequency, and push it back.
- [huffman_tree](huffman_tree.c): repeatedly apply the extract-and-insert
  step until a single root node remains, producing the Huffman tree.
- [huffman_codes](huffman_codes.c): build the Huffman tree for a set of
  symbols and print the binary code for each one.

## Notes

- Symbol frequencies must be strictly positive; `data` and `freq` arrays are
  parallel, matched by index.
- In the Huffman tree, a left child contributes a `0` bit and a right child
  contributes a `1` bit to the code of the symbols beneath it.
- Nodes created while merging two symbol nodes store a `symbol_t` with a
  placeholder character and the summed frequency of both children.
- `heap_extract` and `heap_delete` accept a `free_data` callback so heap
  users control how stored data is released.

## Build

There is no project-wide build system in the repository yet. Compile the
files you need together with your own test program, for example:

```bash
gcc -Wall -Wextra -Werror -pedantic -std=c89 -Ihuffman_coding -Ihuffman_coding/heap \
    huffman_coding/heap/*.c huffman_coding/*.c test_main.c -o test_huffman
```

Adjust the command to match the files you want to exercise.
