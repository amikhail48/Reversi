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
#include "FS.h"
#include "myUART.h" 
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

char playerID[8] = "AndrewsP";
uint8 txPacket[lengthOfPacket];
uint8 rxPacket[lengthOfPacket];
uint8 txWifiPacket[26] = "data 00000000000000000000\n";

int i, j, c=0, k=0;
int txCount = 0, rxCount = 0;
int error = 0;
int state = start;
int sendFlag = 0, ind = 0;

uint8 sequence[3] = {0};
uint8 row[2], col[2];
uint8 pass[1];

char oppID[8], oppSTemp[3], oppOTemp[1];
uint8 oppTemp[3];
int oppSeq, PrevoppSeq, oppPass, oppRow, oppCol;

/*============WIFI VARIABLES====================*/
char wifi_forward[128];
char ad[] = "advertise \n";
char connect[] = "connect 192.168.0.112\n";
int w_flag = 0, w = 0, wp = 0, wr2p, advertise = 0;
typedef struct IP IP;
struct IP{
    uint a;
    uint b;
    uint c;
    uint d;
};
IP homeIP = {0,0,0,0};
IP oppIP = {0,0,0,0};
char network_buffer[128];
/*==========================================*/

void generateTXPacket(){
    j = 0;
    /*Format Bytes */
    int turn = getTurn();
    sprintf(sequence, "%03d", turn%256);
    int passFlag = getPassFlag();
    sprintf(pass, "%01d", passFlag);

    int r = getCurXPos() + 1;
    int c = getCurYPos() + 1; 

    sprintf(row, "%02d", r);
    sprintf(col, "%02d", c);

    /*Packet*/
    txPacket[j++] = 0x55;
    txPacket[j++] = 0xaa;
    for(i = 0; i < 8; i++) {
        txPacket[j++] = playerID[i];
    }
    txPacket[j++] = 0x20;
    txPacket[j++] = sequence[0];
    txPacket[j++] = sequence[1];
    txPacket[j++] = sequence[2];
    txPacket[j++] = pass[0];
    txPacket[j++] = row[0];
    txPacket[j++] = row[1];
    txPacket[j++] = col[0];
    txPacket[j] = col[1];
    
    //sendFlag = 1;
    //for(i=0;i<lengthOfPacket;i++){
    //UART_PutChar(txPacket[i]);
    //}
    //clearBoard();

// TRANSMIT MY PACKET OVER WIFI    
//    for(k=0;k<lengthOfPacket;k++){
//        txWifiPacket[k+5] = txPacket[k];
//    }
//    txWifiPacket[25] = '\n';
}

void checkRXPacket(int i){
//  i = 0;
    state = start;    
    switch(state){
        case start :
            if(rxPacket[i] == 0x55){
                i++;
                state = start2;
            }else{
                //LCD_PrintString("Invalid Move");
                break;
            }
        case start2 :
            if(rxPacket[i] == 0xaa){
                i++; 
                state = checkID;
            }else{
                //LCD_PrintString("Invalid Move");
                break;
            }
        case checkID :
            j = 0;
            while(rxPacket[i] != 0x20){ //Receive  Player ID
                oppID[j] = rxPacket[i];
                i++; j++;
            }
            //else{
                //LCD_PrintString(oppID);
                state = space;
            //}
        case space :
            if(rxPacket[i] == 0x20){
                i++;
                state = getSeq;
            }else{
                //LCD_PrintString("Invalid Move");
                break;
            }
        case getSeq :
            //LCD_PrintString("YES");
            if((int)rxPacket[i] >= 0 && (int)rxPacket[i] <= 256){ //Receive sequence
                for(j=0;j<3;j++){
                    oppTemp[j] = rxPacket[i++];
                }
                sprintf(oppSTemp, "%d%d%d",oppTemp[0] % 0x30, oppTemp[1] % 0x30, oppTemp[2] % 0x30);
                oppSeq = atoi(oppSTemp);
                if(oppSeq == PrevoppSeq) break;
                PrevoppSeq = oppSeq;
                //LCD_PrintNumber(oppSeq);
                //LCD_PrintInt8
                state = checkPass;
            }else{
                //LCD_PrintString("Invalid Move");
                break;
            }
        case checkPass :
            //LCD_PrintString(" WEMADEIT ");
            if(rxPacket[i] == 0x30 || rxPacket[i] == 0x31){ //Receive Passflag
                oppTemp[0] = rxPacket[i++];
                sprintf(oppOTemp, "%d", oppTemp[0] % 0x30);
                oppPass = atoi(oppOTemp);
                setPassFlag(oppPass);
                //LCD_PrintString(" WEMADEIT ");
                //LCD_PrintNumber(oppPass);
                state = checkRow;
            }else{
                //LCD_PrintString("Invalid Move");
                break;
            }
        case checkRow :
            //LCD_PrintString("Almost");
            if(rxPacket[i] < 0x38){ //Receive row
                for(j=0;j<2;j++){
                    oppTemp[j] = rxPacket[i++];
                }
                //LCD_PrintString("Almost");
                sprintf(oppOTemp, "%d%d", oppTemp[0] % 0x30, oppTemp[1] % 0x30);
                oppRow = atoi(oppOTemp);
                //LCD_PrintNumber(oppRow);
                state = checkCol;
            }else{
                //LCD_PrintString("Invalid Move");
                break;
            }
        case checkCol :
            if(rxPacket[i] < 0x38){ //Receive col
                for(j=0;j<2;j++){
                    oppTemp[j] = rxPacket[i++];
                }
                sprintf(oppOTemp, "%d%d", oppTemp[0] % 0x30, oppTemp[1] % 0x30);
                oppCol = atoi(oppOTemp);
                //LCD_PrintString("Correct Packet");
                //LCD_Position(0,1);
                //LCD_PrintNumber(oppCol);
                submitTurn(oppRow-1, oppCol-1);
                sendFlag = 1;
                state = start;
            }else{
                //LCD_PrintString("Invalid Move");
                break;
            }
        } 
    /*
    //TEST FUNCTION
    i=0;
    rxPacket[0] = 0x55;
    rxPacket[1] = 0xaa;
    if(receive[i++] != 0x55) LCD_PrintString("Invalid Move");
    if(receive[i++] != 0xaa) LCD_PrintString("Invalid Move");
    for(j=0;j<8;j++){
        if(receive[i] != 0x20){
            receive[i] = rxPacket[i];
            i++;
        }
    }
    if((int)receive[i] < 0 || (int)receive[i] > 256) LCD_PrintString("Invalid");
    else receive[i] = rxPacket[i]; 
    //rxPacket[i] = oppSeq;
    i++;
    receive[i] = rxPacket[i]; //Receive Passflag
    //rxPacket[i] = oppPass;
    i++;
    receive[i] = rxPacket[i]; //Receive row high
    //rxPacket[i] = oppRow;
    i++;
    receive[i] = rxPacket[i]; //Receive row low
    i++;
    receive[i] = rxPacket[i]; //Receive col high
    //rxPacket[i] = oppCol;
    i++;
    receive[i] = rxPacket[i]; //Receive col low     
    */
}

void txFIFOEmpty(){
//  int networkBufferFlag = 0;    
    /*Send packet to UART*/
    //if(sendFlag){
    if(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL){
//        if(getGameState() == NETWORK_CONNECT){
//            if(networkBufferFlag){
//              for(k=0;k<n;k++){
//              //while (0u == USBUART_CDCIsReady()){}
//                  UART_PutChar(network_buffer[k]);
//              }
//              for(k=0;k<strlen(connect);k++){
//                  while (0u == USBUART_CDCIsReady()){}
//                  UART_PutChar(connect[k]);
//              }
//              UART_PutChar('\n');
//              n = 0;
//              networkBufferFlag = 0;
//          }
//          if(advertise){
//             for(i=0;i<strlen(ad);i++){
//                 while (0u == USBUART_CDCIsReady()){}
//                 USBUART_PutString("Advertising")
//                 UART_PutChar(ad[i]);
//          }
//          USBUART_PutString("Advertising...\n\n");
//          advertise = 0;
//          }
//       }
/*================================================*/
//        TEST TRANSMIT OF PACKET
//        for(i=0;i<4;i++){
//            UART_PutChar(txPacket[ind++]);
//            if(ind == lengthOfPacket){
//                ind = 0;
//            }
//        }
//    }
 /*================================================*/

//        else{
            UART_PutArray(txPacket, lengthOfPacket);
//        }
    }
    //UART_PutString(txPacket);
    FiveMS_Write(0);
    //sendFlag = 0;
    //memcpy(UART_TxBuffer, txPacket, lengthOfPacket);
    //}
}

void rxByteReceived(){
    if(UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY){
//        if(getGameState() != NETWORK_CONNECT){
            rxPacket[c] = UART_GetByte();
            //checkRXPacket(c);
            c++;
            if(c == 19){
                c=0;
                checkRXPacket(0);
            }
//        }else{
//            wifi_forward[w] = UART_GetByte();
//            if(wifi_forward[w] == 0xA){
//                wifi_forward[w+1] = NULL;
//                w = 0;
//                wr2p = 1;
//            }
//        }
    }
}

// Wifi and Network
void setNetwork(){
//    if(wr2p){ //Wifi Ready to Play
//        //isr4_Disable();
//        //LCD_Position(0,0); LCD_PrintString(wifi_forward);
//        while (0u == USBUART_CDCIsReady());
//        USBUART_PutString(wifi_forward);
//        while (0u == USBUART_CDCIsReady()){}
//        USBUART_PutString("\r\n");
//        while (0u == USBUART_CDCIsReady()){}
//        USBUART_PutData(NULL, 0);
//        char identifyIP[] = "IP Address: ";
//        char identifyCon[] = "Connected to";
//        char *found = strstr(wifi_forward, identifyIP);
//        char *connected = strstr(wifi_forward, identifyCon);
//        char *ad_done = strstr(wifi_forward,"OK");
//        if(ad_done != NULL) advertise = 0;
//        if(found != NULL){
//            sscanf(found, "IP Address: %u.%u.%u.%u\n", &homeIP.a, &homeIP.b, &homeIP.c, &homeIP.d);
//            advertise = 1;
//        }
//        if(connected != NULL){
//            sscanf(connected, "Connected to %u.%u.%u.%u\n", &oppIP.a, &oppIP.b, &oppIP.c, &oppIP.d);
//            if(oppIP.d){
//                if(homeIP.d<oppIP.d) setGameState() = HOME;
//                else setGameState() = AWAY;
//            }
//        }
//        LCD_Position(0,0); LCD_PrintNumber(homeIP.a); LCD_PrintString("."); LCD_PrintNumber(homeIP.b);
//        LCD_PrintString("."); LCD_PrintNumber(homeIP.c); LCD_PrintString("."); LCD_PrintNumber(homeIP.d);
//        LCD_Position(1,0); LCD_PrintNumber(oppIP.a); LCD_PrintString("."); LCD_PrintNumber(oppIP.b);
//        LCD_PrintString("."); LCD_PrintNumber(oppIP.c); LCD_PrintString("."); LCD_PrintNumber(oppIP.d);
//        wr2p=0;
//        //isr4_Enable();
//    }
}

void sendMessage(){
//TEST FUNCTION
//    while(USBUART_CDCIsReady() == 0u){};
//    USBUART_PutString("Player: ");
//    while(USBUART_CDCIsReady() == 0u);
//    USBUART_PutString(txPacket);
//    USBUART_PutData(row, 2);
//    while(USBUART_CDCIsReady() == 0u);
//    USBUART_PutCRLF();
//    while(USBUART_CDCIsReady() == 0u);
//    USBUART_PutString("\r\n");
//    USBUART_PutString("RX:");
//    if(getGameState() == AWAY){
    while(USBUART_CDCIsReady() == 0u);
    USBUART_PutData(rxPacket, lengthOfPacket);
//    USBUART_PutString(oppID);
    while (0u == USBUART_CDCIsReady()){}
    USBUART_PutCRLF();
    while (0u == USBUART_CDCIsReady()){}
    USBUART_PutData(NULL, 0);
//    }
}

void printScore(){
    int one = getP1points();
    int two = getP2points();
    if(checkGameOver() == 1){
        LCD_ClearDisplay();
        if(one == two) LCD_PrintString("TIE GAME !!");
        if(one > two) LCD_PrintString("Player 1 Wins !!");
        if(two > one) LCD_PrintString("Player 2 Wins !!");
//    }else if(two == 0 && getGameState() != START_GAME){
//        LCD_ClearDisplay();
//        LCD_PrintString("Player 1 Wins !!");
//    }else if(one == 0 && getGameState() != START_GAME){
//        LCD_ClearDisplay();
//        LCD_PrintString("Player 2 Wins !!");
    }else{
        LCD_ClearDisplay();
        LCD_PrintString("P1:");
        LCD_PrintNumber(one);
        LCD_Position(1,0);
        LCD_PrintString("P2:");
        LCD_PrintNumber(two);
    }
    CyDelay(2);
}

int checkWriteFlag(){
    return sendFlag;
}

void writeToFile(FS_FILE * targetFile, int home){
    char output[256];
    checkPoints();
    if(home){
    //if(P1){
        if(getPassFlag()){
            sprintf(output, "Player %d, %s, has passed.\r\nP1: %d P2: %d\r\n\n", P1, playerID, getP1points(), getP2points());
        }else{
            sprintf(output, "Player %d, %s, has placed a move at [%d][%d].\r\nP1: %d P2: %d\r\n\n", P1, playerID, (getCurXPos()+1), (getCurYPos()+1), getP1points(), getP2points());
        }
    }else{
        if(oppPass){
            sprintf(output, "Player %d, %s, has passed.\r\nP1: %d P2: %d\r\n\n", P2, oppID, getP1points(), getP2points());
        }else{
            sprintf(output, "Player %d, %s, has placed a move at [%d][%d].\r\nP1: %d P2: %d\r\n\n", P2, oppID, oppRow, oppCol, getP1points(), getP2points());
        }
    }
    while(USBUART_CDCIsReady() == 0u){}
    USBUART_PutString(output);
    //LCD_PrintNumber(getGameState());
    FS_Write(targetFile, output, strlen(output));
    //FS_FClose(targetFile);
    sendFlag = 0;
//    if(targetFile){   
//        if(0 != FS_Write(targetFile, output, strlen(output))){
//            /* Inditate that data was written to a file */
//            //FS_Write(targetFile, output, 256);
//            LCD_PrintString("File write succeeded");
//        }
//        else{
//            LCD_PrintString("File write failed");
//        }
//    } 
}
/* [] END OF FILE */
