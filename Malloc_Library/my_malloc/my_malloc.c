#include "my_malloc.h"

#include <limits.h>
#include <stdio.h>
#include <unistd.h>

//get total data size
unsigned long get_data_segment_size() {
  return len_node;
}
//get free data size
unsigned long get_data_segment_free_space_size() {
  //use free list to traverse each node
  node * curr = headNode;
  unsigned long free_size = 0;
  //traverse each node
  while (curr != NULL) {
    free_size = free_size + curr->size + sizeof(node);
    curr = curr->next;
  }
  return free_size;
}
//print list for test
void printList() {
  if (headNode == NULL) {
    printf("NULL\n");
    return;
  }
  node * curr = headNode;
  printf("here is list:\n");
  while (curr != NULL) {
    printf("Node address: %p, size: %lu\n", (char *)curr, curr->size);
    curr = curr->next;
  }
}
//print node for test
void printNode(node * curr) {
  printf("Node address: %p, size: %lu\n", (char *)curr, curr->size);
}

//First Fit malloc/free
void * ff_malloc(size_t size) {
  //test!!!
  //printf("in ff malloc\n");
  //printList();
  node * n1 = findNode_ff(size);//first find node by first fit method
  if (n1 == NULL) {
    //if there is no node found get new node
    n1 = newNode(size);
    return (void *)n1 + sizeof(node);
  }
  //remove found node from free list
  removeNode(n1);
  return (void *)n1 + sizeof(node);//return ptr
}

//Best Fit malloc/free
void *bf_malloc(size_t size){
  node * n1 = findNode_bf(size);//first find node by best fit method
  if (n1 == NULL) {
    //if there is no node found get new node
    n1 = newNode(size);
    return (void *)n1 + sizeof(node);
  }
  //remove found node from free list
  removeNode(n1);
  return (void *)n1 + sizeof(node);//return ptr
}
//find node by ff
node * findNode_ff(size_t size) {
  node * curr = headNode;
  //traverse node to find first one
  while (curr != NULL) {
    if (curr->size >= size) {
      //if found, split it
      node* n1 = splitNode(curr, size);
      return n1;
    }
    else {
      curr = curr->next;
    }
  }
  //printf("no such node\n");
  //if not found, return NULL
  return NULL;
}
//find node by bf
node * findNode_bf(size_t size) {
  node * curr = headNode;
  node * bfNode = NULL;//best node
  int num = INT_MAX;//difference between each node size
  while (curr != NULL) {
    //get temp difference
    int tdiff = curr->size - size;
    if (tdiff == 0)//if size is equal return
      return curr;
    else if (tdiff > 0 && num > tdiff) { //else update num and bfNode
      bfNode = curr;
      num = tdiff;
    }
    else {
      curr = curr->next;
      continue;
    }
  }
  //if there is node found
  if (bfNode != NULL) {
    //split node into two part
    node* n1 = splitNode(bfNode, size);
    return n1;
  }
  //if not found, return NULL
  return NULL;
}
//get new node
node * newNode(size_t size) {
  void * nNode = sbrk(size + sizeof(node));//if function can not get enough space
  if (nNode == (void *)-1)
    return NULL;
  //get node of last break value
  node * n1 = nNode;
  n1->size = size;
  n1->next = NULL;
  n1->prev = NULL;
  //update total used space
  len_node += size + sizeof(node);
  return n1;
}
//free space by ff
void ff_free(void * ptr) {
  if (ptr == NULL)//if no pointer to free
    return;
  //get real node by pointer
  node * n1 = (node *)((void *)ptr - sizeof(node));
  addNode(n1);//add this node
  mergeNode(n1);//merge this node
}
//free space by bf
void bf_free(void *ptr){
  ff_free(ptr);
}
//remove node from list
void removeNode(node * n1) {
  //if this node is headNode
  if (headNode == n1 && tailNode != n1) {
    headNode = n1->next;
    n1->next->prev = NULL;
  }
  //if this node is tailNode
  else if (tailNode == n1 && headNode != n1) {
    tailNode = n1->prev;
    n1->prev->next = NULL;
  }
  //if this node is only node in list
  else if (headNode == n1 && tailNode == n1) {
    headNode = NULL;
    tailNode = NULL;
  }
  //if this node is between other two nodes
  else {
    n1->prev->next = n1->next;
    n1->next->prev = n1->prev;
  }
  //update n1
  n1->next = NULL;
  n1->prev = NULL;
}
//add node to free list
void addNode(node * n1) {
  node * curr = headNode;
  //if list is empty
  if (headNode == NULL && tailNode == NULL) {
    headNode = n1;
    tailNode = n1;
    return;
  }
  //insert before headnode
  if (headNode >= n1) {
    n1->next = headNode;
    headNode->prev = n1;
    headNode = n1;
    return;
  }
  //insert after tailnode
  if (tailNode <= n1) {
    n1->prev = tailNode;
    tailNode->next = n1;
    tailNode = n1;
  }
  //if list is not empty
  while (curr != NULL && curr->next != NULL) {
    //insert between two nodes
    if (curr < n1 && curr->next > n1) {
      n1->next = curr->next;
      n1->prev = curr;
      curr->next->prev = n1;
      curr->next = n1;
      return;
    }
    else {
      curr = curr->next;
    }
  }
}

//split node if necessary
node * splitNode(node * curr, size_t size) {
  //if there is no need to split
  if(curr->size - size <= sizeof(node)){
    return curr;
  }
  //get pointer of remain node
  node * n1 = (void*)curr + sizeof(node) + size;
  //set remain node n1
  n1->size = curr->size - sizeof(node) - size;
  n1->next = curr->next;
  n1->prev = curr;
  //set curr node
  curr->size = size;
  curr->next = n1;
  //set node after n1
  if (n1->next != NULL) {
    n1->next->prev = n1;
  }
  //if it is a tailNode
  else
    tailNode = n1;
  return curr;
}

void merge_helper(node* temp, node* n1){
    n1->size = n1->size + temp->size + sizeof(node);
    n1->next = temp->next;
    if (temp->next != NULL)
      temp->next->prev = n1;
    else
      tailNode = n1;
}

void mergeNode(node * n1) {
  //merge with back
  if (n1->next != NULL && (void*)n1 == (void*)n1->next - n1->size - sizeof(node)) {
    //printf("merge with back\n");
    node * temp = n1->next;
    merge_helper(temp, n1);
    temp->size = 0;
  }
  //merge with front
  if (n1->prev != NULL && (void*)n1 == (void*)n1->prev + n1->prev->size + sizeof(node)) {
    //printf("merge with front\n");
    node * temp = n1->prev;
    merge_helper(n1, temp);
  }
  //printf("finish merge\n");
}
