#include "../include/chip8.h"
#include <stdlib.h>

void ini_cmptes(struct chip8 *c8)
{
    //Limpiar memoria.

    for(int i = 0; i < 4096; i++)
    {
        c8->RAM[i] = 0;
    }

    //Limpiar registros y pila.

    for(int i = 0; i < 16; i++)
    {
        c8->V[i] = 0;
	c8->Pila[i] = 0;
    }

    c8->PC = 0x200;
    c8->SP = 0;
    c8->I = 0;

    //Limpiar pantalla;
    
    for(int i = 0; i < 62*32; i++)
    {
        c8->pantalla[i] = 0;
    }

    //Cargar fuentes.
    //
    // 

    //Limpiar timers.

    c8->temp_delay = 0;
    c8->temp_sonido = 0;

    //Limpiar entrada.
    //
    //
    c8->opcode = 0;
}

void cicloFDE(struct chip8 *c8)
{
     //Variables para guardar partes especificas que vienen en el opcode
     //esto es registros y direcciones de 12 y 8 bits.	
     uint16_t nnn = 0; //direcion de 12 bits
     uint8_t nn = 0; //direccion de 8 bits
     uint8_t reg_x = 0; //registro X
     uint8_t reg_y = 0; //registro Y    

     //Obtenemos el opcode. (Fetch)
     c8->opcode = c8->RAM[c8->PC] << 8 | c8->RAM[c8->PC + 1];	
     
     //Decodificar opcode. (Decode)

     switch(c8->opcode & 0xF000)
     {
         case 0x0000: //Ejecutar instruccion (Execute) (Para el grupo de instrucciones 0x0000)
   	     switch(c8->opcode & 0x00FF)
	     {
	         case 0x00E0: //Limpiar pantalla.
			 for(int i = 0; i < 64*32; i++)
			 {
			     c8->pantalla[i] = 0;
			 }
		 break;
		 case 0x00EE: //Retorno de un subrutina.
		         c8->SP--; //se decrementa el SP en 1.
			 //En PC se guarda la direccion a la que apunta SP en la pila en ese momento.	   
			 c8->PC = c8->Pila[c8->SP]; 
			 //Al ocurrir eso el PC se incrementa en dos.
			 c8->PC += 2;
		 break;
	     }
	 break;
	 case 0x1000: //Intruccion de salto. Salta a la direccion 'nnn' es decir, una direccion de 12 bits.
	     nnn = c8->opcode & 0x0FFF; //obtenemos dicha direccion incrustada en el opcode
	     
	     c8->PC = nnn; //establecemos el PC con dicha direccion
	 break;
	 case 0x2000:
	     c8->Pila[c8->SP] = c8->SP;
	     c8->SP++;

	     nnn = c8->opcode & 0x0FFF;

	     c8->PC = nnn;
	 break;
	 case 0x3000: //Salta a la siguiente instruccion si el registro Vx es igual a la direccion nn (!)
             //obtenemos el registro en cuestion
     	     reg_x = (c8->opcode & 0x0F00) >> 8;
	     nn = c8->opcode & 0x00FF; //obtenemos la direccion nn.

	     if(c8->V[reg_x] == nn) //hacemos la comparacion.
	     {
	        c8->PC += 2;
	     }
	 break;
	 case 0x4000: //Salta a la siguiente instruccion si el registrr Vx es diferente de la direccion nn 
	     reg_x = (c8->opcode & 0x0F00) >> 8;
	     nn = c8->opcode & 0x00FF;

	     if(c8->V[reg_x] != nn)
	     {
		 c8->PC += 2;    
	     }
	 break;
	 case 0x5000: //Salta a la siguiente instruccion si los registros X e Y son iguales.
	     reg_x = (c8->opcode & 0x0F00) >> 8;
	     reg_y = (c8->opcode & 0x00F0) >> 4; //obtenemos el registro Y.

	     if(c8->V[reg_x] == c8->V[reg_y])
	     {
	         c8->PC += 2;
	     }
	 break;
	 case 0x6000: //Coloca el valor de la direccion 'nn' en el registro X.
	     reg_x = (c8->opcode & 0x0F00) >> 8;
	     nn = c8->opcode & 0x00FF;

	     c8->V[reg_x] = nn;
	 break;
	 case 0x7000: //Suma al registro X lo que haya en 'nn' y guarda el resultado en el registro X.
	     reg_x = (c8->opcode & 0x0F00) >> 8;
	     nn = c8->opcode & 0x00FF;

	     c8->V[reg_x] += nn;
	 break;
	 case 0x8000:
	     switch(c8->opcode & 0x000F)
	     {
	         case 0x0000: //Agrega el valor del registro Y al registro X.
			 reg_x = (c8->opcode & 0x0F00) >> 8;
			 reg_y = (c8->opcode & 0x00F0) >> 4;

			 c8->V[reg_x] = c8->V[reg_y];
		 break;
		 case 0x0001: //Realiza la operacion OR entre los registros X e Y, guarda el resultado en X.
		 	 reg_x = (c8->opcode & 0x0F00) >> 8;
			 reg_y = (c8->opcode & 0x00F0) >> 4;

			 c8->V[reg_x] |= c8->V[reg_y];
		 break;
		 case 0x0002: //Realiza la operacion AND entre los registros X e Y, guarda el resultado en X.
		 	 reg_x = (c8->opcode & 0x0F00) >> 8;
			 reg_y = (c8->opcode & 0x00F0) >> 4;

			 c8->V[reg_x] &= c8->V[reg_y];
		 break;
		 case 0x0003: //Realiza la operacion XOR entre los registros X e Y, guarda el resultado en X.
		 	 reg_x = (c8->opcode & 0x0F00) >> 8;
			 reg_y = (c8->opcode & 0x00F0) >> 4;

			c8->V[reg_x] ^= c8->V[reg_y]; 
		 break;
		 /*Realiza la suma de los registros X e Y, guarda el resultado en X y si hay un acarreo,
		  * establece el registro F en 1. En caso de que no se de el acarreo, F se establece en
		  * 0.*/
		 case 0x0004: //(!)
			 reg_x = (c8->opcode & 0x0F00) >> 8;
			 reg_y = (c8->opcode & 0x00F0) >> 4;

			 c8->V[reg_x] += c8->V[reg_y];

			 /*Para saber si hay un acarreo, hay que comprobar si se supero el valor máximo
			  * que pueden guardar nuestros registros de 8 bits. Eso es, comprobar si su
			  * suma supera el valor 255*/
			 if(c8->V[reg_x] + c8->V[reg_y] > 255)
			 {
			     c8->V[0xF] = 1;
			 }
			 else
			 {
			     c8->V[0xF] = 0;
			 }
		   break;
		   /*Realiza la resta de los registros X e Y, guarda el resultado en X y si hubo un 'prestamo'
		    * establece el registro de F en 0. En caso de que no se diese un prestamo, F se establece
		    * en 1.*/
		   case 0x0005:
		   	 reg_x = (c8->opcode & 0x0F00) >> 8;
			 reg_y = (c8->opcode & 0x00F0) >> 4;

		 	 /*Para saber si va haber un prestamo, comprobamos si el valor del registro X
			  * es mayor al del Y. En caso de serlo no habrá prestamo y en caso contrario
			  * si lo hará.*/
			 if(c8->V[reg_x] > c8->V[reg_y]) //(!)
			 {
			     c8->V[0xF] = 1;
			 }
			 else
			 {
			     c8->V[0xF] = 0;
			 }

			 c8->V[reg_x] -= c8->V[reg_y];
		   break;
		   /*Guarda el bit menos significativo del registro X en el registro F y desplaza un 1 bit
		    * hacia la derecha el registro X.*/
		   case 0x0006: 
		         reg_x = (c8->opcode & 0x0F00) >> 8;
			 
			 //obtenemos el dicho bit utilizando el operador '&' y el valor 0x1.
			 c8->V[0xF] = c8->V[reg_x] & 0x1;
			 
			 c8->V[reg_x] >>= 1; //hacemos el desplazamiento
		   break;
		   /*Comprueba si el registro Y es mayor al registro X. En caso de serlo el registro
		    * F = 1 y en caso de no serlo F = 0. Se resta el registro Y con X y se guarda
		    * el resultado en X.*/
		   case 0x0007:
		         reg_x = (c8->opcode & 0x0F00) >> 8;
			 reg_y = (c8->opcode & 0x00F0) >> 4;

			 if(c8->V[reg_y] > c8->V[reg_x])
			 {
			     c8->V[0xF] = 1;
			 }
			 else
			 {
			     c8->V[0xF] = 0;
			 }

			 c8->V[reg_x] = c8->V[reg_y] - c8->V[reg_x];
		   break;
		   /*Esta instruccion es similar a 8XY6, solo que aqui obtenemos el bit mas significativo
		    * de X y lo guardamos en F*/
		   case 0x000E:
		         reg_x = (c8->opcode & 0x0F00) >> 8;
			 
			 //obtenemos el bit mas significativo
			 c8->V[0xF] = (c8->V[reg_x] & 0x80) >> 7;
                         
			 //desplazamos una posicion hacia la izquierda.
			 c8->V[reg_x] <<= 1;
		   break;
	     }
	 case 0x9000: //Salta a la siguiente instruccion si el registro X es diferente de Y.
	     reg_x = (c8->opcode & 0x0F00) >> 8;
	     reg_y = (c8->opcode & 0x00F0) >> 4;

	     if(c8->V[reg_x] != c8->V[reg_y])
	     {
	         c8->PC += 2;
	     }
	 break;
	 case 0xA000: //Establece en el registro indice la direccion 'nnn'.
	     nnn = c8->opcode & 0x0FFF;

	     c8->I = nnn;
	 break;
	 case 0xB000: //Salta a la ubicacion resultante de la suma del registro 0 mas la direccion nnn.
	     nnn = c8->opcode & 0x0FFF;

	     c8->PC = c8->V[0x0] + nnn;
	 break; //Se genera un numero aleatorio y con un '&' con la direccion 'nn' y el resultado se guarda en X
	     uint8_t valor_aleatorio = rand() + 256;
	     
	     reg_x = (c8->opcode & 0x0F00) >> 8;
	     nn = c8->opcode & 0x00FF;

	     c8->V[reg_x] = valor_aleatorio & nn;
	 break;
	 case 0xD000:
	 //instruccion que pinta un sprite en la pantalla.
	 break;
	 case 0xE000:
	     switch(c8->opcode & 0x000F)
	     {
	 	    case 0x000E:
			  //saltar a la siguiente instruccion si X coincide con la tecla presionada.
	 	    break;
		    case 0x0001:
		         //saltar a la siguiente instruccion si X no coincide con la tecla presionada.
		    break;
	     }
	 case 0xF000:
	     switch(c8->opcode & 0x000F)
	     {
	     	    case 0x0007:
			  reg_x = (c8->opcode & 0x0F00) >> 8;
			  
			  c8->V[reg_x] = c8->temp_delay;
		    break;	  
	     } 
     }
} 
