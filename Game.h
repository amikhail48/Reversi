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
#ifndef _GAME_H_
    #define _GAME_H_
    
//  #define playerID    "Andrew"
    #define START_GAME  10
    #define NETWORK_CONNECT 11 
    #define USERBLUE    12
    #define USERRED    13
    #define END_GAME    14
    
    #define UPPERBOUND  0
    #define LOWERBOUND  BOARDSIZE-1
    #define BOARDSIZE   16
    
    #define USBFS_DEVICE    (0u)
    #define USBUART_BUFFER_SIZE (1u)
    
    #define HOME 0
    #define UP  1
    #define DOWN    2
    #define RIGHT   3
    #define LEFT    4
    #define ENTER   5
    #define SKIP    6
    
    #define IDLE    8
    #define CURRENT 10
    
    #define up  0b00000001
    #define down    0b00000010
    #define right   0b00000100
    #define left    0b00001000
    #define TOPRIGHT    0b00010000
    #define TOPLEFT     0b00100000
    #define BOTTOMRIGHT   0b01000000
    #define BOTTOMLEFT    0b10000000
    
    #define BLUE_DISC   1
    #define RED_DISC    2
    
    #define P1  BLUE_DISC
    #define P2  RED_DISC
        
    void initGameBoard();
    
    void setPlayerColor();
    
    void turnOnDisc();
    
    void updateBoard();
    
    void submitTurn(int r, int c);
    
    int getTurn();
    
    void switchTurn();
    
    void changeColor(int r, int c);
    
    int validMove();
    
    void checkPoints();
    
    int getP1points();
    
    int getP2points();
    
    void clearBoard();
    
    void setGameOver(int x);
    
    int checkGameOver();
    
    void otherGameOver();
    
    void setGameState(int x);
    
    int getGameState();
    
#endif
    
/* [] END OF FILE */
