#include "lib/include.h"

//P.1163 PA CONFIGURAR UART
extern void Configurar_UART0(void)
{
    SYSCTL->RCGCUART  = (1<<0); // HABILITACIÓN DE RELOJES, HABILITAMOS EL MODULO 0 
    SYSCTL->RCGCGPIO |= (1<<0);///Paso 2 (RCGCGPIO) pag.382 Enable clock port A     
    GPIOA_AHB->AFSEL = (1<<1) | (1<<0);// (GPIOAFSEL) pag.770 Enable alternate function
    // LE INDICAMOS LA F. ALTERNATIVA, P.1808 
    GPIOA_AHB->PCTL = (GPIOA_AHB->PCTL&0xFFFFFF00) | 0x00000011;    // (1<<0) | (1<<4); //P0 Y P1 SERAN EL UART P. 787
    // GPIO Digital Enable (GPIODEN)
    GPIOA_AHB->DEN = (1<<0) | (1<<1);//PA1 PA0    
    //UART0 UART Control (UARTCTL) P.1188 DISABLE!! DESACTIVA Y LIMPIA REGISTRO
    UART0->CTL = (0<<9) | (0<<8) | (0<<0); 
    //          (0<<TX) | (0<<RX)| (0<<UART)  
    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 20,000,000 / (16 * 9600) = 130.2
    UARTFBRD[DIVFRAC] = integer(0.2 * 64 + 0.5) = 14
    */
    UART0->IBRD = 130;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.1172
    UART0->FBRD = 14;
    //  UART Line Control (UARTLCRH) pag.1186 Serial parameters such as data length, parity, and stop bit selection are implemented in this register.
    UART0->LCRH = (0x3<<5)|(1<<4);      
    //  UART Clock Configuration(UARTCC) pag.1213
    UART0->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.1188
    UART0->CTL = (1<<0) | (1<<8) | (1<<9); //UNA VEZ CONFIGURADO, HABILITAMOS
}


//____________________________________________________________________
extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART0->FR & (1<<4)) != 0 );
    v = UART0->DR & 0xFF;
    c = v;
    return c;
}

//___________________________________________________________________
extern void printChar(char c)
{
    while((UART0->FR & (1<<5)) != 0 );
    UART0->DR = c;
}

//__________________________________________________________________
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

//___________________________________________________________________
extern char * readString(char delimitador)
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;
}