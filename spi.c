/*
 * File:   spi.c
 * Author: paolo
 *
 * Created on 31 marzo 2025, 10.29
 */

#include "xc.h"
#include "spi.h"

void spi_init() {
    SPI1STATbits.SPIEN = 0;    // Disabilita SPI per configurazione

    SPI1CON1bits.MSTEN = 1;    // Master mode
    SPI1CON1bits.MODE16 = 0;   // 8-bit mode
    SPI1CON1bits.CKP = 0;      // Clock idle low
    SPI1CON1bits.CKE = 1;      // Trasmissione su transizione attivo?idle
    SPI1CON1bits.SMP = 0;      // Input sample at middle of data output time

    // Fcy = 72MHz ? F_SPI = Fcy / (Primary × Secondary)
    SPI1CON1bits.PPRE = 0b00;  // Primary prescaler 64:1
    SPI1CON1bits.SPRE = 0b101; // Secondary prescaler 3:1

    SPI1STATbits.SPIROV = 0;   // Clear overflow
    SPI1STATbits.SPIEN = 1;    // Abilita SPI
}


unsigned int spi_write(unsigned int data){
    while(SPI1STATbits.SPITBF == 1); // wait until the tx buffer is not full
    SPI1BUF = data; // transmit
    
    while(SPI1STATbits.SPIRBF == 0); // wait until data has arrived
    unsigned int data_received = SPI1BUF;
    
    return data_received;
}

void spi_write_register(uint8_t reg_addr, uint8_t data) {
    LATBbits.LATB2 = 0; // CS LOW (attiva il chip)
    
    int dummy = spi_write(reg_addr & 0x7F);
    dummy = spi_write(data);
    
    /*
    SPI1BUF = reg_addr & 0x7F; // MSB = 0 per write
    while (!SPI1STATbits.SPIRBF); 
    volatile uint8_t dummy = SPI1BUF; // svuota il buffer

    SPI1BUF = data;
    while (!SPI1STATbits.SPIRBF); 
    dummy = SPI1BUF;
    */
     
    LATBbits.LATB2 = 1; // CS HIGH (disattiva il chip)
}