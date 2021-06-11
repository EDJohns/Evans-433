#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
//include "ws2812b.h"
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

#define LOWTIME 15 // number of 48MHz cycles to be low for 0.35uS
#define HIGHTIME 65 // number of 48MHz cycles to be high for 1.65uS

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} wsColor; 

void delay(void);
void ws2812b_setColor(wsColor * c, int numLEDs);
wsColor HSBtoRGB(float hue, float sat, float brightness);

void ws2812b_setup() {
    T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)
    PR2 = 65535; // maximum period
    TMR2 = 0; // initialize Timer2 to 0
    T2CONbits.ON = 1; // turn on Timer2
    // initialize output pin as off
    TRISBbits.TRISB3=0;
    TRISAbits.TRISA2=0;
    LATBbits.LATB3=0;
    
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
    __builtin_enable_interrupts();
    
    wsColor Carray[4];
    float hue=0;
    float hue2=80.0;
    float hue3=160.0;
    float hue4=240.0;
    Carray[0]=HSBtoRGB(60.0,1.0,1.0);
    Carray[1]=HSBtoRGB(120.0,1.0,1.0);
    Carray[2]=HSBtoRGB(180.0,1.0,1.0);
    Carray[3]=HSBtoRGB(240.0,1.0,1.0);
//    Carray[0].r=255 ;
//    Carray[0].g=0;
//    Carray[0].b=0;
//    Carray[1].r=0;
//    Carray[1].g=255;
//    Carray[1].b=0;
//    Carray[2].r=0;
//    Carray[2].g=0;
//    Carray[2].b=255;
//    Carray[3].r=155;
//    Carray[3].g=155;
//    Carray[3].b=0;

    ws2812b_setup();
    int i=0;
    while(1){
        Carray[0]=HSBtoRGB(hue,1.0,1.0);
        Carray[1]=HSBtoRGB(hue2,1.0,1.0);
        Carray[2]=HSBtoRGB(hue3,1.0,1.0);
        Carray[3]=HSBtoRGB(hue4,1.0,1.0);
        hue= hue+10;
        if (hue>359){
            hue=0;
        }
        hue2= hue2+10;
        if (hue2>359){
            hue2=0;
        }
        hue3= hue3+10;
        if (hue3>359){
            hue3=0;
        }
        hue4= hue4+10;
        if (hue4>359){
            hue4=0;
        }
        
        ws2812b_setColor(Carray,4);
        delay();
    }
    return 0;
}

void delay(void){
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT()<2000000) {
        ;
    }
}
void delay2(void){
    LATAINV = 0b0000100; 
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT()<2000000) {
        ;
    }
}

void ws2812b_setColor(wsColor * c, int numLEDs) {
    unsigned char red, green,blue;
    int i = 0; int j = 0; // for loops
    int numBits = 2 * 3 * 8 * numLEDs; // the number of high/low bits to store, 2 per color bit
    volatile unsigned int delay_times[2*3*8 * numLEDs]; // I only gave you 5 WS2812B, adjust this if you get more somewhere

    // start at time at 0
    delay_times[0] = 0;
    
    int nB = 1; // which high/low bit you are storing, 2 per color bit, 24 color bits per WS2812B
	
    // loop through each WS2812B
    for (i = 0; i < 4; i++) {
        //delay2();
        // loop through each color bit, MSB first
        red=c[i].r;
        green=c[i].g;
        blue=c[i].b;
         // FOR RED
        for (j = 7; j >= 0; j--) {
            //delay2();
            // if the bit is a 1
            if ((red & 0x80)>0) {
               red = red << 1;
                // the high is longer
                //printf(" Red bit %d is 1\n", j);
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
            } 
            else { // if bit is a 0
                // the low is longer
                red = red << 1;
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
               // printf(" Red bit %d is 0\n", j);
            }
        }
         for (j = 7; j >= 0; j--) {
            // if the bit is a 1
            if ((green & 0x80)>0) {
               green = green << 1;
                // the high is longer
              //  printf(" Green bit %d is 1\n", j);
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
            } 
            else { // if bit is a 0
                // the low is longer
                green = green << 1;
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
               // printf(" Green bit %d is 0\n", j);
            }
        }
         for (j = 7; j >= 0; j--) {
             //delay2();
            // if the bit is a 1
            if ((blue & 0x80)>0) {
               blue = blue << 1;
                // the high is longer
              //  printf("Blue bit %d is 1\n", j);
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
            } 
            else { // if bit is a 0
                // the low is longer
                blue = blue << 1;
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
              //  printf("Blue bit %d is 0\n", j);
            }
        }
}
    // turn on the pin for the first high/low

    LATBbits.LATB3 = 1;
    TMR2 = 0; // start the timer
    for (i = 1; i < numBits; i++) {
        while (TMR2 < delay_times[i]) {
        }
       // delay2();
        LATBINV = 0b0001000; // invert B3
    }
    LATBbits.LATB3 = 0;
    TMR2 = 0;
    while(TMR2 < 2400){} // wait 50uS, reset condition
}

// adapted from https://forum.arduino.cc/index.php?topic=8498.0
// hue is a number from 0 to 360 that describes a color on the color wheel
// sat is the saturation level, from 0 to 1, where 1 is full color and 0 is gray
// brightness sets the maximum brightness, from 0 to 1
wsColor HSBtoRGB(float hue, float sat, float brightness) {
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;

    if (sat == 0.0) {
        red = brightness;
        green = brightness;
        blue = brightness;
    } else {
        if (hue == 360.0) {
            hue = 0;
        }

        int slice = hue / 60.0;
        float hue_frac = (hue / 60.0) - slice;

        float aa = brightness * (1.0 - sat);
        float bb = brightness * (1.0 - sat * hue_frac);
        float cc = brightness * (1.0 - sat * (1.0 - hue_frac));

        switch (slice) {
            case 0:
                red = brightness;
                green = cc;
                blue = aa;
                break;
            case 1:
                red = bb;
                green = brightness;
                blue = aa;
                break;
            case 2:
                red = aa;
                green = brightness;
                blue = cc;
                break;
            case 3:
                red = aa;
                green = bb;
                blue = brightness;
                break;
            case 4:
                red = cc;
                green = aa;
                blue = brightness;
                break;
            case 5:
                red = brightness;
                green = aa;
                blue = bb;
                break;
            default:
                red = 0.0;
                green = 0.0;
                blue = 0.0;
                break;
        }
    }

    unsigned char ired = red * 255.0;
    unsigned char igreen = green * 255.0;
    unsigned char iblue = blue * 255.0;

    wsColor c;
    c.r = ired;
    c.g = igreen;
    c.b = iblue;
    return c;
}