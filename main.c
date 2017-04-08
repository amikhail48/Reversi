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
#include "FS.h"
#include "Game.h"
#include "LED.h"
#include "myUART.h"

/*=========================================*/    
uint16 count;
int r, c, sendFlag;
uint8 Buffer[USBUART_BUFFER_SIZE];
char sdFile[9] = "log.txt";
char A;
/*=========================================*/    

CY_ISR(timerInterrupt){
   refreshLED();
}

CY_ISR(cursorDisplay){
//    r = getCurXPos();
//    c = getCurYPos();
//    green[r][c] = (!green[r][c])? 1:0;
}

CY_ISR(txInterrupt){
    //TEST INTERRUPT ENTRY
    //blue[0][1] = 1;
    //UART_PutChar('A');
    txFIFOEmpty();
}

CY_ISR(rxInterrupt){
    //TEST INTERRUPT ENTRY
    //red[1][1] = 1;
   rxByteReceived();
    //TEST RX RECEIVE 
    // A = UART_GetChar();
}

CY_ISR(sendPacketInterrupt){
    FiveMS_Write(1);
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /* Start USBFS operation with 3-V operation. */
    USBUART_Start(USBFS_DEVICE, USBUART_3V_OPERATION);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    initializeArray();
    Timer_Start();
    Timer2_Start();
    Timer3_Start();
    LCD_Start();
    UART_Start();
    isr_StartEx(timerInterrupt);
    isr2_StartEx(cursorDisplay);
    isr3_StartEx(txInterrupt);
    isr4_StartEx(rxInterrupt);
    isr5_StartEx(sendPacketInterrupt);
    initGameBoard();
    FS_Init();
    setGameState(START_GAME);

    //TEST TRANSMIT FUNCTION
    //UART_PutChar(0x55);
    
    // File System Initialization
    char sdVolName[10];     /* Buffer that will hold SD card Volume name */
    FS_FILE * pFile;
    if(0 != FS_GetVolumeName(0u, &sdVolName[0], 9u)){
        /* Getting volume name succeeded so prompt it on the LCD */
        LCD_PrintString("SD card name:");
        LCD_Position(1u, 0u);
        LCD_PrintString(sdVolName);
    }
    else{
        /* Operation Failed - indicate this */
        LCD_PrintString("Failed to get");
        LCD_Position(1u, 0u);
        LCD_PrintString("SD card name");
    }
//    /* Need some delay to indicate output on the LCD */
//    LCD_Position(0u, 0u);
//    LCD_PrintString("SD card format");
//    
//    if(0 == FS_FormatSD(sdVolName)){
//        LCD_Position(1u, 0u);
//        LCD_PrintString("Succeeded");
//    }
//    else{
//        LCD_Position(1u, 0u);
//        LCD_PrintString("Failed");
//    }
    LCD_Position(0u, 0u);
    FS_Mount(sdVolName);
    /* Create specific file for modification */
    pFile = FS_FOpen(sdFile, "w");
    
//  TEST SD CARD FUNCTION    
//    if(pFile) LCD_PrintString("Opened");
//    char output[3] = "Hi";
//    FS_Write(pFile, output, 3);
//    FS_FClose(pFile);
//    FS_Unmount(sdVolName);
        
    for(;;)
    {
        /* Place your application code here. */
        //drawBoard();
        //turnOnDisc();
        updateBoard();
        printScore();
        //r = getCurXPos();
        //c = getCurYPos();
        /* Host can send double SET_INTERFACE request. */
        if (0u != USBUART_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device is configured. */
            if (0u != USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint to receive data from host. */
                USBUART_CDC_Init();
            }
        }
        if(getGameState() == NETWORK_CONNECT){
            // Get output from Wi-Fi board
            setNetwork();
        }
        /* Service USB CDC when device is configured. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Check for input data from host. */
            if (0u != USBUART_DataIsReady())
            {
                /* Read received data and re-enable OUT endpoint. */
                count = USBUART_GetAll(Buffer);
                if(Buffer[0] == 0x1F) moveCursor(DOWN); //down
                if(Buffer[0] == 0x1E) moveCursor(UP); //up
                if(Buffer[0] == 0x1D) moveCursor(RIGHT); //right
                if(Buffer[0] == 0x1C) moveCursor(LEFT); //left
                if(Buffer[0] == 0x68) moveCursor(HOME); //home
                if(Buffer[0] == 0x0D) moveCursor(ENTER); //enter
                if(Buffer[0] == 0x73) moveCursor(SKIP); //pass: s and enter
                if(getGameState() == START_GAME){
                    if(Buffer[0] == 0x31){ //Pressed 1 for blue player
                        setGameState(USERBLUE);
                    }
                    else if(Buffer[0] == 0x32){ //Pressed 2 for red player
                        setGameState(USERRED);
                    }
                }
                if(Buffer[0] == 0x77){ //Pressed w to write to file
                    if(getGameState() == 0x05) writeToFile(pFile, 1);
                    else writeToFile(pFile, 0);
                }
                if(Buffer[0] == 0x78) FS_FClose(pFile); //Pressed x to close file

                //if (0u != count){
                    /* Wait until component is ready to send data to host. */
                    //while (0u == USBUART_CDCIsReady()){}

                    /* Send data back to host. */
                    //USBUART_PutData(Buffer, count);
                    sendMessage();

                    //while (0u == USBUART_CDCIsReady()){};
                    //USBUART_PutChar(A) ;

                    /* If the last sent packet is exactly the maximum packet 
                    *  size, it is followed by a zero-length packet to assure
                    *  that the end of the segment is properly identified by 
                    *  the terminal.
                    */
                    if (USBUART_BUFFER_SIZE == count){
                        /* Wait until component is ready to send data to PC. */
                        while (0u == USBUART_CDCIsReady()){}

                        /* Send zero-length packet to PC. */
                        USBUART_PutData(NULL, 0u);
                    }
                //}
            }
        }
        //sendMessage();
    }
}

/* [] END OF FILE */
