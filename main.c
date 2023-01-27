#include "lib/include.h"
//_______________________________________________________________________________EXPERIMENTO 3
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz , utilizando el uart de la practica 3
 * se enviara dato desde interfaz de simulink para controlar la intensidad luminosa 
 * usando un led RGB externa 
 *
 */

//VARIABLES DECLARADAS CON VALOR DE CÓDIGO ASCII

volatile uint8_t rojo = 114; // PF1
volatile uint8_t verde = 103;// PG0 
volatile uint8_t azul = 98;  // PF2 
volatile uint8_t color;


int main(void)
{
 //__________________________LLAMADAS A FUNCIÓN  
    Configurar_PLL(); 
    Configurar_UART0();
    Configura_Reg_PWM0(8);


    while(1) 
    {
        color = readChar();

        switch (color)
        {
            case 'r':
                while (rojo == 114)
                {
                    rojo = (int)readChar();//AQUÍ EL UART LEE EL PORCENTAJE QUE SIMU LE MANDA       
                }
                PWM0->_0_CMPB = 50000-((int)(rojo*15000)/30);
                //    #PWM_ESTABLECE C. WORK = CUENTAS-LO MANDADO 
                rojo = 114;
            break;

            case 'g':
                while (verde == 103)
                {
                    verde = (int)readChar();       
                }
                PWM0->_2_CMPA = 50000-((int)(verde*15000)/30);
                verde = 103;
            break;

            case 'b':
                while (azul == 98)
                {
                    azul = (int)readChar();   
                }
                PWM0->_1_CMPA = 50000-((int)(azul*15000)/30);
                azul = 98;
            break;

        }
    }
}
