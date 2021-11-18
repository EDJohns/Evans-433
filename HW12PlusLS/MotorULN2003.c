#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "spi.h"
#include "ST7789.h"
#include <stdio.h>
#include <math.h>
#include "font.h"

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


void delay(void){
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT()<50000) {
        ;
    }
}

void delay2(void){
   int a;
   for( a = 0; a < 50000 ; a = a + 1 ){
      ;
   }
}

int main() {
    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;
    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;
    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    // do your TRIS and LAT commands here
    
    //Input data
    TRISBbits.TRISB4=1;
    TRISBbits.TRISB2=1;
    
    
    //Output information 
    TRISAbits.TRISA2=0;
    TRISAbits.TRISA3=0;
    TRISAbits.TRISA4=0;
    TRISAbits.TRISA1=0;
    TRISAbits.TRISA0=0;
    
    LATAbits.LATA4=0;
    
  // Initalize re programmble button to RB3 -> OC1
    ANSELA = 0;
    ANSELB = 0;
    RPB3Rbits.RPB3R=0b0101;
    
    // Set PWM wave using timer 2
    T2CONbits.TCKPS = 0b100; // Timer2 prescaler N=16
    PR2 = 59999; // period = (PR2+1) * N * 20.833 ns 
    TMR2 = 0; // initial TMR2 count is 0
    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
    OC1RS = 1500; // set duty cycle
    OC1R = 1500; // initialize before turning OC1 on; afterward it is read-only
    T2CONbits.ON = 1; // turn on Timer2
    OC1CONbits.ON = 1; // turn on OC1
    
    __builtin_enable_interrupts();
    
    int count=0;
    int a;

    float fps=30;
    initSPI();
    LCD_init();
    LCD_clearScreen(BLACK);
    

    int step=0;
    
    float RevPerMin=10; // only values 0-14.5 will be accepted
    if (RevPerMin>14.5){
        RevPerMin=14.5;
    }
    
    //Find mm per rev by measurment
    float mmPerRev = 110.0/RevPerMin; // #mm per min/revs per min about 7.6mm
    float mmPerMin = mmPerRev*RevPerMin;
    
    float timeConstant=60.0/(2048.0*RevPerMin);
    int delayVal = timeConstant * 24000000;
    char str[25] = "Hello World";
    int PlusMinusOne=1;
    
    //Term to cause the LCD writing delay
    // Display twice a second
    int LCDtimer=0;
    int LCDtimer_limit= (24000000/delayVal)*3;
    
    
    _CP0_SET_COUNT(0);
    
    while (1) {
        if (LCDtimer >LCDtimer_limit){
        LCDtimer=0;
        sprintf(str, "Speed: %d  ", count);
        drawString(50,50,RED,str,10);

        sprintf(str, "Rev/Min: %6.2f", RevPerMin);
        drawString(50,70,RED,str,15);
        
        sprintf(str, "Wait Tics: %d  ", delayVal);
        drawString(50,90,RED,str,16);
        
        sprintf(str, "Speed: %6.1f  mm/min", mmPerMin);
        drawString(50,110,RED,str,21);
        
        count++;
        if (count==100){
            count=0;
        }
        }
        if (PORTBbits.RB4 == 0){// button goes to zero when pressed
            LATAbits.LATA4=1;
            PlusMinusOne=1;
        }
        
        if (PORTBbits.RB2 == 0){// button goes to zero when pressed
            LATAbits.LATA4=1;
            PlusMinusOne=-1;
        }
        
        if (PORTBbits.RB4 == 1 & PORTBbits.RB2 == 1){// button goes to zero when pressed
            LATAbits.LATA4=0;
            LCDtimer=LCDtimer+1;
            continue;
        }
        
        
        
        step=step+PlusMinusOne;
        LCDtimer=LCDtimer+1;
        
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT()<delayVal) {
            ;
        }
                
        if (step==4){
            step=0;
        }
        if (step==-4){
            step=0;
        }
        
        if (step==0){
            LATAbits.LATA0=1; //INT1
            LATAbits.LATA1=0;  
            LATAbits.LATA2=0;
            LATAbits.LATA3=0; //INT4
        }
        if (step==1||step==-3){
            LATAbits.LATA0=0; //INT1
            LATAbits.LATA1=1;  
            LATAbits.LATA2=0;
            LATAbits.LATA3=0; //INT4
        }
        if (step==2||step==-2){
            LATAbits.LATA0=0; //INT1
            LATAbits.LATA1=0;  
            LATAbits.LATA2=1;
            LATAbits.LATA3=0; //INT4
        }
        if (step==3||step==-1){
            LATAbits.LATA0=0; //INT1
            LATAbits.LATA1=0;  
            LATAbits.LATA2=0;
            LATAbits.LATA3=1; //INT4
        }
        
    }
}  
 
