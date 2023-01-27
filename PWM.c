#include "lib/include.h"

/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
  junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
  cuya frecuencia sea de 50Hz

  pag. 1672
  generador 0 --> PWM1 - PF1
  generador 1 --> PWM2 - PF2
  generador 2 --> PWM4 - PG0*/

extern void Configura_Reg_PWM0(uint16_t freq)
{
   // AQUÍ HABILITAMOS EL PWM0 / MI TIVA SOLO TIENE 1 P. 398
    SYSCTL->RCGCPWM |= (1<<0); 
    
    //HABILITAMOS GPIO DEL PORT F Y G (PG0, PF1 Y PF2) P.1233 PINES CORRESPONDIENTES A CADA MODULO Y SU GENERATOR 
    SYSCTL->RCGCGPIO |= (1<<6) | (1<<5);
    
    //AQUÍ HABILITAMOS LA FUNCION ALTERNATIVA P.770
    GPIOG_AHB->AFSEL |= (1<<0); // PG0 
    GPIOF_AHB->AFSEL |= (1<<1) | (1<<2);//PF1 PF2
    
    //HABILITAMOS COMO SALIDA LOS PINES DEL PWM 
    //1->SALIDA
    //0->ENTRADA
    GPIOG_AHB->DIR = (1<<0);
    GPIOF_AHB->DIR = (1<<1) | (1<<2);
   
    //LE DECIMOS QUE TRABAJAREMOS CON LA FUNCIÓN ALTERNATIVA
    GPIOG_AHB->PCTL |= (GPIOG_AHB->PCTL&0xFFFFFFF0) | 0x00000006;
    GPIOF_AHB->PCTL |= (GPIOF_AHB->PCTL&0xFFFFF00F) | 0x00000660;
    
   //CON EL DEN LE DECIMOS SI ES DIGITAL O ANALOG. (1<<DIGI.) / (0<<ANAL.)
    GPIOG_AHB->DEN |= (1<<0); 
    GPIOF_AHB->DEN |= (1<<1) | (1<<2);
    
    //EL 1 EN EL BIT 8 ACTIVA EL DIVISOR | A LOS BITS 0,1,2 LE PONESMOS EL DOS EN BINARIO (010)  QUE INDICA QUE SE DIVIDIRÁ ENTRE 8                                                                                                                           //  frec, tarjeta / frec PWM = cuentas
    PWM0->CC = (1<<8) | (0<<2) | (1<<1) | (0<<0);//

    //AQUÍ DESACTIVAMOS EL GENERADOR, CON LA TABLA DE LA P. 1708 VEMOS EL BIT QUE SE LE ASIGNARÁ. CONSULTAR P.1672 PARA VER QUE GENERADOR USAREMOS
   // PRIMERO DEBEMOS DESATIVAR EL GENERADOR  PORQUE LO VAMOS A CONFIG, EL 0 DESACTIVA
    PWM0->_0_CTL = (0<<0);
    PWM0->_1_CTL = (0<<0);
    PWM0->_2_CTL = (0<<0);
    
    //RECORDAR QUE AQUÍ DEBEMOS IR A P.1687 PARA VER EL COMPARADOR QUE USA EL PWM0
    PWM0->_0_GENB = 0x0000080C;        
    PWM0->_1_GENA = 0x0000008C;
    PWM0->_2_GENA = 0x0000008C;    
   
   //EN ESTE BLOQUE SE CONFIGURAN LAS CARGAS
   /*
    CUENTAS=F.RELOJ/F. PWM
    DONDE 20MHz -> F. TIVA
    8 -> DIVISOR 
    50 - > F. PWM 
    CUENTAS=20 000 000 / 8 = 2 500 000
    2 500 000 / 50 = 50 000 CUENTAS
   */ 
    PWM0->_0_LOAD = 50000;
    PWM0->_1_LOAD = 50000;
    PWM0->_2_LOAD = 50000;
    
    //ESTABLECEMOS CICLO DE TRABAJO, EN ESTE CASO ES AL 70%, POR ESO 35000. EL 70% DE 50000 ES 35 0000 
    PWM0->_0_CMPB = 35000;  
    PWM0->_1_CMPA = 35000;       
    PWM0->_2_CMPA = 35000;
     
    //SE VUELVE A ENCENDER LOS GENERADORES
    PWM0->_0_CTL = (1<<0);
    PWM0->_1_CTL = (1<<0);
    PWM0->_2_CTL = (1<<0);
    
    //HABILITAMOS PWM1, 2 Y PWM3 P. 1247 Y P. 1233
    PWM0->ENABLE = (1<<1) | (1<<2) | (1<<4);          
}