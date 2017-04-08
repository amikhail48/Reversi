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
#ifndef _LED_H_
    #define _LED_H_
    
    uint8 red[BOARDSIZE][BOARDSIZE];
    uint8 green[BOARDSIZE][BOARDSIZE];
    uint8 blue[BOARDSIZE][BOARDSIZE];
    uint8 gameBoard[BOARDSIZE][BOARDSIZE];
    
    void initializeArray();

    void refreshLED();
    
    void drawBoard();
    
    void moveCursor(int dir);
    
    void cursorInc(int x, int y);
    
    void cursorDec(int x, int y);
    
    void resetCursor();
    
    void setPassFlag();
    
    int getPassFlag();
    
    void setCurPos(int r, int c);
    
    int getCurXPos();
    
    int getCurYPos();
    
    void resetSkipCount();
    
#endif 
/* [] END OF FILE */
