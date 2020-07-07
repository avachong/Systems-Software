//mymalloc.h header file

struct Block {
  int occ;              // whether block is occupied
  int size;             // size of block (including header)
  struct Block *prev;   // pointer to previous block
  struct Block *next;   // pointer to next block
};

//static struct Block *head;       // head of list
//static struct Block *tail;

void* my_malloc(int);
void  my_free(void*);
void dump_heap(void);
