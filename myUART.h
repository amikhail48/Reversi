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
#include "stdio.h"
#include "Game.h"
#include "LED.h"
#include "FS.h"
#ifndef _MYUART_H_
    #define _MYUART_H_
    
    #define lengthOfPacket  19
    
    #define start   9
    #define start2  8
    #define checkID 7
    #define space   6
    #define getSeq  5
    #define checkPass   4
    #define checkRow    3
    #define checkCol    2
    
    //uint8 txPacket[4096];
    //uint8 rxPacket[4096];
    
    void generateTXPacket();
    
    void checkRXPacket(int x);
    
    void txFIFOEmpty();
    
    void rxByteReceived();
    
    void sendMessage();
    
    void printScore();
    
    void setNetwork();
    
    int checkWriteFlag();
    
    void writeToFile(FS_FILE* x, int y);
        
#endif
/* [] END OF FILE */
