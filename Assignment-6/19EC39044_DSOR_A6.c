// Name: Ujwal Nitin Nayak
// Roll No: 19EC39044
// EC60011: Data Structure and Object Representation
// Assignment 6: Implementation of insert and delete operations in a Red-Black Tree

#include<stdio.h>
#include<stdlib.h>

enum {
    RED,
    BLACK,
    DBLACK
};

struct node {
    int data;
    int colour; // Red -> 0, Black -> 1
    struct node *left, *right, *parent;
    int isLeaf;
};
typedef struct node *NODEPTR;

NODEPTR getNode(int x, int isLeaf)
{
    NODEPTR p = (NODEPTR)malloc(sizeof(struct node));
    p->data = x;
    p->colour = RED;
    p->left = NULL;
    p->right = NULL;
    p->parent = NULL;
    p->isLeaf = isLeaf;
    return p;
}

void leftRotate(NODEPTR *T, NODEPTR x)
{
    NODEPTR y = x->right;
    x->right = y->left;
    if(y->left!=NULL)
        y->left->parent = x;
    if(x->parent==NULL)
        *(T) = y;
    else {
        y->parent = x->parent;
        if(x==x->parent->left)
            x->parent->left=y;
        else
            x->parent->right=y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(NODEPTR *T, NODEPTR y)
{
    NODEPTR x = y->left;
    y->left = x->right;
    if(x->right)
        x->right->parent = y;
    if(y->parent==NULL)
        *(T) = x;
    else {
        x->parent = y->parent;
        if(y==y->parent->left)
            y->parent->left=x;
        else
            y->parent->right=x;
    }
    x->right = y;
    y->parent = x;
}

void bstInsert(NODEPTR *T, NODEPTR x, NODEPTR par)
{
    if((*T)==NULL)
    {
        (*T)=x;
        (*T)->parent=par;
        return;
    }
    if(x->data==(*T)->data)
        return;
    NODEPTR root = (*T);
    if(x->data<root->data)
        bstInsert(&(root->left),x,root);
    else
        bstInsert(&(root->right),x,root);
}
NODEPTR y;
int bstDeleteMin(NODEPTR x)
{
    NODEPTR par = x->parent;
    if(x->left==NULL)
    {
        int temp=x->data;
        if(par->left==x)
            par->left=x->right;
        else
            par->right=x->right;
        if(x->right != NULL)
        {
            x->right->parent=par;
            y = x->right;
        }
        else
        {
            y = getNode(-1,1);
            y->parent=par;
            // printf("%d ", x->data);
            if(x->colour==RED)
                par->right=NULL;
            else
            {
                y->colour=DBLACK;
                par->right=y;
            }
        }
        free(x);
        return temp;
    }
    return bstDeleteMin(x->left);
}
void bstDelete(NODEPTR *T, NODEPTR x)
{
    NODEPTR root=*T;
    if(root!=NULL)
    {
        if(x->data<root->data)
            return bstDelete(&(root->left),x);
        else if(x->data>root->data)
            return bstDelete(&(root->right),x);
        else if(root->left==NULL&&root->right==NULL)
        {
            if(root->colour==RED)
            {
                if(root->parent!=NULL)
                {
                    if(root->parent->left==root)
                        root->parent->left=NULL;
                    else
                        root->parent->right=NULL;
                }
                y = NULL;
                free(root);
            }
            else
            {
                y = getNode(-1,1);
                y->parent = (*T)->parent;
                y->colour = DBLACK;
                (*T)=y;
                free(root);
            }
        }
        else if(root->left==NULL)
        {
            y = root->right;
            y->parent = root->parent;
            if(root->parent->left==root)
                root->parent->left=y;
            else
                root->parent->right=y;
            if(root->colour==BLACK)
                y->colour = ((y->colour==RED)?BLACK:DBLACK);
            (*T)=y;
        }
        else if(root->right==NULL)
        {
            y = root->left;
            y->parent = root->parent;
            if(root->parent->left==root)
                root->parent->left=y;
            else
                root->parent->right=y;
            if(root->colour==BLACK)
                y->colour = ((y->colour==RED)?BLACK:DBLACK);
            (*T)=y;
        }
        else
        {
            (*T)->data=bstDeleteMin(root->right);
        }
    }
}

void rbInsert(NODEPTR *T, NODEPTR x)
{
    bstInsert(T,x,NULL);
    while (x!=(*T) && x->parent->colour == RED){
        if(x->parent == x->parent->parent->left)
        {   
            NODEPTR y = x->parent->parent->right;
            if(y!=NULL&&y->colour == RED)
            {
                x->parent->colour = BLACK;
                x->parent->parent->colour = RED;
                y->colour = BLACK;
                x = x->parent->parent;
            }
            else
            {
                if(x==x->parent->right)
                {
                    x = x->parent;
                    leftRotate(T,x);
                }
                x->parent->colour = BLACK;
                x->parent->parent->colour = RED;
                rightRotate(T,x->parent->parent);
            }
        }
        else 
        {
            NODEPTR y = x->parent->parent->left;
            if(y!=NULL&&y->colour == RED)
            {
                x->parent->colour = BLACK;
                x->parent->parent->colour = RED;
                y->colour = BLACK;
                x = x->parent->parent;
            }
            else
            {
                if(x==x->parent->left)
                {
                    x = x->parent;
                    rightRotate(T,x);
                }
                x->parent->colour = BLACK;
                x->parent->parent->colour = RED;
                leftRotate(T,x->parent->parent);
            }
        }
    }
    (*T)->colour=BLACK;
    return;
}

void removeNil(NODEPTR root)
{
    NODEPTR temp=root;
    if(!temp)
        return;
    while(temp->left)
        temp=temp->left;
    if(temp->data==-1)
    {
        if(temp->parent)
            temp->parent->left=temp->right;
        else
            root=temp->right;
    }
}

void rbDelete(NODEPTR *T, NODEPTR x)
{
    bstDelete(T,x);
    x = y;
    if(x==NULL)
        return;
    while(x!=(*T)&&x->colour==DBLACK)
    {
        if(x==x->parent->left)
        {
            NODEPTR w=x->parent->right;
            // Case 1
            if(w->colour==RED)
            {
                w->colour=BLACK;
                x->parent->colour=RED;
                leftRotate(T,x->parent);
                w=x->parent->right;
            }
            // Case 2
            if(w&&(w->left==NULL||w->left->colour==BLACK)&&(w->right==NULL||w->right->colour==BLACK))
            {
                w->colour=RED;
                x->colour=BLACK;
                x = x->parent;  
                x->colour = (x->colour==RED?BLACK:DBLACK);
            }
            else
            {
                // Case 3
                if(w&&(w->right==NULL||w->right->colour==BLACK))
                {
                    w->colour=RED;
                    w->left->colour=BLACK;
                    rightRotate(T,w);
                }
                // Case 4
                x->colour=BLACK;
                w->colour=x->parent->colour;
                x->parent->colour=BLACK;
                if(w->right)
                    w->right->colour=BLACK;
                leftRotate(T,x->parent);
                x=(*T);
            }
        }
        else
        {
            NODEPTR w=x->parent->left;
            // Case 1
            if(w->colour==RED)
            {
                w->colour=BLACK;
                x->parent->colour=RED;
                rightRotate(T,x->parent);
                w=x->parent->left;
            }
            // Case 2
            if(w&&(w->left==NULL||w->left->colour==BLACK)&&(w->right==NULL||w->right->colour==BLACK))
            {
                w->colour=RED;
                x->colour=BLACK;
                x = x->parent;
                x->colour = (x->colour==RED?BLACK:DBLACK);
            }
            else
            {
                // Case 3
                if(w&&(w->left==NULL||w->left->colour==BLACK))
                {
                    w->colour=RED;
                    w->right->colour=BLACK;
                    leftRotate(T,w);
                }
                // Case 4
                x->colour=BLACK;
                w->colour=x->parent->colour;
                x->parent->colour=BLACK;
                if(w->left)
                    w->left->colour=BLACK;
                rightRotate(T,x->parent);
                x=(*T);
            }
        }
    }
    (*T)->colour = BLACK;
    removeNil((*T));
    return;
}

void inorderTraversal(NODEPTR *T)
{
    NODEPTR root=(*T);
    if(T==NULL||root==NULL)
        return;
    inorderTraversal(&(root->left));
    printf("(%d, %d)\n", root->data, root->colour);
    inorderTraversal(&(root->right));
}

struct pair{
    int data,colour;
};
typedef struct pair PAIR;

PAIR lot[100][100];
int last[100];

void levelOrderTraversal(NODEPTR root, int level)
{
    if(root==NULL)
        return;
    last[level]++;
    lot[level][last[level]].data=root->data;
    lot[level][last[level]].colour=root->colour;
    levelOrderTraversal(root->left,level+1);
    levelOrderTraversal(root->right,level+1);
}

void printLevelOrder(NODEPTR root)
{
    for(int i=0; i<100; i++)
    {
        for(int j=0; j<100; j++)
        {
            lot[i][j].data=-1;
            lot[i][j].colour=-1;
        }
        last[i]=-1;
    }
    printf("LEVEL ORDER TRAVERSAL\n");
    levelOrderTraversal(root,0);
    for(int i=0; i<100; i++)
    {
        if(last[i]==-1)
            break;
        for(int j=0; j<=last[i]; j++)
            printf("(%d, %d) ",lot[i][j].data, lot[i][j].colour);
        printf("\n");
        last[i]=-1;
    }
}

int main()
{
    printf("RED: 0, BLACK: 1\n\n");
    NODEPTR root=NULL;
    // NODEPTR x = getNode(5);
    // rbInsert(&root,x);
    printf("Insert 35\n");
    NODEPTR x = getNode(35,0);
    rbInsert(&root,x);
    printf("Insert 12\n");
    x = getNode(12,0);
    rbInsert(&root,x);
    printf("Insert 17\n");
    x = getNode(17,0);
    rbInsert(&root,x);
    printf("Insert 21\n");
    x = getNode(21,0);
    rbInsert(&root,x);
    printf("Insert 14\n");
    x = getNode(14,0);
    rbInsert(&root,x);
    printf("Insert 11\n");
    x = getNode(11,0);
    rbInsert(&root,x);
    printf("Insert 8\n");
    x = getNode(8,0);
    rbInsert(&root,x);
    printf("Insert 55\n");
    x = getNode(55,0);
    rbInsert(&root,x);
    printf("Insert 10\n");
    x = getNode(10,0);
    rbInsert(&root,x);
    printLevelOrder(root);
    // printf("%d ", root->data);
    printf("\n");
    printf("Deleting 8\n");
    x = getNode(8,0);
    rbDelete(&root,x);
    printLevelOrder(root);
    printf("\n");
    printf("Deleting 10\n");
    x = getNode(10,0);
    rbDelete(&root,x);
    printLevelOrder(root);
    printf("\n");
    printf("Deleting 11\n");
    x = getNode(11,0);
    rbDelete(&root,x);
    printLevelOrder(root);
    printf("\n");
    printf("Deleting 21\n");
    x = getNode(21,0);
    rbDelete(&root,x);
    printLevelOrder(root);
    printf("\n");
    printf("Deleting 35\n");
    x = getNode(35,0);
    rbDelete(&root,x);
    printLevelOrder(root);
    printf("\n");
    printLevelOrder(root);
    return 0;
}

