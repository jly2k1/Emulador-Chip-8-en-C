#include "../include/chip8.h"

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
     }
} 
