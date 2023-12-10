#include "../include/chip8.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define FRECUENCIA_CPU 1600
#define TIMER 9

int main()
{
    struct chip8 c8;
    struct graficos graf;
    int resp = 0;
    int ver_rom = 0;
    char juego[30];
    char *file = "roms/";
    int ciclos_CPU = 0;   
    
    //Menú sencillo.
    
    printf("   #####  #     #  ######  #####        #######   \n");
    printf("  #       #     #    #     #   #        #     #\n");
    printf("  #       #######    #     #####        #     #\n");
    printf("  #       #     #    #     #     ####   #######     \n");
    printf("  #       #     #    #     #            #     #\n");
    printf("   #####  #     # ######   #            #######   \n");
    
    while(!resp)
    {   
        seleccion_juego(juego);
        
        //Salir del programa al ingresar el digito 'n'
        if(strcmp(juego,"n") == 0)
        {
            return 0;
        }
        
        //Codigo que añade el nombre del archivo en la ruta.
         // Calcular la longitud total de la cadena resultante
        size_t longitud_total = strlen(file) + strlen(juego) + 1;  // +1 para el carácter nulo

        // Reservar memoria suficiente para la cadena concatenada
        char* ruta_completa = (char*)malloc(longitud_total);

        // Verificar si la asignación de memoria fue exitosa
        if (ruta_completa != NULL) {
            // Copiar 'file' en 'ruta_completa'
            strcpy(ruta_completa, file);

            // Concatenar 'juego' en 'ruta_completa'
            strcat(ruta_completa, juego);
        
            // Asignar la cadena resultante a 'file'
            file = (char*)malloc(strlen(ruta_completa) + 1);
            strcpy(file, ruta_completa);
        
            // Liberar la memoria
            free(ruta_completa);
    } else {
        // Manejar un error si la asignación de memoria falla
        printf("Error \n");
        
        return -1;
    }
        
        printf("%s \n", file);
        
        //Verificamos que la lectura de la rom sea correcta.
        ver_rom = leerROM(&c8, file);
         
         
        if(ver_rom < 0)
        {
            printf(" Por favor, intentelo de nuevo. \n");
            
            resp = 0;
        }
        else
        {
            resp = 1;
        }
        
    }


    //Codigo que permite al usuario ingresar el nombre del juego que quiere ejecutar

    
   
    
    ini_cmptes(&c8); //Inicializacion de componentes del chip-8
    leerROM(&c8, file);
    
    ventana(&graf); //Creamos la ventana donde se visualizaran los graficos.

    SDL_Event e;
	
    int quit = 0;

    //Gameloop

    while(!quit)
    {
        cicloFDE(&c8);
        ciclos_CPU++;

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

	if(ciclos_CPU == TIMER)
	{
	    actualizar_timers(&c8);
	    ciclos_CPU = 0;
	}
      
	usleep(FRECUENCIA_CPU);
    }

   return 0; 
}
