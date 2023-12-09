#include "../include/chip8.h"
#include <time.h>

int main()
{
    struct chip8 c8;
    struct graficos graf;
    char *file = "roms/Space_Invaders.ch8";

    ini_cmptes(&c8); //Inicializacion de componentes del chip-8
    leerROM(&c8, file);
    
     ventana(&graf); //Creamos la ventana donde se visualizaran los graficos.
        
    SDL_Event e;
	
    int quit = 0;

    //Gameloop

    while(!quit)
    {
        cicloFDE(&c8);
        actualizar_timers(&c8);	

        while(SDL_PollEvent(&e))
	{
            if(e.type == SDL_KEYDOWN)
	    {
	        for(int i = 0; i < 16; i++)
		{
		    if(e.key.keysym.sym == mapeo_teclas[i])
		    {
		        c8.teclas[i] = 1;
		    }
		}
	    }

	    if(e.type == SDL_KEYUP)
	    {
	        for(int i = 0; i < 16; i++)
		{
		    if(e.key.keysym.sym == mapeo_teclas[i])
		    {
		        c8.teclas[i] = 0;
		    }
		}
	    }
	  
	    if(e.type == SDL_QUIT)
	    {
	        quit = 1;
	    }
	}

        //dibujado y actualizacion de la pantalla (SDL)
        if(c8.drawFlag)
	{
	    c8.drawFlag = 0;	
	    actualizar(&c8, &graf);
	}
      
	usleep(3000);
    }

   return 0; 
}
