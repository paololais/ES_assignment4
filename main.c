/*
 * File:   main.c
 * Author: paolo
 *
 * Created on 26 marzo 2025, 14.19
 */


#include "xc.h"
#include "spi.h"
#include "timer.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"

/* Assignment 1

int main(void) {
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000; // Disabilita gli ingressi analogici
    
    spi_init();
    
    //Make the magnetometer switch to Sleep mode; then make it go to active mode; 
    mag_enable();
    
    CircularBuffer cb;
    cb_init(&cb);
    
    unsigned int read_addr = 0x40;
    unsigned int chip_id = spi_write(read_addr);
    
    UART1_Init(); // Inizializza UART1
    UART1_WriteChar((char)chip_id);
    
    while(1){
        
    }
    return 0;
}
*/

//Assignment 2

int main(void) {
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000; // Disabilita gli ingressi analogici
    
    spi_init();
    
    //Make the magnetometer switch to Sleep mode; then make it go to active mode; 
    mag_enable();
    
    CircularBuffer cb;
    cb_init(&cb);
    
    unsigned int read_addr = 0x42;
    unsigned int value1;
    unsigned int value2;
    unsigned int value;
    //unsigned int chip_id = spi_write(read_addr);
    char buffer1[32];
    char buffer2[16];
    
    UART1_Init(); // Inizializza UART1
    //UART1_WriteChar((char)chip_id);
    
    tmr_setup_period(TIMER2, 100);
    
    while(1){
        spi_write_2_reg(read_addr, &value1, &value2);
        value1 = value1 & 0x00F8;
        value2 = value2 << 8;
        value = value2 | value1;
        value = value >> 3;
        
        sprintf(buffer1, "$MAGX ");
        sprintf(buffer2, "%u\n", value);
        strcat(buffer1, buffer2);
        int l = strlen(buffer1);
        for (int i = 0; i < l; i++){
            UART1_WriteChar(buffer1[i]);
        }

        tmr_wait_period(TIMER2);
    }
    return 0;
}