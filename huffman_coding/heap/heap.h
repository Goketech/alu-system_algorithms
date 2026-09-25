#ifndef _HEAP_H_
#define _HEAP_H_

/* size_t */
#include <stddef.h>


/**
 * struct binary_tree_node_s - Binary tree node data structure
 *
 * @data: Data stored in a node
 * @left: Pointer to the left child
 * @right: Pointer to the right child
 * @parent: Pointer to the parent node
 */
typedef struct binary_tree_node_s
{
	void *data;
	struct binary_tree_node_s *left;
	struct binary_tree_node_s *right;
	struct binary_tree_node_s *parent;
} binary_tree_node_t;


/**
 * struct heap_s - Heap data structure
 *
 * @size: Size of the heap (number of nodes)
 * @data_cmp: Function to compare two nodes data
 * @root: Pointer to the root node of the heap
 */
typedef struct heap_s
{
	size_t size;
	int (*data_cmp)(void *, void *);
	binary_tree_node_t *root;
} heap_t;


/* task 2. Min Binary Heap - Insert node */
/**
 * struct binary_tree_node_queue_s - node in singly linked list used as a
 *   queue to traverse a binary tree in level order
 *
 * @node: pointer to node in a binary tree
 * @next: pointer to next member of queue
 */
typedef struct binary_tree_node_queue_s
{
	binary_tree_node_t *node;
	struct binary_tree_node_queue_s *next;
} bt_node_queue_t;


/* task 0. Min Binary Heap - Create heap */
/* task 2. Min Binary Heap - Insert node */
/* heap_create.c */
heap_t *heap_create(int (*data_cmp)(void *, void *));

/* task 1. Min Binary Heap - Create node */
/* task 2. Min Binary Heap - Insert node */
/* binary_tree_node.c */
binary_tree_node_t *binary_tree_node(binary_tree_node_t *parent, void *data);

/* task 2. Min Binary Heap - Insert node */
/* task 3. Min Binary Heap - Extract */
/* heap_insert.c */
bt_node_queue_t *BTQueuePush(bt_node_queue_t **queue,
			     binary_tree_node_t *node);
void BTQueueAdvance(bt_node_queue_t **queue);
void BTQueueClear(bt_node_queue_t **queue);
/* task 2. Min Binary Heap - Insert node */
/* heap_insert.c */
binary_tree_node_t *BTCompleteInsert(binary_tree_node_t *root,
				     void *data);
binary_tree_node_t *minHeapSiftUp(binary_tree_node_t *new,
				  int (data_cmp)(void *, void *));
binary_tree_node_t *heapInsert(binary_tree_node_t **root,
			       int (data_cmp)(void *, void *), void *data);
binary_tree_node_t *heap_insert(heap_t *heap, void *data);

/* task 3. Min Binary Heap - Extract */
/* heap_extract.c */
size_t binaryTreeSize(const binary_tree_node_t *tree);
int binaryTreeIsComplete(const binary_tree_node_t *tree,
			  size_t i, size_t size);
binary_tree_node_t *lastInLevelOrder(binary_tree_node_t *root);
void minHeapSiftDown(binary_tree_node_t *root,
		     int (data_cmp)(void *, void *));
void *heapExtract(binary_tree_node_t **root,
		  int (data_cmp)(void *, void *));
void *heap_extract(heap_t *heap);

/* task 4. Min Binary Heap - Delete heap */
/* heap_delete.c */
void binaryTreeDelete(binary_tree_node_t *tree, void (*free_data)(void *));
void heap_delete(heap_t *heap, void (*free_data)(void *));


#endif /* _HEAP_H_ */
