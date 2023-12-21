// Name: Ujwal Nitin Nayak
// Roll No: 19EC39044
// EC60011: Data Structure and Object Representation
// Assignment 3: Generating the game tree for a Tic-Tac-Toe game.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Defining a structure to store the game state.
struct nodetype {
    char board[3][3];
    int turn;
    struct nodetype *child;
    struct nodetype *next;   
};
typedef struct nodetype *GAME_STATE;

// Defining a structure to help propogate states and their evaluation values 
// from the leaf nodes of the game tree up to the root.
struct pairStruct{
    GAME_STATE state;
    int evalVal;
};
typedef struct pairStruct pair;

// Initialising an empty game state
GAME_STATE initState() {
    GAME_STATE state = (GAME_STATE)malloc(sizeof(struct nodetype));
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++){
            state->board[i][j]='_';   
        }
    }
    state->turn=0;
    state->child=NULL;
    state->next=NULL;
    return state;
}

// Function to check if the state is a win state (terminal)
int checkWin(GAME_STATE state, char x) {
    // Row fulfilled
    for(int i=0; i<3; i++) {
        int in_row = 0;
        for(int j=0; j<3; j++) {
            in_row += (state->board[i][j]==x);
        }
        if(in_row==3)
            return 1;
    }
    // Column fulfilled
    for(int j=0; j<3; j++) {
        int in_col = 0;
        for(int i=0; i<3; i++) {
            in_col += (state->board[i][j]==x);
        }
        if(in_col==3)
            return 1;
    }

    // Diagonal fulfilled
    // Forward diagonal
    int in_diag = 0;
    for(int i=0; i<3; i++) {
        in_diag += (state->board[i][i]==x);
    }
    if(in_diag==3)
        return 1;
    // Backward diagonal
    in_diag = 0;
    for(int i=0; i<3; i++) {
        in_diag += (state->board[i][2-i]==x);
    }
    if(in_diag==3)
        return 1;

    return 0;
}

// Check if row is winnable for character x
int rowCheck(GAME_STATE state, char x, int row) {
    char alt = (x=='X'?'O':'X');
    for(int k=0; k<3; k++)
    {
        if(state->board[row][k]==alt)
            return 0;
    }
    return 1;
}

// Check if column is winnable for character x
int colCheck(GAME_STATE state, char x, int col) {
    char alt = (x=='X'?'O':'X');
    for(int k=0; k<3; k++)
    {
        if(state->board[k][col]==alt)
            return 0;
    }
    return 1;
}

// Check if diagonal is winnable for character x
int diagCheck(GAME_STATE state, char x, int diag){
    char alt = (x=='X'?'O':'X');
    if(diag==1)
    {
        for(int i=0; i<3; i++)
        {
            if(state->board[i][i]==alt)
                return 0;
        }
    } else {
        for(int i=0; i<3; i++)
        {
            if(state->board[i][2-i]==alt)
                return 0;
        }
    }
    return 1;
}

// Calculating evaluation function - this function works with 
// respect to 'X'
int evaluateState(GAME_STATE state) {
    // Check X win
    int xwin = checkWin(state,'X');
    if(xwin)
        return 1e9;

    // Check O win
    int owin= checkWin(state, 'O');
    if(owin)
        return -1e9;

    // State evaluation
    int visx[8], viso[8];
    for(int i=0; i<8; i++){
        visx[i]=0;
        viso[i]=0;
    }

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(state->board[i][j]=='X') {
                // Checking for empty row
                visx[i]+=rowCheck(state,'X',i);

                // Checking for empty column
                visx[j+3]+=colCheck(state,'X',j);

                // Checking for empty diagonal
                if(i==j)
                    visx[6]+=diagCheck(state,'X',1);
                if(i+j==2)
                    visx[7]+=diagCheck(state,'X',2);
            }
            else if(state->board[i][j]=='O') {
                // Checking for empty row
                viso[i]+=rowCheck(state,'O',i);

                // Checking for empty column
                viso[j+3]+=colCheck(state,'O',j);

                // Checking for empty diagonal
                if(i==j)
                    viso[6]+=diagCheck(state,'O',1);
                if(i+j==2)
                    viso[7]+=diagCheck(state,'O',2);
            }
        }
    }

    int x_score = 0, o_score = 0;
    for(int i=0; i<8; i++){
        x_score += (visx[i]>0);
        o_score += (viso[i]>0);
    }
    return (x_score-o_score);

}

// Generating the game tree
GAME_STATE* makeTree(GAME_STATE *root, int lookAhead) {
    GAME_STATE curr = *root;
    if(lookAhead==0)
        return root;

    // Flipping turn from the previous move
    int next_turn = 1 - (curr->turn);
    // Choosing the character to be filled based on the new turn value
    char ins = (next_turn==1?'O':'X');

    // Creating new states by filling the blank spaces with the character
    // of the next player
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(curr->board[i][j]=='_') {
                GAME_STATE new_state = initState();
                // Copying the board
                memcpy(new_state,curr,sizeof(*curr));
                // Modifying one blank space
                new_state->board[i][j]=ins;
                new_state->turn=next_turn;
                new_state->child=NULL;
                new_state->next=NULL;

                int val = evaluateState(new_state);
                GAME_STATE *subtreeRoot;
                // Checking for terminal state and pruning the tree
                if(val==1e9||val==-1e9)
                    subtreeRoot = &new_state;
                // If new state is not terminal, building subtree through a recursive call
                else
                    subtreeRoot = makeTree(&new_state,lookAhead-1);
                
                // Placing new subtree in appropriate location
                // If current state does not have a child, add new subtree as child
                if(curr->child==NULL)
                    curr->child=(*subtreeRoot);
                // Else traverse the child list through next pointers and add new subtree 
                // at the end of the list
                else
                {
                    GAME_STATE q = curr->child;
                    GAME_STATE r = NULL;
                    while(q) {
                        r = q;
                        q = q->next;
                    }
                    r->next=(*subtreeRoot);
                }
            }
        }
    }
    return root;
}

// Finding the next best move
pair nextBestMove(GAME_STATE state, int startTurn, int step, int lookAhead)
{
    pair p;

    // If the current level is equal to the lookAhead value, return 
    // the state and either the value of the evaluation function or its
    // additive inverse based on whether the last move was X's or O's 
    // respectively.
    if(lookAhead==step) {
        p.state = state;
        p.evalVal = evaluateState(state);
        if(!startTurn)
            p.evalVal*=-1;
        return p;
    }
    // Take the maximum value of the evalVal of the states in the child list 
    // when the step is even
    if(step%2==0) {
        // Take max of children
        GAME_STATE q = state->child;
        int mx=-2e9;
        GAME_STATE mxState;
        while(q){
            pair ret = nextBestMove(q, startTurn, step+1, lookAhead);
            if (mx<ret.evalVal) {
                mx = ret.evalVal;
                mxState = q;
            }
            q=q->next;
        }
        p.evalVal=mx;
        p.state=mxState;
    }
    // Take the minimum value of the evalVal of the states in the child list 
    // when the step is odd 
    else {    
        // Take min of children
        GAME_STATE q = state->child;
        int mn=2e9;
        GAME_STATE mnState;
        while(q){
            pair ret = nextBestMove(q, startTurn, step+1, lookAhead);
            if (mn>ret.evalVal) {
                mn = ret.evalVal;
                mnState = q;
            }
            q=q->next;
        }
        p.evalVal=mn;
        p.state=mnState;
    }
    return p;
}

int main() {
    GAME_STATE state;
    printf("Enter the game state: \n");

    // Initialising board and taking input
    state = initState();

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            printf("Enter value at (%d, %d) (must be X, O or _): ", i, j);
            char v;
            scanf(" %c", &v);
            
            if(v!='X'&&v!='O'&&v!='_') {
                printf("Invalid entry! Try again.\n");
                j=j-1;
                continue;
                return 0;
            }
            state->board[i][j]=v;
        }
    }

    // Ensuring that number of X's is at most 1 more than number of 0's
    int x=0, o=0;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(state->board[i][j]=='X')
                x++;
            if(state->board[i][j]=='O')
                o++;
        }
    }

    // Assumption: X always starts
    if(x!=o&&x!=o+1){
        printf("Invalid state! Number of Xs should be equal or at max one more than the number of Os!");
        return 0;
    }

    if(x==o)
        state->turn=1;
    else
        state->turn=0;

    int cnt_empty = 0;

    // Displaying entered state
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            printf("%c ", state->board[i][j]);
            if(state->board[i][j]=='_')
                cnt_empty++;
        }
        printf("\n");
    }

    //Checking if one of the players has already won
    int eval = evaluateState(state);
    if(eval==1e9) {
        printf("X has already won.");
        free(state);
        return 0;
    } else if(eval==-1e9) {
        printf("O has already won.");
        free(state);
        return 0;
    } else if(cnt_empty==0) {
        printf("Tie.");
        free(state);
        return 0;
    }

    // Taking lookAhead value as input from the user
    int lookAhead;
    printf("Enter the lookahead value: ");
    scanf("%d", &lookAhead);

    // Ensuring lookAhead is at max the number of empty spaces
    if(lookAhead > cnt_empty)
        lookAhead = cnt_empty;

    // Generating the game tree
    GAME_STATE *root = makeTree(&state,lookAhead);

    // Getting the next best state for the current player
    pair move = nextBestMove(*root,state->turn,0,lookAhead);
    GAME_STATE nextBestState = move.state;

    printf("The most favourable next state for the current player based on the lookahead: \n");
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            printf("%c ", nextBestState->board[i][j]);
        }
        printf("\n");
    }
    return 0;
}
