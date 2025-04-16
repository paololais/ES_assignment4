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
    unsigned int lsb;
    unsigned int msb;
    unsigned int raw; 
    int signed_value;

    char buffer[32];
    
    UART1_Init(); // Inizializza UART1
    
    tmr_setup_period(TIMER2, 100);
    
    while(1){
        spi_write_2_reg(read_addr, &lsb, &msb);
        lsb = lsb & 0x00F8;
        msb = msb << 8; //left shift by 8
        raw = msb | lsb; //put together the two bytes
        //raw = raw >> 3; //right shift by 3
        signed_value = (int) raw / 8; // alternativa più robusta
        
        // Copia bit-a-bit in un signed 16-bit per convertire da unsigned a signed
        // portabilità: evito comportamenti implementation-defined
        // memcpy(&signed_value, &raw, sizeof(raw));
        
        sprintf(buffer, "$MAGX=%d*", signed_value);

        int l = strlen(buffer);
        for (int i = 0; i < l; i++){
            UART1_WriteChar(buffer[i]);
        }

        tmr_wait_period(TIMER2);
    }
    return 0;
}