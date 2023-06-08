
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements
#define _XTAL_FREQ 4000000 

#define step RC7
#define dir RC6
#define enable RD4
#define up RA5
#define down RE2
#define right RE0
#define left RE1

#define RS RC4
#define EN RC5
#define D4 RD0
#define D5 RD1
#define D7 RD2
#define D6 RD3
// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include "lcd.h";
#include <math.h>
#include <string.h>
#include <stdlib.h>


char num[4];
char s[16];
unsigned char timer;
int timerPrueba;
char modo;
char seleccion=0;
double ms=0;
double minuto=0;
double mL=0;
int tiempo;

void interrupt ISR()
{
    
    if(INTCONbits.INTF)
    {
        while(!PORTBbits.RB0){}
        enable=~enable;
        tiempo=0;
        T1CONbits.TMR1ON=~enable;
        INTCONbits.TMR0IE=~enable;
        INTCONbits.INTF=0;
    }
    else if(INTCONbits.TMR0IF)
    {
        step=~step;
        TMR0=timer;
        INTCONbits.TMR0IF=0;
    }
    else if(PIR1bits.TMR1IF)
    {
        tiempo++;
        PIR1bits.TMR1IF=0;
    }
    else if(INTCONbits.RBIF)
    {
        PORTB=PORTB;
        PORTB=0x00;
        for(int i=2;i<=8;i*=2)
        {
            PORTB=~i;
            switch(PORTB)
            {
                case 0b11101101:
                    strcat(num,"1");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b11101011:
                    strcat(num,"2");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b11100111:
                    strcat(num,"3");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b11011101:
                    strcat(num,"4");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b11011011:
                    strcat(num,"5");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b11010111:
                    strcat(num,"6");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b10111101:
                    strcat(num,"7");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b10111011:
                    strcat(num,"8");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b10110111:
                    strcat(num,"9");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b01111101:
                    num[strlen(num)-1]=0;
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b01111011:
                    strcat(num,"0");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                case 0b01110111:
                    strcat(num,".");
                    PORTB=0x00;
                    INTCONbits.RBIF=0;
                    return;
                    break;
                default:
                    break;
            }
        }
        PORTB=0x00;
        INTCONbits.RBIF=0;
    } 
    
    else
    {
        PORTB=PORTB;
        PORTB=0x00;
        INTCONbits.RBIF=0;
        INTCONbits.INTF=0;
        INTCONbits.TMR0IF=0;
        INTCONbits.T0IF=0;
    }
    
}
void floatToString(float floatNum)
{
    int integer;
    int decimal;
    char string[16];
    integer=(int)floatNum;
    decimal=round((floatNum-integer)*100.0);
    sprintf(string,"%d.%d",integer,decimal);
    strcpy(s,string);
}
void Navkey()
{
    /*if(!up)
    {
        while(!up){}
        timer+=15;
    }
    else if(!down)
    {
        while(!down){}
        timer-=15;
    }*/
    if(enable)
    {
        if((!up)&&(modo==3))
        {
            while(!up){}
            if(seleccion==1)
            {
                strcpy(num,"");
            }
            seleccion=0;
        }
        else if((!down)&&(modo==3))
        {
            while(!down){}
            if(seleccion==0)
            {
                strcpy(num,"");
            }
            seleccion=1;
        }
        else if(!right)
        {
            while(!right){}
            (modo<3)? modo++:modo=3;
        }
        else if(!left)
        {
            while(!left){}
            (modo>0)? modo--:modo=0;
        }
    }
}
void Setup()
{
    ADCON1bits.PCFG3=0;
    ADCON1bits.PCFG2=1;
    ADCON1bits.PCFG1=1;
    TRISB=0b11110001;
    TRISC=0b00001111;
    TRISD=0b00000000;
    TRISAbits.TRISA5=1;
    TRISEbits.TRISE0=1;
    TRISEbits.TRISE1=1;
    TRISEbits.TRISE2=1;
    PORTB=0x00;
    PORTDbits.RD5=1;
    PORTDbits.RD6=0;
    PORTDbits.RD7=1;
    OPTION_REGbits.nRBPU=0;
    OPTION_REGbits.INTEDG=0;
    OPTION_REGbits.T0CS=0;
    OPTION_REGbits.PSA=0;
    OPTION_REGbits.PS=0b011; //Prescalador
    INTCONbits.RBIE=1;
    INTCONbits.TMR0IE=0;
    INTCONbits.INTE=1;
    PIE1bits.TMR1IE=1;
    INTCONbits.PEIE=1;
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    enable=1;
    dir=1;
    INTCONbits.INTF=0;
    INTCONbits.RBIF=0;
    INTCONbits.TMR0IF=0;
    PIR1bits.TMR1IF=0;
    __delay_ms(100);
    INTCONbits.GIE=1;
    modo=0;
}
void main(void) {
    Setup();
    TMR0=0x00;
    /*while(1)
    {
        Navkey();
        sprintf(s,"%d  ",timer);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(s);
    }*/
    while(1)
    {
        if(modo==0)
        {
            strcpy(num,"");
            timer=229;
            minuto=30;
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String(" Cancer de mama ");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String(" Abraxane: 52mL ");
        }
        else if(modo==1)
        {
            strcpy(num,"");
            timer=184;
            minuto=30;
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String(" Cancer pancreas");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String(" Abraxane: 20mL ");
        }
        else if(modo==2)
        {    
            strcpy(num,"");
            timer=199;
            minuto=30;
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Cancer de pulmon");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String(" Abraxane: 25mL ");
        }
        else if(modo==3)
        {
            if(atof(num)>9999)
            {
                num[strlen(num)-1]=0;
            }
            if(seleccion==0)
            {
                minuto=atof(num);
                Lcd_Set_Cursor(1,7);
                Lcd_Write_Char('>');
                Lcd_Set_Cursor(2,7);
                Lcd_Write_Char(' ');
            }
            else if(seleccion==1)
            {
                mL=atof(num);
                Lcd_Set_Cursor(1,7);
                Lcd_Write_Char(' ');
                Lcd_Set_Cursor(2,7);
                Lcd_Write_Char('>');
            }
            if((mL!=0)&&(minuto!=0))
            {
                ms=minuto/(mL*0.64);
                timerPrueba=round(((ms-8.22)*255)/(0.086-8.22));
            }
            if((timerPrueba<=253)&&(timerPrueba>=0))
            {
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("      ");
                timer=timerPrueba;
            }
            else
            {
                enable=1;
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("ERROR ");
            }
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Dosis:");
            Lcd_Set_Cursor(1,8);
            floatToString(minuto);
            Lcd_Write_String(s);
            Lcd_Write_String("min   ");
            Lcd_Set_Cursor(2,8);
            floatToString(mL);
            Lcd_Write_String(s);
            Lcd_Write_String("mL    ");
        }
        Navkey();   
        if(tiempo>=minuto*915)
        {
            tiempo=0;
            enable=1;
            T1CONbits.TMR1ON=0;
            INTCONbits.TMR0IE=0;
        }
    }
    return;
}