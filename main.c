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

// Assignment 1

int main(void) {
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000; // Disabilita gli ingressi analogici
    
    CircularBuffer cb;
    unsigned int chip_id;
    unsigned int read_addr;
    
    spi_init();
    UART1_Init(); // Inizializza UART1
    cb_init(&cb);
    
    //Make the magnetometer switch to Sleep mode;then make it go to active mode; 
    void mag_enable(); 
    read_addr = 0x40;
    chip_id = spi_write(read_addr);
    //cb_push(&cb, (char)chip_id);
    UART1_WriteChar((char)chip_id);
    while(1){
        
    }
    return 0;
}
