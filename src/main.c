#include "../include/chip8.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    struct chip8 c8;
    char *file = "roms/test_opcode.ch8";

    ini_cmptes(&c8);

    leerROM(&c8, file);
    /* codigo que muestra el contenido de la rom
    for(int i = 512; i < 4096; i++)
    {
        printf("%i  ", c8.RAM[i]);	

	if(i % 5 == 0)
	{
	    printf("\n");
	}
    }*/

   return 0; 
}
