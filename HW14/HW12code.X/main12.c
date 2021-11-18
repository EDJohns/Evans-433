#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include <math.h>



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

void setDegree(float degree);
void delay(void){
    int i=0;
    for (i=0;i<3000000;i++){
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
    TRISAbits.TRISA4=0;
    TRISAbits.TRISA2=0;
    LATAbits.LATA2=0;
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
    // generates sin function
    float tempy;
    int ii=0;
    float pi=3.141592;
    float Sin_array[100];
        for (ii=0; ii<100; ii++) { 
            tempy=(float) 0.02*ii*pi;
            tempy=sin(tempy);
            Sin_array[ii]= (tempy);
            //printf("sin wave value %6.2f\n",Sin_array[ii]);
        }

    int degree;
    int count=0;
    LATAbits.LATA4=1;// Tell me the code is working
    delay();
    LATAbits.LATA4=0;
    delay();
    
    while (1) {

        degree=90+80*Sin_array[count]; // Set degree from 10 to 170
        setDegree(degree); // send to degree function
        count=count+1;
        if (count>100){
            count = 0;
        }
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT()<240000) { // 50 hz delay 
            ;
        }
        

    }
}

void setDegree(float degree){
    int temp= (33.333333* degree)+1500;
    OC1RS=(int) temp;
}







