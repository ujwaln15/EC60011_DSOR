// Name: Ujwal Nitin Nayak
// Roll No: 19EC39044
// EC60011: Data Structure and Object Representation
// Assignment 2: Comparing Absolute Values of Long Numbers Using Doubly Circular Linked List

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdbool.h>

// Function to convert string to number.
int getNum(char* sNum, int length){
    int num=0;
    for(int i=0; i<length; i++)
    {
        num=num*10;
        num=num+(*(sNum+i)-'0');
    }
    return num;
}

// Function to check validity of strings.
bool checkValid(char* num1, char* num2, int numDigits1, int numDigits2)
{
    // Checking if all the characters are digits between 0 and 9.
    for(int i=0; num1[i]!='\0'; i++)
    {
        if(num1[i]>='0'&&num1[i]<='9');
        else
            return false;
    }
    for(int i=0; num2[i]!='\0'; i++)
    {
        if(num2[i]>='0'&&num2[i]<='9');
        else
            return false;
    }
    return true;
}
bool checkValid2(char* num1, char* num2, int numDigits1, int numDigits2){
    // Checking if the lengths match with expected lengths.
    int i=0, j=0;
    while(num1[i++]!='\0');
    while(num2[j++]!='\0');
    return ((i-1==numDigits1)&&(j-1==numDigits2));
}

// Definition of structure of the node.
struct node {
    int info;
    struct node *left, *right;
};
typedef struct node *NODEPTR;

// Function to create a new node.
NODEPTR getNode(int val){
    NODEPTR p = (NODEPTR)malloc(sizeof(struct node));
    p->info = val;
    p->left = p;
    p->right = p;
    return p;
}

// Inserts new node after the current node p.
void insertAfter(NODEPTR p, int x){
    NODEPTR q,r;
    q = getNode(x);
    r = p->right;
    r->left = q;
    q->right = r;
    q->left = p;
    p->right = q;
}

// Fill the list with created chunks of size divLen.
NODEPTR fillList(NODEPTR head, char* num, int numDigits, int divLen){
    int curr=0;
    char* chunk = (char*)calloc(divLen,sizeof(char));
    NODEPTR temp = head;
    // Extracting the chunks and adding the chunks to
    // new list nodes.
    for(int i=numDigits-1; i>=0; i--)
    {
        curr++;
        chunk[divLen-curr]=*(num+i);
        if(curr==divLen)
        {
            int num = getNum(chunk, divLen);
            insertAfter(temp,num);
            temp = temp->right;
            curr=0;
            for(int l=0; l<divLen; l++)
                chunk[l]='0';
        }
    }
    if(curr!=0)
    {
        int num=getNum(chunk,divLen);
        insertAfter(temp,num);
    }
    return head;
}

// Recursive function to compare the absolute value of the two numbers.
int compareList(NODEPTR head1, NODEPTR head2, NODEPTR start1)
{
    if(head1->info == head2->info)
    {
        // Terminating condition which is hit when the numbers
        // are equal.
        if(head1->left==start1)
            return -1;
        return compareList(head1->left, head2->left, start1);
    }
    if(head1->info>head2->info)
        return 1;
    if(head2->info>head1->info)
        return 0;
}

int main(){

    // Taking the number of digits in the two numbers as input
    // to assist in dynamic memory allocation.
    int numDigits1, numDigits2;
    printf("Enter number of digits in the first number: \n");
    scanf("%d", &numDigits1);
    printf("Enter number of digits in the second number: \n");
    scanf("%d", &numDigits2);

    // Checking for edge cases
    if(numDigits1<=0||numDigits2<=0)
    {
        printf("Number must contain non-zero number of digits!");
        return 0;
    }

    // Dynamically allocating strings based on the number of
    // digits provided.
    char *num1, *num2;
    num1 = (char*)malloc((numDigits1+10)*sizeof(char));
    num2 = (char*)malloc((numDigits2+10)*sizeof(char));
    printf("Enter the first number: \n");
    scanf("%s",num1);
    printf("Enter the second number: \n");
    scanf("%s",num2);

    // Removing the leading sign if any.
    if(num1[0]=='-'||num1[0]=='+')
        num1++;
    if(num2[0]=='-'||num2[0]=='+')
        num2++;

    // Checking validity of the strings.
    if(!checkValid(num1,num2,numDigits1,numDigits2))
    {
        printf("Strings are not numbers!");
        return 0;
    }
    if(!checkValid2(num1,num2,numDigits1,numDigits2))
    {
        printf("Number of digits is not the same as that entered!");
        return 0;
    }
    
    // Dividing the number into segments of divLen digits each
    // and storing the segmented number in respective doubly 
    // circular linked lists.
    int divLen;
    printf("Enter chunk length (between 1 and 8): \n");
    scanf("%d", &divLen);
    // Value of chunk length should be between 1 and 8 to prevent
    // integer overflow.
    if(divLen<1||divLen>8)
    {
        printf("Chunk length should be between 1 and 8");
        return 0;
    }

    int numNodes1 = (numDigits1+divLen-1)/divLen;
    int numNodes2 = (numDigits2+divLen-1)/divLen;

    NODEPTR head1, head2;
    head1 = getNode(numNodes1);
    head2 = getNode(numNodes2);
    
    fillList(head1,num1,numDigits1,divLen);
    fillList(head2,num2,numDigits2,divLen);

    int verdict = compareList(head1,head2,head1);

    if(verdict==1)
        printf("First number is bigger.");
    else if(verdict==0)
        printf("Second number is bigger.");
    else
        printf("Both numbers are equal.");

    return 0;
}
