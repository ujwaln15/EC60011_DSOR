// Name: Ujwal Nitin Nayak
// Roll No: 19EC39044
// EC60011: Data Structure and Object Representation
// Assignment 1: Implementation of Ackermann Function

#include<stdio.h>
#include<math.h>
#include<string.h>
#define __uint64 long long
#define ROW_SIZE 6
#define COL_SIZE 17

// RECURSIVE FORMULATION OF ACKERMANN FUNCTION
long long rec(int i, int j)
{
    if(i==1){
        if(j<=62){
            return pow(2,j);
        }else{
            return -1;
        }
    }
    if(j==1){
        return rec(i-1,2);
    }
    if(rec(i,j-1)==-1){
        return -1;
    } else {
        return rec(i-1,rec(i,j-1));
    }
    return -1;
}

void main()
{
    // ITERATIVE FORMULATION OF ACKERMANN FUNCTION
    printf("\n----------------------------------------ITERATIVE----------------------------------------\n\n");
    // Initialising the table
    long long arr[ROW_SIZE][COL_SIZE];
    for(int i=1; i<ROW_SIZE; i++){
        for(int j=1; j<COL_SIZE; j++){
            arr[i][j]=-1;
        }
    }
    
    // Populating the table
    arr[1][1]=2;
    for(int j=2; j<COL_SIZE; j++){
        arr[1][j]=arr[1][j-1]*2;
    }
    for(int i=2; i<ROW_SIZE; i++){
        if(arr[i-1][2]!=-1)
            arr[i][1] = arr[i-1][2];
        for(int j=2; j<COL_SIZE; j++){
            int x = i-1;
            int y = arr[i][j-1];
            if(x==1 && (y>=1 && y<COL_SIZE)){
                arr[i][j]=arr[x][y];
            } else if((y>=1 && y<COL_SIZE) && arr[x][y]!=-1){
                arr[i][j]=arr[x][y];
            }
        }
    }

    for(int i=1; i<ROW_SIZE; i++){
        for(int j=1; j<COL_SIZE; j++){
            printf("%4d ",arr[i][j]);
        }
        printf("\n");
    }

    printf("\n----------------------------------------RECURSIVE----------------------------------------\n\n");
    for(int i=1; i<ROW_SIZE; i++){
        for(int j=1; j<COL_SIZE; j++){
            printf("%4d ",rec(i,j));
        }
        printf("\n");
    }
    printf("\n-----------------------------------------------------------------------------------------\n");
    printf("Note: -1 signifies an integer overflow. This shows that the function is very fast-growing.");
}