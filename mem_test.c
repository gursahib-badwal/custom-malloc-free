#include "my_malloc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void test2(){
    printf("Test 2 starts here.....\n");

    mem_init();

    char *a = (char *)my_malloc(10);
    char *b = (char *)my_malloc(20);
    char *c = (char *)my_malloc(30);
    char *d = (char *)my_malloc(40);
    char *e = (char *)my_malloc(50);
    char *f = (char *)my_malloc(60);

    printf("Freeing the memory allocated to B");
    my_free(b);
    printf("\n");
    printf("The heap array obtained after freeing B : ");
    printHeap();
    printf("\n");
    printf("The required heap array : ");
    printf("22, ->END OF HEAP ARRAY");
    printf("\n");

    printf("Test 2 ends here.....\n");
    mem_free();
}


void test3(){
    printf("Test 3 starts here .....\n");

    mem_init();

    char *a = (char *)my_malloc(10);
    char *b = (char *)my_malloc(20);
    char *c = (char *)my_malloc(30);
    char *d = (char *)my_malloc(40);
    char *e = (char *)my_malloc(50);
    char *f = (char *)my_malloc(60);

    printf("Freeing the memory allocated to B");
    my_free(b);
    printf("Freeing the memory allocated to D");
    my_free(d);

    printf("The heap array obtained after freeing B and D : ");
    printHeap();
    printf("The required heap array : ");
    printf("42 , 22 ->>>> END OF HEAP ARRAY");

    printf("Freeing the memory allocated to F");
    my_free(f);

    printf("The heap array obtained after freeing B, D, F : ");
    printHeap();
    printf("The required heap array : ");
    printf("62, 22, 42->>>> END OF HEAP ARRAY");

    printf("Test 3 ends here .....\n");
    mem_free();
}
void advanceTest(){
    printf("Advance Test starts here .....\n");
    mem_init();

    char *a = (char *)my_malloc(100);
    char *b = (char *)my_malloc(90);
    char *c = (char *)my_malloc(80);
    char *d = (char *)my_malloc(70);
    char *e = (char *)my_malloc(60);
    char *f = (char *)my_malloc(50);

    printf("Address After Allocation: ");
    printf("%d %d %d", b, d, f );


    printf("Freeing the memory allocated to B\n");
    my_free(b);
    printf("Freeing the memory allocated to D\n");
    my_free(d);

    printf("Freeing the memory allocated to F\n");
    my_free(f);

    printf("The heap array obtained after freeing B, D, F : ");
    printHeap();
    printf("\n");
    printf("The required heap array : ");
    printf("92, 72, 52 ->>>> END OF HEAP ARRAY");
    printf("\n");


    printf("Allocating Memory");
    char *g = (char *)my_malloc(90);
    char *i = (char *)my_malloc(70);
    char *h = (char *)my_malloc(50);
    printf("\n");

    printf("Printing the heap after allocation");
    printf("\n");
    printHeap();
    printf("\n");
    printf("Address After Allocation: ");
    printf("%d %d %d", g, i, h );
    printf("Advance Test ends here .....\n");
    mem_free();
}

void test4(){
    printf("Test 4 starts here .....\n\n");

    mem_init();

    char *a = (char*)my_malloc(10);
    char *b = (char*)my_malloc(20);
    char *c = (char*)my_malloc(30);
    char *d = (char*)my_malloc(40);
    char *e = (char*)my_malloc(50);
    char *f = (char*)my_malloc(60);

    printf("Freeing the memory allocated to B");
    my_free(b);
    printf("\n");
    printf("Freeing the memory allocated to D");
    my_free(d);
    printf("\n");

    printf("The heap array obtained after freeing B and D : ");
    printHeap();
    printf("\n");
    printf("The required heap array : ");
    printf("42 , 22 ->>>> END OF HEAP ARRAY");
    printf("\n");

    printf("Freeing the memory allocated to C");
    my_free(c);
    printf("\n");

    printf("The heap array obtained after freeing B, D and C : ");
    printHeap();
    printf("\n");
    printf("The required heap array : ");
    printf("74, 22 -> END OF HEAP ARRAY");
    printf("\n");

    printf("Test 4 ends here .....\n\n");
    mem_free();
}

void test5(){
    printf("Test 5 starts here .....\n");

    mem_init();

    char *a = (char *)my_malloc(10);
    char *b = (char *)my_malloc(20);
    char *c = (char *)my_malloc(30);
    char *d = (char *)my_malloc(40);
    char *e = (char *)my_malloc(50);
    char *f = (char *)my_malloc(60);

    printf("Freeing the memory allocated to D");
    my_free(d);
    printf("\n");
    printf("Freeing the memory allocated to E");
    my_free(e);
    printf("\n");

    printf("The heap array obtained after freeing D and E : ");
    printHeap();
    printf("\n");
    printf("The required heap array : ");
    printf("94 ->>>> END OF HEAP ARRAY");
    printf("\n");
    printf("Because we merged two adjacent blocks so there will be just one heap node having size of D + size of E");
    printf("Test 5 ends here .....\n");
    mem_free();
}


int main(){
    test2();
    test3();
    test4();
    test5();
    advanceTest();

    return 0;
}

