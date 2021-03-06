/*
  PIC18F47Q43 ROM RAM and UART emulation firmware
  This single source file contains all code

  Target: EMUZ80 - The computer with only Z80 and PIC18F47Q43
  Compiler: MPLAB XC8 v2.36
  Written by Tetsuya Suzuki

  Target: EMU1802 - The computer with only COSMAC CDP1802 and PIC18F47Q43
  Modifed by Kazuhiro Ouchi
*/

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator Selection (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

// CONFIG2
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG3
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (Low-Power BOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG4
#pragma config BORV = VBOR_1P9  // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD module is disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be set and cleared repeatedly (subject to the unlock sequence))
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG5
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG6
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG7
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config DEBUG = OFF      // Background Debugger (Background Debugger disabled)

// CONFIG8
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not Write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not Write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not Write protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG10
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>

//#define Z80_CLK 2500000UL // Z80 clock frequency(Max 16MHz)

//#define ROM_SIZE 0x4000 // ROM size 16K bytes
//#define RAM_SIZE 0x1000 // RAM size 4K bytes
//#define RAM_TOP 0x8000 // RAM start address

// CDP1802
//#define Z80_CLK 1800000UL // CDP1802 clock frequency(1.8MHz)
#define Z80_CLK   200000UL // CDP1802 clock frequency(0.2MHz)

// for Low address test ($0000-$00FF)
//#define ROM_SIZE 0x80 // ROM size 128 bytes
//#define RAM_SIZE 0x80 // RAM size 128 bytes
//#define RAM_TOP 0x80 // RAM start address

// for High Address (ROM $0000-$0400, RAM $8000-8FFF)
//#define ROM_SIZE 0x0030 // ROM size 256 bytes
#define ROM_SIZE 0x0400 // ROM size 1k bytes
#define RAM_SIZE 0x1000 // RAM size 4k bytes
#define RAM_TOP 0x8000 // RAM start address

#define UART_DREG 0xE000 // UART data register address
#define UART_CREG 0xE001 // UART control register address

#define _XTAL_FREQ 64000000UL

extern const unsigned char rom[]; // Equivalent to ROM, see end of this file
unsigned char ram[RAM_SIZE]; // Equivalent to RAM
static union {
    unsigned int w; // 16 bits Address
    struct {
        unsigned char l; // Address low 8 bits
        unsigned char h; // Address high 8 bits
    };
} address;
    

/*
// UART3 Transmit
void putch(char c) {
    while(!U3TXIF); // Wait or Tx interrupt flag set
    U3TXB = c; // Write data
}

// UART3 Recive
char getch(void) {
    while(!U3RXIF); // Wait for Rx interrupt flag set
    return U3RXB; // Read data
}
*/

//#define dff_reset() {G3POL = 1; G3POL = 0;}
#define db_setin() (TRISC = 0xff)
#define db_setout() (TRISC = 0x00)

// Never called, logically
void __interrupt(irq(default),base(8)) Default_ISR(){}

/**
   IOCAF1 Interrupt Service Routine TPB(RA1) Rising edge
 */
// Called at COSMAC TPB(RA1) Rising edge
void IOCAF1_ISR(void) {
    IOCAFbits.IOCAF1 = 0;
    //address.h = 0; // Read address high (256byte only)
    address.l = PORTB; // Read address low
    
    if(!RA5) { // COSMAC memory read cycle (MRD active)
        db_setout(); // Set data bus as output
        if(address.w < ROM_SIZE){ // ROM area
            LATC = rom[address.w]; // Out ROM data
        } else if((address.w >= RAM_TOP) && (address.w < (RAM_TOP + RAM_SIZE))){ // RAM area
            LATC = ram[address.w - RAM_TOP]; // RAM data
        //} else if(address.w == UART_CREG){ // UART control register
        //    LATC = PIR9; // U3 flag
        //} else if(address.w == UART_DREG){ // UART data register
        //    LATC = U3RXB; // U3 RX buffer
        } else { // Out of memory
            LATC = 0xff; // Invalid data
        }
    } else { // COSMAC memory write cycle (MWR active)
        //if(RE0) while(RE0);
        if(!RE0) {
            if((address.w >= RAM_TOP) && (address.w < (RAM_TOP + RAM_SIZE))){ // RAM area
                ram[address.w - RAM_TOP] = PORTC; // Write into RAM
            //} else if(address.w == UART_DREG) { // UART data register
            //    U3TXB = PORTC; // Write into U3 TX buffer
            }
        }
    }
}

/**
   IOCAF2 Interrupt Service Routine
*/
// Called at COSMAC TPA(RA2) Falling edge
void IOCAF2_ISR(void) {
    IOCAFbits.IOCAF2 = 0;
    address.h = PORTB;  // Read address high
}

void __interrupt(irq(IOC),base(8)) PIN_MANAGER_IOC()
{   
	// interrupt on change for pin IOCAF1
    if(IOCAFbits.IOCAF1 == 1)
    {
        IOCAF1_ISR();  
    }	
	// interrupt on change for pin IOCAF2
    if(IOCAFbits.IOCAF2 == 1)
    {
        IOCAF2_ISR();  
    }	
}

#ifdef false

// Called at COSMAC TPB falling edge (PIC18F47Q43 issues WAIT)
void __interrupt(irq(CLC1),base(8)) CLC1_ISR(){
    CLC1IF = 0; // Clear interrupt flag
    
    //address.h = 0; // Read address high (256byte only)
    address.l = PORTB; // Read address low
    
    if(!RA5) { // COSMAC memory read cycle (MRD active)
        db_setout(); // Set data bus as output
        if(address.w < ROM_SIZE){ // ROM area
            LATC = rom[address.w]; // Out ROM data
        } else if((address.w >= RAM_TOP) && (address.w < (RAM_TOP + RAM_SIZE))){ // RAM area
            LATC = ram[address.w - RAM_TOP]; // RAM data
        //} else if(address.w == UART_CREG){ // UART control register
        //    LATC = PIR9; // U3 flag
        //} else if(address.w == UART_DREG){ // UART data register
        //    LATC = U3RXB; // U3 RX buffer
        } else { // Out of memory
            LATC = 0xff; // Invalid data
        }
    } else { // COSMAC memory write cycle (MRD inactive)
        //if(RE0) while(RE0);
        if((address.w >= RAM_TOP) && (address.w < (RAM_TOP + RAM_SIZE))){ // RAM area
            ram[address.w - RAM_TOP] = PORTC; // Write into RAM
            //} else if(address.w == UART_DREG) { // UART data register
            //    U3TXB = PORTC; // Write into U3 TX buffer
        }
    }
    dff_reset(); // WAIT inactive
}

// Called at COSMAC TPA falling edge (PIC18F47Q43 issues WAIT)
void __interrupt(irq(CLC2),base(8)) CLC2_ISR()
{
    CLC2IF = 0;         // Clear the CLC interrupt flag
    address.h = PORTB;  // Read address high
}

#endif

//  Called at COSMAC TPB(RA1) Falling edge
void __interrupt(irq(INT0),base(8)) INT0_ISR(){
    INT0IF = 0; // Clear interrupt flag
    db_setin(); // Set data bus as input
}

// main routine
void main(void) {
    // System initialize
    OSCFRQ = 0x08; // 64MHz internal OSC

    // Address bus A15-A8 pin (RD0-RD7) --- Unused
    ANSELD = 0x00; // Disable analog function
    WPUD = 0xff; // Week pull up
    TRISD = 0xff; // Set as input

    // Address bus A7-A0 pin (RB0-RB7)
    ANSELB = 0x00; // Disable analog function
    WPUB = 0xff; // Week pull up
    TRISB = 0xff; // Set as input

    // Data bus D7-D0 pin (RC0-RC7)
    ANSELC = 0x00; // Disable analog function
    WPUC = 0xff; // Week pull up
    TRISC = 0xff; // Set as input(default)

    // MWR input pin (RE0)
    ANSELE0 = 0; // Disable analog function
    WPUE0 = 1; // Week pull up
    TRISE0 = 1; // Set as input

    // RESET output pin (RE1)
    ANSELE1 = 0; // Disable analog function
    LATE1 = 0; // Reset
    TRISE1 = 0; // Set as output

    // INT output pin (RE2) --- Unused
    //ANSELE2 = 0; // Disable analog function
    //LATE2 = 1; // No interrupt request
    //TRISE2 = 0; // Set as output

    // MRD input pin (RA5)
    ANSELA5 = 0; // Disable analog function
    WPUA5 = 1; // Week pull up
    TRISA5 = 1; // Set as intput

    // clock(RA3) by NCO FDC mode
    RA3PPS = 0x3f; // RA3 asign NCO1
    ANSELA3 = 0; // Disable analog function
    TRISA3 = 0; // PWM output pin
    NCO1INCU = (unsigned char)((Z80_CLK*2/61/65536) & 0xff);
    NCO1INCH = (unsigned char)((Z80_CLK*2/61/256) & 0xff);
    NCO1INCL = (unsigned char)((Z80_CLK*2/61) & 0xff);
    NCO1CLK = 0x00; // Clock source Fosc
    NCO1PFM = 0;  // FDC mode
    NCO1OUT = 1;  // NCO output enable
    NCO1EN = 1;   // NCO enable

    // UART3 initialize
    //U3BRG = 416; // 9600bps @ 64MHz
    //U3RXEN = 1; // Receiver enable
    //U3TXEN = 1; // Transmitter enable

    // UART3 Receiver
    //ANSELA7 = 0; // Disable analog function
    //TRISA7 = 1; // RX set as input
    //U3RXPPS = 0x07; //RA7->UART3:RX3;

    // UART3 Transmitter
    //ANSELA6 = 0; // Disable analog function
    //LATA6 = 1; // Default level
    //TRISA6 = 0; // TX set as output
    //RA6PPS = 0x26;  //RA6->UART3:TX3;

    //U3ON = 1; // Serial port enable
    
    
    // TPB(RA1) CLC1 input pin
    ANSELA1 = 0; // Disable analog function
    WPUA1 = 1; // Week pull up
    TRISA1 = 1; // Set as input
    //CLCIN0PPS = 0x1; //RA1->CLC1:CLCIN0;

    // TPA(RA2) CLC2 input pin
    ANSELA2 = 0; // Disable analog function
    WPUA2 = 1; // Week pull up
    TRISA2 = 1; // Set as input
    //CLCIN1PPS = 0x2; //RA2->CLC1:CLCIN1;

    // RA4 CLC1 output pin --- debug
    //ANSELA4 = 0; // Disable analog function
    //LATA4 = 1; // Default level
    //TRISA4 = 0; // Set as output
    //RA4PPS = 0x01;  //RA4->CLC1:CLC1;

    // RA0 CLC2 output pin --- debug   
    //ANSELA0 = 0; // Disable analog function
    //LATA0 = 1; // Default level
    //TRISA0 = 0; // Set as output
    //RA0PPS = 0x02;  //RA0->CLC2:CLC2;
  
#ifdef false
    //CLC1 configuration
    CLCSELECT = 0x0; // CLC1 instance  
    CLCnPOL = 0x82; // LCG2POL inverted, LCPOL inverted

    // CLC data inputs select
    CLCnSEL0 = 0x2a;  // D-FF CLK assign NC01
    CLCnSEL1 = 0; // D-FF D assign CLCIN0PPS(RA1) 
    CLCnSEL2 = 127; // D-FF S assign none
    CLCnSEL3 = 127; // D-FF R assign none

    // CLCn gates logic select
    CLCnGLS0 = 0x1; // Connect LCG1D1N 
    CLCnGLS1 = 0x4; // Connect LCG2D2N
    CLCnGLS2 = 0x0; // Connect none
    CLCnGLS3 = 0x0; // Connect none

    CLCDATA = 0x0; // Clear all CLC outs
    CLCnCON = 0x8c; // Select D-FF, falling edge inturrupt
#endif

    
    // INTERRUPT_Initialize
    //
    // Vectored Interrupt Controller setting sequence
    INTCON0bits.IPEN = 1; // Interrupt priority enable

    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCKbits.IVTLOCKED = 0x00; // Unlock IVT

    IVTBASEU = 0;
    IVTBASEH = 0;
    IVTBASEL = 8; // Default VIT base address

    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCKbits.IVTLOCKED = 0x01; // Lock IVT

    // Assign peripheral interrupt priority vectors
    IPR0bits.IOCIP = 1;

    /**
    IOCx registers 
    */
    //interrupt on change for group IOCAF - flag COSMAC TPB(RA1)
    IOCAFbits.IOCAF1 = 0;
    //interrupt on change for group IOCAF - flag COSMAC TPA(RA2)
    IOCAFbits.IOCAF2 = 0;
    //interrupt on change for group IOCAN - negative
    IOCANbits.IOCAN1 = 0;
    //interrupt on change for group IOCAN - negative COSMAC TPA(RA2) Falling edge
    IOCANbits.IOCAN2 = 1;
    //interrupt on change for group IOCAP - positive COSMAC TPB(RA1) Rising edge
    IOCAPbits.IOCAP1 = 1;
    //interrupt on change for group IOCAP - positive
    IOCAPbits.IOCAP2 = 0;
 
    // Enable IOCI interrupt 
    PIE0bits.IOCIE = 1; 


#ifdef false
    // CLC VI enable
    CLC1IF = 0; // Clear the CLC interrupt flag
    CLC1IE = 1; // Enabling CLC1 interrupt
    
    // Assign peripheral interrupt priority vectors
    IPR6bits.CLC2IP = 1;
    IPR0bits.CLC1IP = 1;
  
    // Set the CLC2 to the options selected in the User Interface
     
    CLCSELECT = 0x01;   // SLCT 1;   
    CLCnPOL = 0x83;     // G1POL not_inverted; G2POL inverted; G3POL not_inverted; G4POL not_inverted; POL inverted;
    CLCnSEL0 = 0x2a;    // D1S NCO1_OUT; 
    CLCnSEL1 = 0x01;    // D2S CLCIN1 (NC01);
    CLCnSEL2 = 0x00;    // D2S CLCIN1 (CLCIN1PPS);   
    CLCnSEL3 = 0x00;    // D3S CLCIN0 (CLCIN0PPS);
    CLCnGLS0 = 0x01;    // G1D3N disabled; G1D2N disabled; G1D4N disabled; G1D1T disabled; G1D3T disabled; G1D2T disabled; G1D4T disabled; G1D1N enabled;  
    CLCnGLS1 = 0x04;    // G2D2N enabled; G2D1N disabled; G2D4N disabled; G2D3N disabled; G2D2T disabled; G2D1T disabled; G2D4T disabled; G2D3T disabled; 
    CLCnGLS2 = 0x00;    // G3D1N disabled; G3D2N disabled; G3D3N disabled; G3D4N disabled; G3D1T disabled; G3D2T disabled; G3D3T disabled; G3D4T disabled; 
    CLCnGLS3 = 0x00;    // G4D1N disabled; G4D2N disabled; G4D3N disabled; G4D4N disabled; G4D1T disabled; G4D2T disabled; G4D3T disabled; G4D4N disabled; 
    CLCDATA = 0x00;     // CLC2OUT 0; 
    CLCnCON = 0x8C;     // EN enabled; INTN enabled; INTP disabled; MODE 1-input D flip-flop with S and R;

    CLC2IF = 0; // Clear the CLC interrupt flag
    CLC2IE = 1; // Enabling CLC2 interrupt.

#endif

    // INT0 VI enable
    // COSMAC TPB(RA1) Falling edge
    INT0PPS = 0x1; //RA1->INTERRUPT MANAGER:INT0;
    INT0EDG = 0; // 0: Falling edge 1: Rising edge
    INT0IE = 1;
   
    // COSMAC start
    GIE = 1; // Global interrupt enable
    LATE1 = 1; // Release reset COSMAC ~CLEAR(RE1)
    
    while(1); // All things come to those who wait
}

// blink program 1
//00  7B               1->Q
//01  7A               0->Q
//02  30 00            00->R(P).0

// Counter program 2 (Low RAM $0081 R/W)
//0000 F8 81             8 (   2) START   LDI     #IOR    ;D <- $81  $0081
//0002 A3                9 (   2)         PLO     3       ;R(3).0 <- D
//0003 E3               10 (   2)         SEX     3       ;X <- 3
//0004 84               11 (   2) LOOP1   GLO     4       ;D <- R(4).0
//0005 53               12 (   2)         STR     3       ;M(R(3)) <- D
//0006 61               13 (   2)         OUT     1       ;BUS <- M(R(3)); R(3)++
//0007 23               14 (   2)         DEC     3       ;R(3)--
//0008 14               15 (   2)         INC     4       ;R(4)++
//0009 30 04            16 (   2)         BR      LOOP1   ;Branch to LOOP1

// Counter program 3  (High RAM $8008 R/W)
//0000 F8 80             8 (   2) START   LDI     /RAM    ;D <- $80  $8007
//0002 B3                9 (   2)         PHI     3       ;R(3).1 <- D
//0003 F8 08            10 (   2)         LDI     #RAM    ;D <- $07
//0005 A3               11 (   2)         PLO     3       ;R(3).0 <- D
//0006 E3               13 (   2)         SEX     3       ;X <- 3
//0007 84               14 (   2) LOOP1   GLO     4       ;D <- R(4).0
//0008 53               15 (   2)         STR     3       ;M(R(3)) <- D
//0009 61               16 (   2)         OUT     1       ;BUS <- M(R(3)); R(3)++
//000A 23               17 (   2)         DEC     3       ;R(3)--
//000B 14               18 (   2)         INC     4       ;R(4)++
//000C 30 07            19 (   2)         BR      LOOP1   ;Branch to LOOP1

// memory move program 4
// $0000-$0030 -> $8008-$8038 + OUT 1
//0000-                  7        SOURCE  .EQ     $0000
//0070-                  9        MAXADD  .EQ     $0030
//0080-                  8        DESTIN  .EQ     $8008
//00F0-                 10        MAXDUMP .EQ     $8039
//0000-                 13        PROGPC  .EQ     0
//
//0000-F8 00            33 (   2) START:  LDI     /BEG
//0002-B9               34 (   2)         PHI     R9
//0003-F8 07            35 (   2)         LDI     #BEG
//0005-A9               36 (   2)         PLO     R9
//0006-D9               37 (   2)         SEP     R9
//0007-                 38
//0007-F8 00            39 (   2) BEG:    LDI     /SOURCE
//0009-BA               40 (   2)         PHI     RA
//000A-F8 00            41 (   2)         LDI     #SOURCE
//000C-AA               42 (   2)         PLO     RA
//000D-                 43
//000D-F8 80            44 (   2)         LDI     /DESTIN
//000F-BB               45 (   2)         PHI     RB
//0010-F8 08            46 (   2)         LDI     #DESTIN
//0012-AB               47 (   2)         PLO     RB
//0013-                 48
//0013-4A               49 (   2) REPEAT: LDA     RA
//0014-5B               50 (   2)         STR     RB
//0015-1B               51 (   2)         INC     RB
//0016-8A               52 (   2)         GLO     RA
//0017-FB 31            53 (   2)         XRI     #MAXADD+1
//0019-3A 13            54 (   2)         BNZ     REPEAT
//001B-                 55
//001B-F8 80            56 (   2) DUMP:   LDI     /DESTIN
//001D-BB               57 (   2)         PHI     RB
//001E-F8 08            58 (   2)         LDI     #DESTIN
//0020-AB               59 (   2)         PLO     RB
//0021-EB               60 (   2)         SEX     RB
//0022-61               61 (   2) REPEAT2: OUT    1
//0023-8B               62 (   2)         GLO     RB
//0024-FB 39            63 (   2)         XRI     #MAXDUMP+1
//0026-3A 22            64 (   2)         BNZ     REPEAT2
//0028-                 65
//0028-                 66
//0028-30 28            67 (   2) ENDLOOP: BR ENDLOOP

const unsigned char rom[ROM_SIZE] = {
#ifdef false
    // Program 1: Blink
    0x7b, 0x7a, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff, // 0000

    // Program 2: Counter2 (Low RAM $0081 R/W)
    0xf8, 0x81, 0xa3, 0xe3, 0x84, 0x53, 0x61, 0x23, // 0000
    0x14, 0x30, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0010
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            
    // Program 3: Counter3 (High RAM $8008 R/W)
    0xf8, 0x80, 0xb3, 0xf8, 0x08, 0xa3, 0xe3, 0x84, // 0000
    0x53, 0x61, 0x23, 0x14, 0x30, 0x07, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0010
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0020
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            
    // Program 4: memory move and dump $0000 -> $8008 $30byte
    0xF8, 0x00, 0xB9, 0xF8, 0x07, 0xA9, 0xD9, 0xF8, // 0000
    0x00, 0xBA, 0xF8, 0x00, 0xAA, 0xF8, 0x80, 0xBB,
    0xF8, 0x08, 0xAB, 0x4A, 0x5B, 0x1B, 0x8A, 0xFB, // 0010
    0x31, 0x3A, 0x13, 0xF8, 0x80, 0xBB, 0xF8, 0x08,
    0xAB, 0xEB, 0x61, 0x8B, 0xFB, 0x39, 0x3A, 0x22, // 0020
    0x30, 0x28, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff

    // Program 5: Save register (Save area $8000-$801F)
    0xF8, 0x00, 0xB0, 0xF8, 0x80, 0xB1, 0xF8, 0x1E, // 0000
    0xA1, 0xF8, 0xA0, 0xB4, 0xE1, 0xF8, 0xD0, 0x51,
    0xF3, 0x3A, 0x28, 0x21, 0x94, 0xFC, 0x70, 0x33, // 0010
    0x1B, 0xFC, 0x21, 0xFC, 0x7F, 0xB4, 0x51, 0xF3,
    0x3A, 0x28, 0xD1, 0x51, 0x21, 0x21, 0x30, 0x0D, // 0020
    0x30, 0x28, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
#endif
            
// Program 6: IDIOT (0000-03FF)
// ORG:0000 EFHI:0 QHI:0
0x71,0x00,0xF8,0xFF,0xB4,0xF8,0xFF,0xA4,0x54,0x04,0xFB,0xFF,0xC6,0x54,0x04,0x32,
0x1A,0x94,0x32,0xDD,0xA4,0x24,0x84,0xB4,0x30,0x05,0xF8,0xDF,0xA4,0xE4,0x8F,0x73,
0x9F,0x73,0x8E,0x73,0x9E,0x73,0x8D,0x73,0x9D,0x73,0x8C,0x73,0x9C,0x73,0x8B,0x73,
0x9B,0x73,0x8A,0x73,0x9A,0x73,0x89,0x73,0x99,0x73,0x88,0x73,0x98,0x73,0x87,0x73,
0x97,0x73,0x86,0x73,0x96,0x73,0x85,0x73,0x95,0x73,0x73,0x73,0x83,0x73,0x93,0x73,
0x82,0x73,0x92,0x73,0x81,0x73,0x91,0x73,0x80,0x73,0x90,0x73,0xF8,0x00,0xB3,0x3F,
0x63,0xF9,0x04,0x3E,0x67,0xF9,0x30,0x73,0x93,0x3D,0x6D,0xF9,0x02,0x3C,0x71,0xF9,
0x10,0x73,0x93,0xC5,0xF9,0x01,0x73,0xF8,0x01,0xCC,0xF8,0x00,0x73,0x93,0x73,0x7C,
0x00,0x73,0x78,0x24,0xF8,0xC5,0xA1,0x90,0xB1,0xF8,0x8D,0xA1,0xD1,0xF8,0x00,0x54,
0xF8,0xCA,0xA5,0x91,0xFC,0x03,0xB5,0xF8,0xA0,0xA4,0xF8,0x18,0xA3,0x45,0x54,0x14,
0x23,0x83,0x3A,0x9D,0x94,0xB2,0xF8,0xC3,0xA4,0xF8,0xBB,0x73,0x91,0x73,0x91,0xFC,
0x01,0xB5,0xF8,0xF8,0xA5,0xF8,0xFE,0xA3,0x91,0xB3,0xD3,0xE2,0x22,0x73,0x94,0x73,
0x84,0x52,0xE4,0x30,0x02,0xF8,0x10,0x73,0xF8,0xC9,0xA4,0x42,0x73,0x42,0x73,0xF8,
0xBB,0xA4,0x42,0x54,0xF8,0xC5,0xA4,0x82,0x73,0x92,0x73,0x30,0x90,0x90,0xFC,0x01,
0xB3,0xF8,0xF8,0xA3,0xE2,0xD3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD3,0x9E,
0xF6,0xAE,0x2E,0x43,0xFF,0x01,0x3A,0xF4,0x8E,0x32,0xEE,0x23,0x30,0xF2,0x93,0xBC,
0xF8,0xEF,0xAC,0xF8,0x00,0xAE,0xAF,0x3E,0x07,0x36,0x09,0xF8,0x03,0xFF,0x01,0x3A,
0x0D,0x8F,0x3A,0x17,0x3E,0x19,0x1F,0x3E,0x1E,0x1E,0xF8,0x07,0x30,0x0D,0x2E,0x2E,
0x8E,0xF9,0x01,0xBE,0xDC,0x0C,0x36,0x2C,0x9E,0xFA,0xFE,0xBE,0xDC,0x26,0xD5,0xFC,
0x07,0x33,0x37,0xFC,0x0A,0x33,0x88,0xFC,0x00,0x9F,0xD5,0xF8,0x80,0x38,0x83,0xC8,
0xF8,0x00,0xAF,0xF8,0x80,0xBF,0xE3,0x8F,0xC6,0x67,0x80,0x36,0x4B,0x3E,0x4D,0xDC,
0x02,0x3E,0x4D,0x8F,0xC6,0x67,0x40,0xE2,0xE2,0x9E,0xF6,0x33,0x62,0x3E,0x61,0x7A,
0xC8,0x7B,0xC4,0xC7,0xE2,0xE2,0xE2,0xC4,0xC4,0xDC,0x07,0x1E,0x9F,0xF6,0xBF,0x33,
0x78,0xF9,0x80,0x36,0x57,0xBF,0x30,0x59,0x7B,0x32,0x43,0x8F,0xFE,0x3B,0x39,0x9F,
0xFF,0x41,0x3B,0x2F,0xFF,0x06,0x33,0x37,0xFE,0xFE,0xFE,0xFE,0xFC,0x08,0xFE,0xAE,
0x8D,0x7E,0xAD,0x9D,0x7E,0xBD,0x8E,0xFE,0x3A,0x8F,0x30,0x39,0x9E,0xAE,0x38,0xD5,
0x45,0x38,0x46,0x38,0x9F,0xAD,0xFB,0x0A,0x3A,0xBE,0xF8,0x5B,0x30,0xC0,0x9F,0xF6,
0xF6,0xF6,0xF6,0xFC,0xF6,0xC7,0xFC,0x07,0xFF,0xC6,0xAD,0xF8,0x1B,0xC8,0xF8,0x0B,
0xAF,0x8E,0xCE,0xDC,0x17,0x7A,0xDC,0x07,0xC4,0xC4,0xC4,0xC4,0xC4,0xC4,0x2F,0xF5,
0x8D,0x76,0xAD,0xCF,0x7A,0xC8,0x7B,0xC4,0x8F,0xFA,0x0F,0x3A,0xC6,0x8F,0xFC,0xFB,
0xAF,0x3B,0x9F,0xFF,0x1B,0x32,0x9F,0xF8,0x00,0x33,0xF5,0x9F,0xFA,0x0F,0xFC,0xF6,
0xC7,0xFC,0x07,0xFF,0xC6,0xAD,0x30,0xC1,0x95,0xB3,0xF8,0x9C,0xA3,0xD3,0x0D,0xD3,
0x0A,0xD3,0x49,0xD3,0x44,0xD3,0x49,0xD3,0x4F,0xD3,0x54,0xD3,0x2F,0xD3,0x34,0x95,
0xFF,0x01,0xB3,0xF8,0x9C,0xA3,0xD3,0x0D,0xD3,0x0A,0xD3,0x2A,0xF8,0x00,0xBD,0xAD,
0xF8,0x3B,0xA3,0xD3,0xFB,0x24,0x32,0xCC,0xFB,0x05,0xA8,0xCE,0xFB,0x1E,0x3A,0x1C,
0xD3,0xFB,0x52,0x3A,0x40,0xF8,0xB8,0xAA,0x92,0xBA,0xF8,0x28,0xAD,0xD3,0x30,0x5C,
0xFB,0x1F,0x3A,0x1C,0xD3,0x3B,0x44,0xD3,0x33,0x47,0xFB,0x20,0x3A,0xC1,0x9D,0xBA,
0x8D,0xAA,0x88,0x32,0xAB,0xF8,0x00,0xAD,0xBD,0xD3,0x33,0x59,0x8D,0xA8,0x9D,0xB8,
0x9F,0xFB,0x21,0x32,0xFA,0xFB,0x01,0x3A,0x6C,0xD3,0x30,0x61,0xFB,0x2D,0x3A,0xC1,
0xF8,0x9C,0xA3,0xD3,0x0A,0x36,0xC1,0x9A,0xBF,0xF8,0xAE,0xA3,0xD3,0x8A,0xBF,0xF8,
0xAE,0xA3,0xD3,0xD3,0x20,0x4A,0xBF,0xF8,0xAE,0xA3,0xD3,0x28,0x88,0x3A,0x92,0x98,
0x32,0x0F,0x8A,0xFA,0x0F,0x3A,0x9D,0xD3,0x3B,0xD3,0x0D,0x30,0x73,0xF6,0x33,0x85,
0x30,0x83,0xD3,0x3B,0xA2,0xD3,0x3B,0xC1,0x8D,0x5A,0x1A,0xD3,0x33,0xA5,0xFB,0x0D,
0x32,0x0F,0xFB,0x21,0x32,0xA2,0xFB,0x17,0x3A,0xAB,0xD3,0xFB,0x0D,0x3A,0xBA,0x30,
0x44,0xF8,0x9C,0xA3,0xD3,0x0D,0xD3,0x0A,0xD3,0x3F,0x30,0x0F,0xD3,0xFB,0x52,0xA8,
0xCE,0xFB,0x02,0x3A,0x1C,0xD3,0x33,0xD5,0xFB,0x0D,0x3A,0xC1,0x9D,0xB0,0x8D,0xA0,
0xF8,0x9C,0xA3,0xD3,0x0A,0x88,0x32,0xFE,0xF8,0xBB,0xA1,0x95,0xFF,0x02,0xB1,0xE5,
0x70,0x00,0x30,0xFF,0x30,0xC1,0xC4,0xC4,0x30,0x0F,0x95,0xFC,0x01,0xB5,0x92,0xB3,
0xF8,0xBF,0xA2,0xE2,0xF8,0xC1,0x52,0x80,0xAD,0xFA,0x0F,0xFE,0xF4,0xA3,0x03,0x73,
0xF8,0xA8,0x53,0x23,0x03,0x73,0x92,0x53,0xF0,0xCE,0x7B,0x38,0x7A,0xF8,0xC0,0x73,
0xF0,0xBD,0x22,0x72,0x73,0xF8,0xF8,0x73,0xF0,0xF6,0xF8,0xA2,0x73,0xF8,0xC5,0xA3,
0x03,0x73,0xF8,0xF8,0x73,0xF8,0xC0,0xA2,0xF0,0xB0,0x9D,0x52,0x12,0xF0,0xA0,0x8D,
0x52,0x12,0x72,0xB1,0x72,0xA1,0x60,0x60,0x72,0xB3,0x72,0xA3,0x72,0xB4,0x72,0xA4,
0x60,0x60,0x72,0xB6,0x72,0xA6,0x72,0xB7,0x72,0xA7,0x72,0xB8,0x72,0xA8,0x72,0xB9,
0x72,0xA9,0x72,0xBA,0x72,0xAA,0x72,0xBB,0x72,0xAB,0x72,0xBC,0x72,0xAC,0x72,0xBD,
0x72,0xAD,0x72,0xBE,0x72,0xAE,0x72,0xBF,0xF0,0xAF,0x30,0x9E,0xD3,0xFB,0x4D,0x3A,
0x86,0xBD,0xAD,0xD3,0x33,0x83,0xFB,0x0D,0x3A,0xF0,0x9A,0xB3,0x8A,0xA3,0x9D,0xBF,
0x8D,0xAF,0x93,0x3A,0x96,0x83,0x32,0xAF,0x23,0x9F,0x3A,0x9D,0x8F,0xC8,0x92,0xB5,
0x2F,0x3A,0x92,0x98,0x3A,0xA7,0x88,0x32,0xF4,0x4A,0x5D,0x1D,0x28,0x30,0xA3,0x98,
0xB3,0x88,0xA3,0xC8,0x1A,0x1D,0x23,0x93,0x3A,0xB4,0x83,0x3A,0xB4,0xED,0x98,0x3A,
0xC2,0x88,0x32,0xF4,0x0A,0x73,0x2A,0x28,0x30,0xBE,0xF8,0xC0,0xA2,0x72,0xCE,0x70,
0x38,0x71,0x15,0xC8,0x30,0xB3,0xF8,0xCA,0xA2,0x42,0xB5,0x02,0xA5,0xF8,0xC4,0xA2,
0x02,0xB2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x95,0xFF,0x01,0xB5,0x95,0xFF,0x01,0xB5,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x7C            

};
