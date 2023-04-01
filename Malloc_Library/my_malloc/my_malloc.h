//
// Created by yx C on 2023/1/23.
//
#ifndef PJ1_MY_MALLOC_H
#define PJ1_MY_MALLOC_H
#include <stdio.h>
#include <stdbool.h>

struct node {
    size_t size;
    //bool isFree;
    struct node* next;
    struct node* prev;
};
typedef struct node node;

//First Fit malloc/free
void *ff_malloc(size_t size);
void ff_free(void *ptr);

//Best Fit malloc/free
void *bf_malloc(size_t size);
void bf_free(void *ptr);

//Find ff node
node* findNode_ff(size_t size);
//Find bf node
node* findNode_bf(size_t size);
//Make a new node
node* newNode(size_t size);
//remove a node from free list
void removeNode(node* n1);
//add node to free list
void addNode(node* n1);
//merge node together
void mergeNode(node* n1);
//split node if necessary
node* splitNode(node* curr, size_t size);

//get the total data segmentation size
unsigned long get_data_segment_size(); //in bytes
//get the free data size
unsigned long get_data_segment_free_space_size(); //in byte

node* headNode = NULL;//head node of list
node* tailNode = NULL;//tail node of list
unsigned long len_node = 0;//total size of heap;

#endif //PJ1_MY_MALLOC_H
