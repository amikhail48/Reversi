/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include "Game.h" 
#include "LED.h"
#include "myUART.h"

int i, j, k, l, temp;
int userColor = P2, oppColor = P1;
int dir;
int r, c;
int turn = 0;
int state, onCount = 0, gameOver = 0;
int P1_points, P2_points;

void initGameBoard(){
    for(i=0;i<BOARDSIZE;i++){
        for(j=0;j<BOARDSIZE;j++){
            gameBoard[i][j] = 0;
        }
    }
    i = ((BOARDSIZE/2)-1)%BOARDSIZE;
    j = (BOARDSIZE/2)%BOARDSIZE;
    
    gameBoard[i][i] = BLUE_DISC;
    gameBoard[j][j] = BLUE_DISC;
    gameBoard[i][j] = RED_DISC;
    gameBoard[j][i] = RED_DISC;
    green[0][0] = 1;
    turnOnDisc();
//    state = CURRENT;
    if(userColor == P1) switchTurn(); //Red goes first
}

void setPlayerColor(){
//    switchTurn();
    if(getGameState() == USERBLUE){
        userColor = P1;
        oppColor = P2;
    }
    if(getGameState() == USERRED){
        userColor = P2;
        oppColor = P1;
    }
}

//TEST FUNCTION
void clearBoard(){
    for(i=0;i<BOARDSIZE;i++){
        for(j=0;j<BOARDSIZE;j++){
            gameBoard[i][j] = 0;
        }
    }
}

void turnOnDisc(){
    for(i=0;i<BOARDSIZE; i++){
        for(j=0;j<BOARDSIZE;j++){
            red[i][j] = 0;
            blue[i][j] = 0;
        }
    }
    for(i=0;i<BOARDSIZE;i++){
        for(j=0;j<BOARDSIZE;j++){
            if(gameBoard[i][j] == BLUE_DISC) blue[i][j] = 1;
            else if(gameBoard[i][j] == RED_DISC) red[i][j] = 1;
        }
    }
}
void checkPoints(){
    P1_points = 0, P2_points =0;
    for(i=0;i<BOARDSIZE;i++){
        for(j=0;j<BOARDSIZE;j++){
            if(blue[i][j] == 1) P1_points++;
            else if(red[i][j] == 1) P2_points++;
        }
    }
}
    
int getP1points(){
    return P1_points;
}

int getP2points(){
    return P2_points;
}

void updateBoard(){
    for(i=0;i<BOARDSIZE;i++){
        for(j=0;j<BOARDSIZE;j++){
            if(blue[i][j] == 1) gameBoard[i][j] = BLUE_DISC;
            else if(red[i][j] == 1) gameBoard[i][j] = RED_DISC;
        }
    }
}

void submitTurn(int row, int col){
    r = row; c = col;
    if((row == 69 && col == 69) || getPassFlag()) switchTurn(); //Skipped turn
    if(validMove(row, col)){
        if(userColor == P1) { //Valid Move && Blue's turn
            changeColor(row, col);
            blue[row][col] = 1;
        }
        else{ //Valid Move && red's turn
            changeColor(row, col);
            red[row][col] = 1;
        }
        resetSkipCount();
        checkPoints();
        switchTurn();
        updateBoard();
    }
    else if(!validMove(row, col)){ //Invalid move
        otherGameOver();
        if(checkGameOver()){
            checkPoints();
            //printScore();
        }
      else{
//        LCD_PrintString("Invalid");
////        while(USBUART_CDCIsReady() == 0u){};
////        USBUART_PutString("Invalid Move !");
      }
//        blue[10][3] = 1;
//        CyDelay(5);
//        blue[10][3] = 0;
    }
}

void switchTurn(){
    //resetCursor();
    turn++;
    //if(userColor == P1)
    generateTXPacket();
    temp = oppColor;
    oppColor = userColor ;
    userColor = temp;

    if(getGameState() == USERBLUE) setGameState(USERRED);
    else setGameState(USERBLUE);
    
    //if(state == CURRENT) state = IDLE;
    //if(state == IDLE) state = CURRENT;
}

int getTurn(){
    return turn;
}

void changeColor(int row, int col){
    //TEST FUNCTION
    /*
    //Check Left
    if(col > 1){
        if(gameBoard[row][col-1] == oppColor){
            for(i=col-2;i>=0;i--){
                if(gameBoard[row][i] == userColor){
                    for(j=i+1;j<col;j++){
                        gameBoard[row][j] = userColor;
                    }
                    break;
                }
            }
        }
    }
    //Check Right
    if(col < BOARDSIZE-3){
        if(gameBoard[row][col+1] == oppColor){
            for(i=col+2;i<BOARDSIZE;i++){
                if(gameBoard[row][i] == userColor){
                    for(j=i-1;j<col;j--){
                        gameBoard[row][j] = userColor;
                    }
                    break;
                }
            }
        }
    }
    //Check Up
    if(row > 1){
        if(gameBoard[row-1][col] == oppColor){
            for(i=row-2;i>=0;i--){
                if(gameBoard[i][col] == userColor){
                    for(j=i+1;j<row;j++){
                        gameBoard[j][col] = userColor;
                    }
                    break;
                }
            }
        }
    }
    //Check Down
    if(row < BOARDSIZE-3){
        if(gameBoard[row+1][col] == oppColor){
            for(i=row+2;i<BOARDSIZE;i++){
                if(gameBoard[i][col] == userColor){
                    for(j=i-1;j<row;j--){
                        gameBoard[j][col] = userColor;
                    }
                    break;
                }
            }
        }
    }
    //Check top right diagonal
    if(row>1 && col<BOARDSIZE-3){
        if(gameBoard[row-1][col+1] == oppColor){
            i=row-2, j=col+2;
            while(i>=0 && j<BOARDSIZE){
                if(gameBoard[i][j] == userColor){
                    k=i+1, l=j-1;
                    while(k<row && l>col){
                        gameBoard[k][l] = userColor;
                        k++; l--;
                    }
                    break;
                }
                i--; j++;
            }
        }       
    }
    //Check top left diagnoal
    if(row>1 && col>1){
        if(gameBoard[row-1][col-1] == oppColor){
            i=row-2, j=col-2;
            while(i>=0 && j>=0){
                if(gameBoard[i][j] == userColor){
                    k=i+1, l=j+1;
                    while(k<row && l<col){
                        gameBoard[k][l] = userColor;
                        k++; l++;
                    }
                    break;
                }
                i--; j--;
            }
        }
    }
    //Check bottom right diagnonal
    if(row<BOARDSIZE-3 && col<BOARDSIZE-3){
        if(gameBoard[row+1][col+1] == oppColor){
            i=row+2, j=col+2;
            while(i<BOARDSIZE && j<BOARDSIZE){
                if(gameBoard[i][j] == userColor){
                    k=i-1, l=j-1;
                    while(k>row && l>col){
                        gameBoard[k][l] = userColor;
                        k--; l--;
                    }
                    break;
                }
                i++; j++;
            }
        }
    }
    //Check bottom left diagonal
    if(row<BOARDSIZE-3 && col>1){
        if(gameBoard[row+1][col-1] == oppColor){
            i=row+2, j=col-2;
            while(i<BOARDSIZE && j>=0){
                if(gameBoard[i][j] == userColor){
                    k=i-1, l=j+1;
                    while(k<row && l>col){
                        gameBoard[k][l] = userColor;
                        k--; l++;
                    }
                    break;
                }
                i++; j--;
            }
        }
    }*/
    //Change Up
    if(dir & up){
        for(i=1;gameBoard[row-i][col] == oppColor;i++){
            gameBoard[row-i][col] = userColor;
        }
    }
    //Change Down
    if(dir & down){
        for(i=1;gameBoard[row+i][col] == oppColor;i++){
            gameBoard[row+i][col] = userColor;
        }
    }
    //Change Right
    if(dir & right){
        for(i=1;gameBoard[row][col+i] == oppColor;i++){
            gameBoard[row][col+i] = userColor;
        }
    }
    //Change Left
    if(dir & left){
        for(i=1;gameBoard[row][col-i] == oppColor;i++){
            gameBoard[row][col-i] = userColor;
        }
    }
    //Change Top Right Diagonal
    if(dir & TOPRIGHT){
        for(i=1;gameBoard[row-i][col+i] == oppColor;i++){
            gameBoard[row-i][col+i] = userColor;
        }
    }
    //Change Top Left Diagonal
    if(dir & TOPLEFT){
        for(i=1;gameBoard[row-i][col-i] == oppColor;i++){
            gameBoard[row-i][col-i] = userColor;
        }
    }
    //Change Bottom Right Diagonal
    if(dir & BOTTOMRIGHT){
        for(i=1;gameBoard[row+i][col+i] == oppColor;i++){
            gameBoard[row+i][col+i] = userColor;
        }
    }
    //Change Bottom Left Diagonal
    if(dir & BOTTOMLEFT){
        for(i=1;gameBoard[row+i][col-i] == oppColor;i++){
            gameBoard[row+i][col-i] = userColor;
        }
    }
    dir = 0b00000000;
    turnOnDisc();
}

int validMove(int row, int col){
    int valid = 0;
    if(row > LOWERBOUND) return 0; //Check if Move is in bounds 
    if(row < UPPERBOUND) return 0;
    if(col > LOWERBOUND) return 0;
    if(col < UPPERBOUND) return 0;
    
    if(gameBoard[row][col] != 0) return 0; //Check if space is empty
    
    //Check Left
    if(col > 1){
        if(gameBoard[row][col-1] == oppColor){
            for(i=col-2;i>=0;i--){
                if(gameBoard[row][i] == userColor){
                    dir |= left;
                    valid = 1;
                }
            }
        }
    }
    //Check Right
    if(col <= BOARDSIZE-3){
        if(gameBoard[row][col+1] == oppColor){
            for(i=col+2;i<BOARDSIZE;i++){
                if(gameBoard[row][i] == userColor){
                    dir |= right;
                    valid = 1;
                }
            }
        }
    }
    //Check Up
    if(row > 1){
        if(gameBoard[row-1][col] == oppColor){
            for(i=row-2;i>=0;i--){
                if(gameBoard[i][col] == userColor){
                    dir |= up;
                    valid = 1;
                }
            }
        }
    }
    //Check Down
    if(row <= BOARDSIZE-3){
        if(gameBoard[row+1][col] == oppColor){
            for(i=row+2;i<BOARDSIZE;i++){
                if(gameBoard[i][col] == userColor){
                    dir |= down;
                    valid = 1;
                }
            }
        }
    }
    //Check top right diagonal
    if(row>1 && col<=BOARDSIZE-3){
        if(gameBoard[row-1][col+1] == oppColor){
            i=row-2, j=col+2;
            while(i>=0 && j<BOARDSIZE){
                if(gameBoard[i][j] == userColor){
                    dir |= TOPRIGHT;
                    valid = 1;
                }
                i--; j++;
            }
        }       
    }
    //Check top left diagnoal
    if(row>1 && col>1){
        if(gameBoard[row-1][col-1] == oppColor){
            i=row-2, j=col-2;
            while(i>=0 && j>=0){
                if(gameBoard[i][j] == userColor){
                    dir |= TOPLEFT;
                    valid = 1;
                }
                i--; j--;
            }
        }
    }
    //Check bottom right diagnonal
    if(row<=BOARDSIZE-3 && col<=BOARDSIZE-3){
        if(gameBoard[row+1][col+1] == oppColor){
            i=row+2, j=col+2;
            while(i<BOARDSIZE && j<BOARDSIZE){
                if(gameBoard[i][j] == userColor){
                    dir |= BOTTOMRIGHT;
                    valid = 1;
                }
                i++; j++;
            }
        }
    }
    //Check bottom left diagonal
    if(row<=BOARDSIZE-3 && col>1){
        if(gameBoard[row+1][col-1] == oppColor){
            i=row+2, j=col-2;
            while(i<BOARDSIZE && j>=0){
                if(gameBoard[i][j] == userColor){
                    dir |= BOTTOMLEFT;
                    valid = 1;
                }
                i++; j--;
            }
        }
    }
    if(valid == 1) return 1;
    return 0;
}

void setGameOver(int g){
    gameOver = g;
}

void otherGameOver(){
    int one = getP1points();
    int two = getP2points();
    if(one == 0) setGameOver(1);
    if(two == 0) setGameOver(1);
    for(i=0;i<BOARDSIZE;i++){
        for(j=0;j<BOARDSIZE;j++){
            if(gameBoard[i][j] != P1 || gameBoard[i][j] != P2) onCount++;
        }
    }
    if(onCount == 0) setGameOver(1);
        
     //Check if No more moves available
        if(!validMove(r,c)){
            userColor = P1;
            oppColor = P2;
            if(!validMove(r,c)) gameOver = 1;
            else{ 
                userColor = P2;
                oppColor = P1;
            }
        }
//TEST FUNCTION
//    //Check if No more moves available
//    for(i=0;i<BOARDSIZE;i++){
//        for(j=0;j<BOARDSIZE;j++){
//            if(gameBoard[i][j] == userColor || gameBoard[i][j] == oppColor) onCount++;
//        }
//    }
//    if(onCount == (BOARDSIZE^2) || onCount == (BOARDSIZE^2)-1) setGameOver(1);   
}

int checkGameOver(){
    return gameOver;
}

void setGameState(int s){
    state = s;
}

int getGameState(){
    return state;
}

/* [] END OF FILE */
