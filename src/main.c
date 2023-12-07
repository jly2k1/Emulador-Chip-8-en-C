#include "../include/chip8.h"
#include <SDL2/SDL.h>

int main()
{
    struct chip8 c8;
    struct graficos graf;
    char *file = "roms/test_opcode.ch8";

    ini_cmptes(&c8); //Inicializacion de componentes del chip-8

    leerROM(&c8, file);
    
    ventana(&graf); //Creamos la ventana donde se visualizaran los graficos.
    
    SDL_Event e;

    int quit = 0;

    //Gameloop

    while(!quit)
    {
        cicloFDE(&c8);

	//dibujado y actualizacion de la pantalla (SDL)
        if(c8.drawFlag)
	{
	    actualizar(&c8, &graf);
	}	
	//Teclas (SDL)
	
        while(SDL_PollEvent(&e) != 0)
	{
	    if(e.type == SDL_QUIT)
	    {
	        quit = 1;
		cerrar(&graf);
	    }
	}

	SDL_Delay(16); //Esta funcion hace que el programa corra a una velocidad determinada de FPS.
    }
    
   return 0; 
}
