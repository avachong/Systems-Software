#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "mymalloc.h"

//start list
static struct Block *head = NULL; 
static struct Block *root;

//my malloc func
void* my_malloc(int size){
    if(head == NULL){
        //make space for the header and fill in
        head = sbrk(sizeof(struct Block));
        root = head;
        void *ptr = sbrk(size); 
        head->occ = 1;
        head->prev = NULL;
        head->next = NULL;
        head->size = size + sizeof(struct Block);  
        //return start of malloc
        return ptr;     
    } else {
        //current block to head
        struct Block *curr = head; 
        int lowest = -1; 
        //free block with lowest wasted space
        struct Block *currLowest; 

        //traverse linked list
        while(curr->next != NULL){
            //perfect fit
            if(curr->occ == 0 && curr->size == (size + sizeof(struct Block)))
            {
                curr->occ = 1;
                return ((char*)curr + sizeof(struct Block));
            } 
            //next best fit with some wasted space
            if(curr->occ == 0 && curr->size >= (size + sizeof(struct Block)) && (lowest == -1 || curr->size < lowest))
            {
                lowest = curr->size; 
                currLowest = curr; 
            }
            curr = curr->next;
        }

        //end of the list, no current blocks are large enough
        if(lowest == -1)
        {
            //allocate size of block to append to heap
            struct Block *new = sbrk(sizeof(struct Block));
            //pointer to START of data
            void *ptr = sbrk(size);
            new->occ = 1; 
            new->size = (size + sizeof(struct Block)); 
            new->prev = curr;
            new->prev->next = new; 
            new->next = NULL;
 
            return ptr;  //start of malloc
        }

        //free spot was found, but bigger than needed and could store another set of data
        if(lowest > sizeof(struct Block) * 2 + size)
        {
            struct Block *newNode = (struct Block*)((char*)currLowest + size + sizeof(struct Block)); 
            //set size to what the current block won't use and set to free
            newNode->size = currLowest->size - size - sizeof(struct Block);   
            newNode->occ = 0;                
            //set new nodes next to large old blocks next                                              
            newNode->next = currLowest->next;                                          
            newNode->next->prev = newNode;
            //make current lowest the nodes prev
            newNode->prev = currLowest;                                                  
            newNode->prev->next = newNode;

            //make size the malloc plus original struct size
            //reassure that the best fit's next node will be the new free node
            currLowest->size = size + sizeof(struct Block);                              
            currLowest->next = newNode;                                                  
            currLowest->next->prev = currLowest;
            currLowest->occ = 1;
            return (currLowest + sizeof(struct Block));
        }
        return currLowest;
    }
}

void my_free(void *data){
    if(data - sbrk(0) > 0){
        return;
    } 
    
    struct Block *d = 0; //data block
    struct Block *prevNode = NULL;
    struct Block *nextNode = NULL; 

    d = data - sizeof(struct Block);
    if(d->prev != NULL){
        prevNode = d->prev;
    }
    if(d->next != NULL){
        nextNode = d->next;
    }
    if(prevNode == NULL && nextNode == NULL){
        head = NULL;
        sbrk(-(d->size));
        return;
    }

    //free from the tail
    else if(nextNode == NULL) {
        int size = 0;
        if(prevNode->occ == 0){
            //
            size = prevNode->size + d->size;
            if(prevNode->prev != root){
                prevNode = prevNode->prev;
                prevNode->next = NULL;
                sbrk(-(size));
                return;
            }else {
                sbrk(-(size + root->size));
                head = NULL;
                return;
            }
        }else {
            prevNode -> next = NULL;
            sbrk(-(d->size));
            return;
        }
    }

    //free from the head
    else if (prevNode == NULL){
        int size = 0;
        if(nextNode->occ == 0){
            size = nextNode->size + d->size;
            if(nextNode->next != NULL){
                nextNode = nextNode->next;
                d->size = size;
                d->next = nextNode;
                d->next->prev = d;
                d->occ = 0;
                return;

            } else {
                sbrk(-(size + root->size));
                head = NULL;
                return;
            }
        } else{
            d->occ = 0;
            return;
        }

    }
    //CASE: Freeing node and coalescing with one behind it
    else if (prevNode->occ == 0){
        //memory expands in both directions 
        int newSize = 0;
        if(nextNode->occ == 0){
            //new size is all three sizes combines
            newSize = prevNode->size + nextNode->size + d->size;
            if(nextNode != NULL && prevNode != root){
                nextNode = nextNode->next;
                prevNode = prevNode->prev; 
                d->size = newSize;
                d->prev = prevNode; 
                d->prev->next = d;
                d->next = nextNode; 
                d->next->prev = d;
                d->occ = 0;
                return;
            } else if (nextNode != NULL && prevNode == root){
                sbrk(-(newSize + root->size));
                head = NULL;
                return;
            } else if (nextNode->next == NULL && prevNode != root){
                prevNode = prevNode->prev;
                prevNode->next = NULL;
                sbrk(-(newSize));
                return;
            }
            //free from the head 
        } else /*nextNode->occ == 1 */{
            newSize = prevNode->size + d->size;
            if(prevNode->prev != root){
                prevNode = prevNode->prev;
                d->size = newSize;
                d->prev = prevNode;
                d->prev->next = d;
                d->occ = 0;
                return;
            } else {
                d->size = newSize;
                d->prev = root;
                d->occ = 0;
                return;
            }
        }
    }

    else if(prevNode->occ == 1){
        if(nextNode->occ == 0){
            int newSize = d->size + nextNode->size;
            if(nextNode->next != NULL){
                d->size = newSize;
                nextNode = nextNode->next;
                d->next = nextNode;
                d->next->prev = d;
                d->occ = 0;
                return;
            } else {    
                prevNode->next = NULL;
                //two from tail, free whole cluster
                sbrk(-(newSize));                                                                  
                return;
            }
        } else {
            //memory can only be freed between prev and next nodes
            d->occ = 0;
            return;
        }
    }
}

//given dump heap function 
void dump_heap()
{
    struct Block *cur;
    printf("brk: %p\n", sbrk(0));
    printf("head->");
    for(cur = head; cur != NULL; cur = cur->next) {
        printf("[%d:%ld:%d]->", cur->occ, (char*)cur - (char*)head, cur->size);
        fflush(stdout);
        assert((char*)cur >= (char*)head && (char*)cur + cur->size <= (char*)sbrk(0)); // check that block is within bounds of the heap
        if(cur->next != NULL) {
            assert(cur->next->prev == cur); // if not last block, check that forward/backward links are consistent
            assert((char*)cur + cur->size == (char*)cur->next); // check that the block size is correctly set
        }
  }
    printf("NULL\n");
}