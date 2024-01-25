#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h>

#define NODES_ARRAY_SIZE 1000
#define BIG_CHUNK 1024 * 1024

enum node_type
{
    not_used,        // Represents that the node has no information and hasnt been used yet
    allocated_block, // Represents if the node contains information about an allocated memory block
    free_block       // Represents if the node contains information about a freed memory block
};

struct Node
{
    size_t block_size;
    void *block_start_addr;
    void *block_end_addr; //Not to be used anywhere in the code, like no read operation to be done and utilization of this field shall happend in the .c file

    enum node_type node_state;
    int next_free_node_index; //Can store index of two types of nodes, the node of type node_used or the node of type free_block.
    int previous_node_index;
    int heap_node_index;
};

struct HeapNode{
  size_t size;
  int node_index;
};

void heapify(int i);
int heap_insert(size_t size, int node_index);
void heap_delete(int i);
int get_largest_size_node_index(int size);

int mem_init(); // Intilization function
void mem_free();

void *my_malloc(size_t size); // Our custom allocate function

void my_free(void *ptr); // Our custom free function


void printHeap();
#endif