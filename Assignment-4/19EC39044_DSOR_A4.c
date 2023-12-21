// Name: Ujwal Nitin Nayak
// Roll No: 19EC39044
// EC60011: Data Structure and Object Representation
// Assignment 4: Constructing a Huffman Tree using a Priority Queue

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1001

// Declaring stucture and function prototypes
struct pair;
struct node;
struct PQueue;
typedef struct pair Pair;
typedef struct node *NODEPTR;
typedef struct PQueue *PQptr;
NODEPTR delete(PQptr);
void insert(PQptr,NODEPTR);


// Defining structures for supporting data handling in 
// the priority queue and the Huffman tree.
struct pair
{
    char symbol;
    int freq;
};

struct node
{
    Pair data;
    struct node *left, *right;
};

struct PQueue
{
    NODEPTR elements[MAX_SIZE];
    int last;
};


// -------- HUFFMAN TREE METHODS ----------

// Function to create a new tree node
NODEPTR makeTree()
{
    NODEPTR p = (NODEPTR)malloc(sizeof(struct node));
    p->data.symbol = '~';
    p->data.freq = -1;
    p->left = NULL;
    p->right = NULL;
    return p;
}

// Function to construct a Huffman tree using priority queue
NODEPTR constructHuffman(PQptr PQ)
{
    int numSymbols = PQ->last;
    NODEPTR root = NULL;
    for (int i = 0; i < numSymbols - 1; i++)
    {
        // Popping the least frequency elements
        NODEPTR top = delete (PQ);
        NODEPTR top2 = delete (PQ);
        // Creating a new parent node
        NODEPTR p = makeTree();
        // Storing the combined frequency of popped nodes into parent node
        p->data.freq = top->data.freq + top2->data.freq;
        p->left = top;
        p->right = top2;
        // Inserting the new node into the priority queue
        insert(PQ, p);
        // Rooting the Huffman tree at the new root
        root = p;
    }
    return root;
}

// Function to traverse the Huffman tree and obtain codes for each
// of the symbols
void traverseHuffman(NODEPTR root, int *code, int idx)
{
    // If the current node contains a valid symbol
    // print the code formed upto this point.
    if(root->data.symbol!='~')
    {
        printf("%c: ", root->data.symbol);
        for(int i=0; i<idx; i++)
        {
            printf("%d", code[i]);
        }
        printf("\n");
        return;
    }
    // Add 0 to the code while moving to the left child
    code[idx]=0;
    traverseHuffman(root->left,code,idx+1);
    // Add 1 to the code while moving to the right child
    code[idx]=1;
    traverseHuffman(root->right,code,idx+1);
    return;
}
// ----------------------------------------

// -------- PRIORITY QUEUE METHODS --------

// Function to clear the priority queue
void makeNull(PQptr PQ)
{
    PQ->last = 0;
}
// Function to check if the priority queue is empty
int isEmpty(PQptr PQ)
{
    if (PQ->last == 0)
        return 1;
    return 0;
}
// Function to check if the symbol already exists in the priority queue
int checkExistence(PQptr PQ, char symb)
{
    int curr = 1;
    int last = PQ->last;
    for (; curr <= last; curr++)
    {
        if (PQ->elements[curr]->data.symbol == symb)
            return 1;
    }
    return 0;
}
// Function swap two nodes in the priority queue
void swap(NODEPTR *a, NODEPTR *b)
{
    NODEPTR temp = *b;
    *b = *a;
    *a = temp;
}
// Function to maintain a balanced binary heap structure 
// after every insert and delete
void heapify(PQptr PQ, int curr)
{
    if (PQ->last == 1)
        return;
    int elements = PQ->last;
    int mn = curr;
    int l = 2 * curr;
    int r = 2 * curr + 1;
    if (l <= elements && PQ->elements[l]->data.freq < PQ->elements[mn]->data.freq)
        mn = l;
    if (r <= elements && PQ->elements[r]->data.freq < PQ->elements[mn]->data.freq)
        mn = r;
    if (mn != curr)
    {
        swap(&(PQ->elements[curr]), &(PQ->elements[mn]));
        heapify(PQ, mn);
    }
}
// Function to insert a node into the priority queue
void insert(PQptr PQ, NODEPTR element)
{
    if (PQ->last == MAX_SIZE - 1)
    {
        printf("Priority queue overflow!");
        return;
    }
    // Increment last and add element in new position
    PQ->last++;
    PQ->elements[PQ->last] = element;
    // If there is more than one element, heapify the tree
    if (PQ->last > 1)
    {
        for (int i = (PQ->last) / 2; i >= 1; i--)
        {
            heapify(PQ, i);
        }
    }
}
// Function to delete a node from the priority queue
NODEPTR delete(PQptr PQ)
{
    NODEPTR out = makeTree();
    out->data.symbol = '~';
    out->data.freq = -1;
    if (isEmpty(PQ))
    {
        printf("Empty");
        return out;
    }
    // Store top
    NODEPTR top = PQ->elements[1];
    // Replace top with last element
    swap(&(PQ->elements[1]), &(PQ->elements[PQ->last]));
    // Reduce last to remove it from scope
    PQ->last--;
    // Heapify the remaining tree
    for (int i = (PQ->last) / 2; i >= 1; i--)
    {
        heapify(PQ, i);
    }
    return top;
}
// ----------------------------------------

int main()
{
    PQptr PQ = (PQptr)malloc(sizeof(struct PQueue));
    makeNull(PQ);
    int numSymbols = 0;
    printf("Enter the number of symbols: ");
    scanf("%d", &numSymbols);
    if(numSymbols<0)
    {
        printf("Number of symbols should be non-negative!\n");
        return 0;
    }
    // Preventing overflow
    if(numSymbols>MAX_SIZE-1)
    {
        printf("Reduce number of symbols or increase MAX_SIZE!\n");
        return 0;
    }
    for (int i = 0; i < numSymbols; i++)
    {
        char symb;
        int freq;
        printf("Symbol %d (~ not allowed): ", (i + 1));
        scanf(" %c", &symb);
        if(symb == '~')
        {
            printf("Illegal symbol!\n");
            i--;
            continue;
        }
        printf("Frequency of symbol %c: ", symb);
        scanf(" %d", &freq);
        if(freq<=0)
        {
            printf("Frequency must be a positive integer!\n");
            i--;
            continue;
        }
        NODEPTR element = makeTree();
        element->data.symbol = symb;
        element->data.freq = freq;
        if (checkExistence(PQ, symb))
        {
            printf("Symbol already exists!\n");
            i--;
        }
        else
            insert(PQ, element);
    }
    if (isEmpty(PQ))
    {
        printf("No symbols!\n");
        return 0;
    }
    else if (PQ->last == 1)
    {
        printf("\n\nHUFFMAN CODES:\n");
        printf("%c: 0", PQ->elements[PQ->last]->data.symbol);
    }
    else
    {
        printf("\n\nHUFFMAN CODES:\n");
        // Integer array to store the running code
        int *code = (int*)malloc(numSymbols*sizeof(int));
        NODEPTR root = constructHuffman(PQ);
        traverseHuffman(root,code,0);
    }
    return 0;
}
