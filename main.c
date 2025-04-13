/*
 * File:   main.c
 * Author: paolo
 *
 * Created on 26 marzo 2025, 14.19
 */


#include "xc.h"
#include "spi.h"
#include "timer.h"

//prova push

int main(void) {
    //Configure SPI;
    spi_init();
    
    //Make the magnetometer switch to Sleep mode; 
    //sleep mode
    spi_write_register(0x4B, 0x01);
    
    //wait 3 ms before switching to active mode
    tmr_wait_ms(TIMER1, 3);
    
    //then make it go to active mode;
    spi_write_register(0x4C, 0x00);
    
    while(1){
        
    }
    return 0;
}
