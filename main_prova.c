/*
 * File:   main_prova.c
 * Author: andre
 *
 * Created on 9 aprile 2025, 14.37
 */


#define Fcy 72000000
#define ACC_CS LATBbits.LATB3 //registro accelerometro
#define MAG_CS LATDbits.LATD6 //registro magnometro
#define GYR_CS LATBbits.LATB4 //registro giroscopio

#include "extimer1.h"
#include <stdbool.h>
#include <math.h>

typedef struct {
    int presc1;
    int presc2;
} PrescValues;

typedef struct {
    unsigned int value1;
    unsigned int value2;
} Values;

PrescValues spi_presc(long int clock){
    PrescValues result;
    long int Clockrate = Fcy / clock; //clockrate = 180
    int i = 0;
    bool done = true;
    int primarypresc = 0;
    int secondarypresc = 0;
    while(done){
        primarypresc = pow (2, i);
        secondarypresc = Clockrate / primarypresc;
        if (secondarypresc > 8){
            i = i + 2;
        } else {
            done = false;
        }
    }
    if (primarypresc == 1){
        result.presc1 = 3;
    }else if (primarypresc == 4){
        result.presc1 = 2;
    }else if (primarypresc == 16){
        result.presc1 = 1;
    }else{
        result.presc1 = 0;
    }
    if (secondarypresc == 1){
        result.presc2 = 7;
    }else if (secondarypresc == 2){
        result.presc2 = 6;
    }else if (secondarypresc == 3){
        result.presc2 = 5;
    }else if (secondarypresc == 4){
        result.presc2 = 4;
    }else if (secondarypresc == 5){
        result.presc2 = 3;
    }else if (secondarypresc == 6){
        result.presc2 = 2;
    }else if (secondarypresc == 7){
        result.presc2 = 1;
    }else{
        result.presc2 = 0;
    }
    return result;
}

void spi_setup(long int clock){
    SPI1STATbits.SPIEN = 0; //diabilito spi per fare configurazione
    SPI1CON1bits.MSTEN = 1; //imposto 
    SPI1CON1bits.MODE16 = 0;
    SPI1CON1bits.CKE = 1;
    SPI1STATbits.SPIROV = 0;
    
    PrescValues prval = spi_presc(clock);
    SPI1CON1bits.PPRE = prval.presc1;
    SPI1CON1bits.SPRE = prval.presc2;
    
    TRISAbits.TRISA1 = 1;
    TRISFbits.TRISF12 = 0;
    TRISFbits.TRISF13 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISDbits.TRISD6 = 0;
    RPINR20bits.SDI1R = 0b0010001;
    RPOR12bits.RP109R = 0b000101;
    RPOR11bits.RP108R = 0b000110;
    ACC_CS = 1;
    GYR_CS = 1;
    MAG_CS = 1;
    
    SPI1STATbits.SPIEN = 1;
}

void mag_enable(){
    unsigned int addr;
    unsigned int trash;
    
    MAG_CS = 0;
    addr = 0x004B;
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = addr & 0x7F;
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x01;
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;
    MAG_CS = 1;
    
    tmr_wait_ms(TIMER1, 4);
    
    MAG_CS = 0;
    addr = 0x004C;
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = addr & 0x7F;
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x00;
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;
    MAG_CS = 1;
    
    if (SPI1STATbits.SPIROV == 0){
        SPI1STATbits.SPIROV = 1;
    }
}

unsigned int spi_write(unsigned int read_addr){
    unsigned int value;
    unsigned int trash;
    
    MAG_CS = 0;
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = read_addr | 0x80;
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x00;
    while (SPI1STATbits.SPIRBF == 0);
    value = SPI1BUF;
    MAG_CS = 1;
    
    if (SPI1STATbits.SPIROV == 0){
        SPI1STATbits.SPIROV = 1;
    }
    
    return value;
}

Values spi_write_double(unsigned int read_addr){
    Values values;
    unsigned int trash;
    
    MAG_CS = 0; //seleziono come slave il magnometro
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = read_addr | 0xC0;
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x00;
    while (SPI1STATbits.SPIRBF == 0);
    values.value1 = SPI1BUF; 
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x00;
    while (SPI1STATbits.SPIRBF == 0);
    values.value2 = SPI1BUF;
    MAG_CS = 1;
    
    if (SPI1STATbits.SPIROV == 0){
        SPI1STATbits.SPIROV = 1;
    }
    
    return values;
}

