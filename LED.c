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
   
int i, j, k, temp = 0, temp2 = 0;
uint8 color, color2;
int modNum, R = 0;
int r, c, flag, row, col, skipFlag = 0;
int skipCount = 0;

void initializeArray(){
    for(i=0; i<BOARDSIZE; i++){
        for(j=0; j<BOARDSIZE; j++){
            red[i][j] = 0;
            green[i][j] = 0;
            blue[i][j] = 0;
        }
    }
}    

void refreshLED(){
    if(BOARDSIZE == 16 || BOARDSIZE == 8){
        modNum = 8;
    }else{
        modNum = 4;
    }
    i = R++ %modNum;
    outputEnable_Write(1); /* Clear display */
    rowSelect_Write(i);
    for(j=0; j<BOARDSIZE; j++){
        color = 0b000;
        color2 = 0b000;
        if(red[i][j]) color |= 0b001;
        if(red[i+8][j]) color2 |= 0b001;
        if(green[i][j]) color |= 0b010;
        if(green[i+8][j]) color2 |= 0b010;
        if(blue[i][j]) color |= 0b100;
        if(blue[i+8][j]) color2 |= 0b100;
        color_Write(color);
        if(BOARDSIZE < 16) color2 = 0b000;
        color2_Write(color2);
        
        clock_Write(0b1);
        clock_Write(0b0);
    }
    color_Write(0b000); /*Blank out rest of columns*/    
    color2_Write(0b000);
    for(k = 32; k > BOARDSIZE; k--){ /*Clock high to low for rest of columns*/
        clock_Write(0b1);
        clock_Write(0b0);
    }
    latch_Write(0b1);
    latch_Write(0b0);
    outputEnable_Write(0);
}
//TEST FUNCTION
//void drawBoard(){
//    BOARDSIZE = 4 //8 //16
//    i = ((BOARDSIZE/2)-1)%BOARDSIZE;
//    j = (BOARDSIZE/2)%BOARDSIZE;
//    
//    blue[i][i] = 1;
//    blue[j][j] = 1;
//    red[i][j] = 1;
//    red[j][i] = 1;
//}

void moveCursor(int dir){
    //setPassFlag(skipFlag);
    for(i=0;i<BOARDSIZE;i++){
        for(j=0;j<BOARDSIZE;j++){
            green[i][j]=0;
        }
    }
    if(dir == UP) { //Arrow Key: Up
        if(row - 1 >= 0) row = row - 1;
    }
    else if(dir == DOWN) { //Arrow Key: Down
        if(row + 1 < BOARDSIZE) row = row + 1;
    }
    else if(dir == RIGHT) { //Arrow Key: Right
        if(col + 1 < BOARDSIZE) col = col + 1;
    }
    else if(dir == LEFT) { //Arrow Key: Left
        if( col - 1 >= 0 ) col = col - 1;
    }
    else if(dir == HOME){ //H key: Home
        row = 0;
        col = 0;
    }
    else if(dir == ENTER){ //Enter
        if(skipFlag == 0){
            setCurPos(row, col);
            submitTurn(row, col);   
        }else if(skipFlag == 1) {
            setPassFlag(skipFlag);
            submitTurn(69, 69);
            skipFlag = 0;
        }
    }
    else if(dir == SKIP){ //Skip
        skipFlag = 1;    
    }
    green[row][col] = 1;
    setCurPos(row, col);
}

void setCurPos(int row, int col){
    r = row;
    c = col;
}

int getCurXPos(){
    return r;
}

int getCurYPos(){
    return c;
}

//TEST FUNCTION
//void cursorInc(int x, int y){
//    if( (temp+x)>= UPPERBOUND && (temp2+y)>=UPPERBOUND && (temp+x)<=LOWERBOUND && (temp2+y)<=LOWERBOUND ){
//        green[temp+x][temp2+y] = 1;
//        temp += x;
//        temp2 += y;
//        green[temp-x][temp2-y] = 0;
//    }
//}

//TEST FUNCTION
//void cursorDec(int x, int y){
//    if( (temp-x)>=UPPERBOUND && (temp2-y)>=UPPERBOUND && (temp-x)<=LOWERBOUND && (temp2-y)<=LOWERBOUND ){
//        green[temp-x][temp2-y] = 1;
//        temp -= x;
//        temp2 -= y;
//        green[temp+x][temp2+y] = 0;
//    }
//}

void resetCursor(){
    for(i=0;i<BOARDSIZE;i++){
        for(j=0;j<BOARDSIZE;j++){
            green[i][j] = 0;
        }
    }
    row = 0; col = 0;
    green[row][col] = 1;
//    temp = 0;
//    temp2 = 0;
}

void resetSkipCount(){
    skipCount = 0;
}

void setPassFlag(int f){
    flag = f;
    skipCount++;
    if(skipCount == 2) setGameOver(1);
}

int getPassFlag(){
    return flag;
}
    
/* [] END OF FILE */
