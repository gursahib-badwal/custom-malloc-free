#include "my_malloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>


static int next_free_node = 0;
static void* next_free_mem_addr; //This will be a pointer that will keep track of the next avaiable memory address
// which will be set as the block start address of the new node created as every new node keep tracks of the new memory 
// block and new memory block has to start right next to its previous memory block
static void* main_block_addr = NULL;
static void* last_mem_addr;
// Note 1 - In mem_init we will set this to the pointer address returned by the one and only malloc call as that will be the 
// starting point of our whole memory addresses stuff.

// Note 2 - Once next_free_mem_addr has been initialized by the mem init, everytime the function my_malloc will be run, we
// will update the next_free_mem_addr to the imaginary lower bound of the current block by doing (char*)current_node->block_start_Addr + current_node->block_size

// Note 3 - Everytime a new node is created its starting block address field has to be set to something that shoud be 
// right after the previous blocks lower bound, and here comes the next_free_mem_addr in action as the computation that 
// we did above Note-2 automatically returns the next free memory address from where the next memory block can 
// allocate/start from

// Note 4 - Remember we had the doubt that when free function is called then free takes in a pointer, while we 
// we have a array of nodes, how will we know which memory block does the free function wants to free, the answer to that 
// is that we will compare each node's block_start_addr because whenver the free function gives an pointer is always the pointer
// to the address from where the respective memory block starts, so right now (not considering optimization) we can simply
// traverse through the whole array of nodes and check which node's block_start_addr fiels is equal to the parameter of 
// the free function and then that node can be free with whatever implementation required.

struct Node nodes_array[NODES_ARRAY_SIZE];

struct HeapNode heap_nodes[NODES_ARRAY_SIZE];
static int heap_size = 0;

int mem_init(){

    main_block_addr = malloc(BIG_CHUNK);
    if (main_block_addr == NULL){
        return -1;
    }
    next_free_mem_addr = main_block_addr;

    last_mem_addr = (char*) main_block_addr + BIG_CHUNK; //points to right after the end of the allocated main block

    for (int i=0; i < NODES_ARRAY_SIZE - 1 ; i++) {
        nodes_array[i].node_state = not_used;
        nodes_array[i].next_free_node_index = i + 1;
        nodes_array[i].heap_node_index = -1;
        heap_nodes[i].size = 0;
        heap_nodes[i].node_index = -1; 
    }
    nodes_array[NODES_ARRAY_SIZE - 1].node_state = not_used; //for last free node
    nodes_array[NODES_ARRAY_SIZE - 1].next_free_node_index = -1;
    nodes_array[NODES_ARRAY_SIZE - 1].heap_node_index = -1;

    for (int j=1; j < NODES_ARRAY_SIZE; j++){
        nodes_array[j].previous_node_index = j - 1;
    }
    
    nodes_array[0].block_start_addr = main_block_addr; //This confirms that the very first memory block that will be allocated will have the starting address same as the starting address of the actual big allocated chunk, which acting as the memory to us
    return 0;
}

void mem_free(){
    free(main_block_addr);
    main_block_addr = NULL;
    next_free_mem_addr = NULL;
    last_mem_addr = NULL;
    heap_size = 0;
    next_free_node = 0;
}

void *my_malloc(size_t size){

    size_t mem_bytes_left = (size_t)((char*)last_mem_addr - (char*)next_free_mem_addr);
    int least_largest_free_node_index = get_largest_size_node_index(size);


    if (heap_nodes[0].node_index != -1 && least_largest_free_node_index != -1){ //This condition checks if there is even a single free block available as heap_nodes array takes care of all the free blocks
        nodes_array[least_largest_free_node_index].node_state = allocated_block;
       // heap_delete(nodes_array[least_largest_free_node_index].heap_node_index);
        char* temp3 = nodes_array[least_largest_free_node_index].block_start_addr;
        return (void*) ((temp3 + 2));
    }

    else if (size <= mem_bytes_left){ //checks if there is space to allocate a new block

        //Considering the most basic allocation case when there are all empty nodes and nothing has ever been freed 
        //First step is to make a new memory block 
        nodes_array[next_free_node].block_start_addr = next_free_mem_addr; //set to right after the lower bound of the previous memory block
        nodes_array[next_free_node].block_size = size + 2;
        nodes_array[next_free_node].block_end_addr = (char*)nodes_array[next_free_node].block_start_addr + size + 2;
        nodes_array[next_free_node].node_state =  allocated_block;

        uint16_t* node_index_ptr = (uint16_t*) nodes_array[next_free_node].block_start_addr;
        *node_index_ptr = next_free_node; 

        char* temp = nodes_array[next_free_node].block_start_addr;
        int temp2 = next_free_node;

        next_free_mem_addr = nodes_array[next_free_node].block_end_addr;  //Updating the fence so that the memory that has already been used to allocate a memory block can not be used by any future my_mallocs so we keep on updating the fence and thats why the start block adddress of every new memory block is set excatly to the fence.
        next_free_node = nodes_array[next_free_node].next_free_node_index; //updating the next_free_node static variable
        nodes_array[next_free_node].previous_node_index = temp2; //this step is important as previous node index cant simply be detected by doing -1 on the current next_free_node

        return (void*)((temp + 2)); //Imagine data is like this 1,2,3,4.....50 this means a 50 byte memory block is allocated, then we want to give user the pointer to the memory block starting from the 3rd byte because the first two bytes will store the corresponding node index
    }

    else {
        return NULL;
    }
}

void my_free(void *pointer){
    if (pointer == NULL)
    return;

    short retrieved_node_index = -1;

    // offset the pointer by 2 bytes less
    pointer = ((char *)pointer) - 2;
    if (pointer == NULL){
        return;
    }
    //getting the index stored in the first two bytes
    memcpy(&retrieved_node_index, pointer, 2);
    //checking if index obtained is valid or not
    if(retrieved_node_index < 0 || retrieved_node_index > NODES_ARRAY_SIZE - 1 || nodes_array[retrieved_node_index].node_state != allocated_block){
        return;
    } 
    //if the index is valid
    nodes_array[retrieved_node_index].node_state = free_block;
    //inserting the empty node_block in the heap
    // heap_insert(nodes_array[retrieved_node_index].block_size,retrieved_node_index);
    // ----------------------------freeing a block finishes here------------------------------------------->
    //-----------------------------Merging adjacent free blocks-------------------------------------------->
    //Check if the next adjacent memory block is a free block too 
    struct Node next_node_temp = nodes_array[nodes_array[retrieved_node_index].next_free_node_index];
    struct Node prev_node_temp = nodes_array[nodes_array[retrieved_node_index].previous_node_index];
    // int temp_next_next = next_node_temp.next_free_node_index;

    if (next_node_temp.node_state == free_block){
        nodes_array[retrieved_node_index].block_size += next_node_temp.block_size;
        nodes_array[retrieved_node_index].block_end_addr = next_node_temp.block_end_addr;


        // nodes_array[retrieved_node_index].next_free_node_index = temp_next_next;
        // nodes_array[temp_next_next].previous_node_index = retrieved_node_index;

        heap_delete(next_node_temp.heap_node_index); //removed the heap node corresponding to the next node bcoz now the next node is not required and has to be recycled in the next few lines 

        // heap_update(nodes_array[retrieved_node_index].heap_node_index, nodes_array[retrieved_node_index].block_size);
        //now recycling the next node 

        int retrieved_index1 = nodes_array[retrieved_node_index].heap_node_index;
        int temp6 = heap_insert(nodes_array[retrieved_node_index].block_size ,retrieved_node_index );
        nodes_array[retrieved_node_index].heap_node_index = temp6;

        //heap_delete(retrieved_index1);

        next_node_temp.block_size = 0;
        next_node_temp.block_start_addr = NULL;
        next_node_temp.block_end_addr = NULL;
        next_node_temp.node_state = not_used;

        int temp3 = nodes_array[next_free_node].previous_node_index;
        int temp = next_free_node;  

        next_free_node = nodes_array[retrieved_node_index].next_free_node_index;
        nodes_array[next_free_node].next_free_node_index = temp;
        nodes_array[next_free_node].previous_node_index = temp3;

    }

    else if (prev_node_temp.node_state == free_block){
        nodes_array[retrieved_node_index].block_size += prev_node_temp.block_size;
        nodes_array[retrieved_node_index].block_start_addr = prev_node_temp.block_start_addr;

        heap_delete(prev_node_temp.heap_node_index);

        int retrieved_index2 = nodes_array[retrieved_node_index].heap_node_index;
        int temp7 = heap_insert(nodes_array[retrieved_node_index].block_size ,retrieved_node_index );
        nodes_array[retrieved_node_index].heap_node_index = temp7;
        // heap_update(nodes_array[retrieved_node_index].heap_node_index, nodes_array[retrieved_node_index].block_size);
        //heap_delete(retrieved_index2);
        prev_node_temp.block_size = 0;
        prev_node_temp.block_start_addr = NULL;
        prev_node_temp.block_end_addr = NULL;
        prev_node_temp.node_state = not_used;

        int temp4 = nodes_array[next_free_node].previous_node_index;
        int temp5 = next_free_node;

        next_free_node = nodes_array[retrieved_node_index].previous_node_index;
        nodes_array[next_free_node].next_free_node_index = temp5;
        nodes_array[next_free_node].previous_node_index = temp4;
       

    }
    else {
        heap_insert(nodes_array[retrieved_node_index].block_size,retrieved_node_index);
    }
}


// ----------------------------------------<HEAPPPPPPPPPPPPPPPPPPPPPPPPPPPPP>-------------------------

// void heap_update(int heap_node_index, size_t new_size){
//     heap_nodes[heap_node_index].size = new_size; //updated the new size as the block is now bigger
//     heapify(heap_node_index);
// }


//this function makes sure that largest size heap node is always is the root node
void heapify(int index){
    int l = 2 * index + 1;
    int r = 2 * index + 2;
    int max = index;

    if (r < heap_size && heap_nodes[r].size > heap_nodes[max].size){
        max = r;
    }
    if(l < heap_size && heap_nodes[l].size > heap_nodes[max].size){
        max = l;
    }

    if (index != max)
    {
        struct HeapNode temp = heap_nodes[index];
        heap_nodes[index] = heap_nodes[max];
        heap_nodes[max] = temp;
        nodes_array[heap_nodes[index].node_index].heap_node_index = index;
        nodes_array[heap_nodes[max].node_index].heap_node_index = max;
        heapify(max);
    }
}

//Here we are inserting a heap node in the heap_nodes array and making it a maxHeap
//such the node with the max size is the root node
int heap_insert(size_t size, int node_index) 
{

  heap_nodes[heap_size].size = size;
  heap_nodes[heap_size].node_index = node_index;
  nodes_array[node_index].heap_node_index = heap_size;
//   printf("HEAP SIZE: %d\n",)
  int index = heap_size;
//   printf("Index after line 241:%d \n",index);
  heap_size++;
  int parent = (index - 1) / 2;
//   printf("Parent after line 244:%d \n",parent);
  while (parent >= 0 &&  heap_nodes[index].size > heap_nodes[parent].size)
  {
    struct HeapNode temp = heap_nodes[index];
    heap_nodes[index] = heap_nodes[parent];
    heap_nodes[parent] = temp;

    nodes_array[heap_nodes[index].node_index].heap_node_index = index; 
    nodes_array[heap_nodes[parent].node_index].heap_node_index = parent; 

    index = parent;
    parent = (index - 1) / 2;
  }
//   printf("PARENTNTTTT: %d \n", parent);
//   printf("INDEXXXX: %d\n",index);
  return index;
}

int get_largest_size_node_index(int size)
{
    if (heap_size == 0){
        return -1;
    }
    int max = heap_nodes[0].node_index;

    if (nodes_array[max].node_state != free_block || nodes_array[max].block_size < size){
        return -1;
    }
    heap_nodes[0] = heap_nodes[heap_size -1];
    heap_size--;

    heapify(0);

    return max;

}


void heap_delete(int index){ 
    // printf("Heap_delete runnning\n");
    heap_nodes[index].size = BIG_CHUNK + 10;

    int parent = (index - 1) / 2;

    while( parent >= 0 &&   heap_nodes[index].size > heap_nodes[parent].size){
        struct HeapNode temp = heap_nodes[index];
        heap_nodes[index] = heap_nodes[parent];
        heap_nodes[parent] = temp;

        nodes_array[heap_nodes[index].node_index].heap_node_index = index;
        nodes_array[heap_nodes[parent].node_index].heap_node_index = parent;

        index = parent;
        parent = (index - 1) / 2;
    }
    heap_nodes[0] = heap_nodes[heap_size - 1];
    heap_nodes[heap_size - 1].node_index = -1; //This marks the fact that this heap node is not tracking any actaul node
    // heap_nodes[heap_size - 1].size = 0;
    //   printf("Current heap_size :%d\n", heap_size);
    heap_size--; 
    heapify(0); 
}

void printHeap(){
    if(heap_size == 0){
        printf("Heap is empty\n");
    }
    for(int i=0;i<heap_size;i++){
        printf(" %d,", heap_nodes[i].size);
        if(i == heap_size -1){
            printf(" ->END OF HEAP ARRAY");
        }
    }
    printf("\n");
    printf("The size of the heap is %d\n", heap_size);
    
}

