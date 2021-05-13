#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include "font.h"
#include "spi.h"
#include "ST7789.h"
#include "i2c_master_noint.h"

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
void setPin(unsigned char address, unsigned char reg, unsigned char value);
unsigned char readPin(unsigned char Waddress,unsigned char Raddress, unsigned char reg);
void I2C_read_multiple(unsigned char address, unsigned char register, unsigned char * data, int length);

void delay(void){
    int i=0;
    for (i=0;i<50000;i++){
        ;
    }
}
//help

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
    TRISAbits.TRISA4=0;
    TRISAbits.TRISA2=0;
    LATAbits.LATA2=0;
    LATAbits.LATA4=0;
    
    int count=0;
    int Ncount=0;
    float fps=30;
    initSPI();
    LCD_init();
    LCD_clearScreen(BLACK);
    char str[25] = "Hello World";
    unsigned char data[14];
    short dataS[7];
    unsigned char Wadd=0b11010100;
    unsigned char Radd=0b11010101;
    
    unsigned char set1= 0b10000010;
    unsigned char set2= 0b10001000;
    unsigned char set3= 0b00000100;
    short Ay=0;
    unsigned char Ay1= 0;
    unsigned char Ay2= 0;
    
    __builtin_enable_interrupts();
    
    setPin(Wadd, 0x10, 0b10000010);
    setPin(Wadd, 0x11, 0b10001000);
    setPin(Wadd, 0x12, 0b00000100);
    
    unsigned char rec=0x80;
    rec = readPin(Wadd,Radd,0x0F);
    int i=0;
    int ii=0;
    int len=0;
    while (1) {
        _CP0_SET_COUNT(0);
        int ii=0;
        //drawBar(50,70,100,count);
       // drawBar2(120,120,50,count/2,1,0);
        //drawBar2(120,120,50,count/2,1,1);
        
        count++;
        if (count==100){
            count=0;
        }
        // Write idenity statement
        //rec = readPin(Wadd,Radd,0x0F);
//        sprintf(str, "Who: %d", rec);
//        drawString(10,10,RED,str,8);
        
        I2C_read_multiple(Wadd, 0x20, data,14);
        
        //Transform chars to shorts
        for (i=0;i<7;i++){
            dataS[i]=(((short)data[ii+1]) << 8) | data[ii];
            ii=ii+2;
        }
        
        sprintf(str, "T: %d  ",dataS[0]);
        drawString(10,10,RED,str,8);
        
        sprintf(str, "A: %d %d %d          ",dataS[4], dataS[5],dataS[6]); //acceleation values
        drawString(10,25,RED,str,19);
        
        sprintf(str, "w: %d %d %d        ",dataS[1], dataS[2],dataS[3]); //omega balues
        drawString(10,40,RED,str,19);
        
        
        len = abs((dataS[4]*70)/17000);
        if(dataS[4]>0){
            drawBar2(120,120,70,len,1,0,BLUE);
        }
        if(dataS[4]<0){
            drawBar2(120,120,70,len,-1,0,BLUE);
        }
        len = abs((dataS[5]*70)/17000);
        if(dataS[5]>0){
            drawBar2(120,120,70,len,1,1,GREEN);
        }
        if(dataS[5]<0){
            drawBar2(120,120,70,len,-1,1,GREEN);
        }      
       // drawBar2(120,120,50,count/2,1,1,BLUE);
     
        fps = 1/((float) _CP0_GET_COUNT()/24000000); // Note this is not accurate because of the delay statements
        sprintf(str, "fps:%6.2f ", fps);
        drawString(10,210,RED,str,11);
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

void I2C_read_multiple(unsigned char address, unsigned char reg, unsigned char * data, int length){
    int i=0;
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(reg); //OLATA address
    i2c_master_restart();
    i2c_master_send(address+1);
    for (i=0;i<length-1;i++){
        data[i] = i2c_master_recv();
        i2c_master_ack(0x00); 
    }
    data[length-1] = i2c_master_recv();
    i2c_master_ack(0x01);
    i2c_master_stop();

}