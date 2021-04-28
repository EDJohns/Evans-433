#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
#include "i2c_master_noint.h"
//#include SPI__H__
//#include <studio.h>

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use internal oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // internal RC
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF	 // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0xFFFF // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

void delay(void);
void delay2(void);
void setPin(unsigned char address, unsigned char reg, unsigned char value);
unsigned char readPin(unsigned char Waddress,unsigned char Raddress, unsigned char reg);

// void i2c_master_setup(void); // set up I2C1 as master
// void i2c_master_start(void); // send a START signal
// void i2c_master_restart(void); // send a RESTART signal
// void i2c_master_send(unsigned char byte); // send a byte (either an address or data)
// unsigned char i2c_master_recv(void); // receive a byte of data
// void i2c_master_ack(int val); // send an ACK (0) or NACK (1)
// void i2c_master_stop(void); // send a stop

int main() {
    __builtin_disable_interrupts(); // disable interrupts while initializing things
    
    i2c_master_setup();
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;
    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;
    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    // do your TRIS and LAT commands here
    TRISBbits.TRISB4=1;
    TRISAbits.TRISA4=0;
    TRISAbits.TRISA2=0;
    LATAbits.LATA4=0;
    
    __builtin_enable_interrupts();
    
    unsigned char Wadd=0b01000000;
    unsigned char Radd=0b01000001;
    //i2c_master_setup();
    
    i2c_master_start();
    i2c_master_send(Wadd);
    i2c_master_send(0x00); //I0DIRA
    i2c_master_send(0x00); // Make all As outputs
    i2c_master_stop();

    i2c_master_start();
    i2c_master_send(Wadd);
    i2c_master_send(0x01); //I0DIRB
    i2c_master_send(0xFF); // Make all Bs Inputs
    i2c_master_stop();
    
    
    unsigned char rec=0x80;
    
    while (1) {      
        setPin(Wadd,0x14,0x00);
        rec = readPin(Wadd,Radd,0x13);
        
        if (rec==0x01){
            setPin(Wadd,0x14,0x00); // Turns A1 off
        }
        
        if (rec==0x00){
            setPin(Wadd,0x14,0x40); // Turns A1 on 
        }
        
        LATAbits.LATA2=1;
        delay2();
        LATAbits.LATA2=0;
        delay2();
    }
}

void setPin(unsigned char address, unsigned char reg, unsigned char value){
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(reg); //OLATA address
    i2c_master_send(value); // TurnA1 off
    i2c_master_stop();
}

unsigned char readPin(unsigned char Waddress,unsigned char Raddress, unsigned char reg){
    unsigned char read = 0;
    i2c_master_start();
    i2c_master_send(Waddress);
    i2c_master_send(reg); //OLATA address
    i2c_master_restart();
    i2c_master_send(Raddress);
    read = i2c_master_recv();
    i2c_master_ack(0x01);
    i2c_master_stop();
    return read;
}

void delay(void){
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT()<12000000) {
        ;
    }
}
void delay2(void){
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT()<2000000) {
        ;
    }
}

