#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
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
unsigned char spi_io(unsigned char o);
void initSPI();

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
    TRISBbits.TRISB4=1;
    TRISAbits.TRISA4=0;
    LATAbits.LATA4=0;

    U1RXRbits.U1RXR = 0b0001; // U1RX is B6
    RPB7Rbits.RPB7R = 0b0001; // U1TX is B7
    
      // turn on UART3 without an interrupt
    U1MODEbits.BRGH = 0; // set baud to NU32_DESIRED_BAUD
    U1BRG = ((48000000 / 115200) / 16) - 1;

    // 8 bit, no parity bit, and 1 stop bit (8N1 setup)
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;

    // configure TX & RX pins as output & input pins
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;

    // enable the uart
    U1MODEbits.ON = 1;
    initSPI();
    __builtin_enable_interrupts();
    unsigned short P = 800;
    unsigned short P2 = 800;
    P=P<<2;
    P=P|(0b1111<<12);
    P2=P2<<2;
    P2=P2|(0b0111<<12);
    unsigned char out = 255;
    //P = 0b1111111111111111;
    
    //Generate Triangle array
    int Tri_array[100];
    int c=0;
    float temp=0.0;
        for (c=0; c<100; c++) { 
            if (c<50){
                temp= 0.02*(float) c*900;
                Tri_array[c]=(int) temp;
            }
            else if (c<100){
                temp=-.02*(float)(c-50)*900+900;
                Tri_array[c]=(int) temp;
            }
           // printf ( "%d\n\r" , Tri_array[c] ); 
        }
    
    int ii=0;
    float pi=3.141592;
    int Sin_array[100];
        for (ii=0; ii<100; ii++) { 
            temp= (float) ii * 0.04* pi;
            temp=sin(temp)*400.0+400.0;
            Sin_array[ii]=(int) temp;
        }

    int i =0;
    while (1) {
        //  delay();
        //LATAbits.LATA4=0;
        //delay();

        LATAbits.LATA1=0;// CS LOW
        P=Sin_array[i];//Sending Triangle Wave
        P=P<<2;
        P=P|(0b0111<<12);
        spi_io(P>>8); 
        spi_io(P); 
        
        LATAbits.LATA1=1;// CS HIGH
        
        LATAbits.LATA1=0;// CS LOW
        P2=Tri_array[i];//Sending Triangle Wave
        P2=P2<<2;
        P2=P2|(0b1111<<12);
        spi_io(P2>>8); 
        spi_io(P2);
        LATAbits.LATA1=1;// CS HIGH
        
        delay2();
        LATAbits.LATA4=1;
        i++;
        if (i==100){
            i=0;
        }
        
    }
}

void delay(void){
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT()<12000000) {
        ;
    }
}

void delay2(void){
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT()<210000) { // hald second delay (-30000) to account for DAC delay
        ;
    }
}

unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}

void initSPI() {
    // Pin B14 has to be SCK1
    // Turn of analog pins
    ANSELA=0;
    // Make an output pin for CS
    TRISAbits.TRISA1=0;
    LATAbits.LATA1=1;
    // Set SDO1
    RPB5Rbits.RPB5R=0b0011;
    // Set SDI1
    //...

    // setup SPI1
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1000; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
}