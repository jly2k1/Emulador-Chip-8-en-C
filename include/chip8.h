#ifndef CHIP8_T_H
#define CHIP8_T_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <math.h>

struct chip8
{
    //Memoria ram	
    uint8_t RAM[4096];
    
    //Registros
    uint8_t V[16];
    uint16_t PC;
    uint8_t SP;
    uint16_t I;
    uint16_t Pila[16];
    
    //temporizadores
    uint8_t temp_delay;
    uint8_t temp_sonido;
    
    //teclas - controles
    uint8_t teclas[16];

    //pantalla
    uint32_t pantalla[64 * 32];
    
    //bandera de dibujado.
    uint8_t drawFlag;

    //opcodes - instrucciones
    uint16_t opcode;
};

//Arreglo que mapea las teclas.
static uint8_t mapeo_teclas[16] = 
{
    SDLK_0, 
    SDLK_1, 
    SDLK_2, 
    SDLK_3, 
    SDLK_4, 
    SDLK_5, 
    SDLK_6, 
    SDLK_7, 
    SDLK_8, 
    SDLK_9, 
    SDLK_a, 
    SDLK_s, 
    SDLK_d,
    SDLK_z, 
    SDLK_x, 
    SDLK_c 
}; 

//Struct relacionado con la parte grafica (SDL 2).
struct graficos
{
    SDL_Window *ventana;
    SDL_Renderer *render;
    SDL_Texture *textura;
};

//Conjunto de fuentes del chip-8.
static uint8_t fontset[80] = 
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

//funciones de inicializar, reiniciar, cicloFDE, lectura de roms, timers, graficos...
void ini_cmptes(struct chip8 *c8);
void cicloFDE(struct chip8 *c8);
int leerROM(struct chip8 *c8 ,char *romfile);
void actualizar_timers(struct chip8 *c8);


//Parte grafica.
int ventana(struct graficos *graf);
void actualizar(struct chip8 *c8, struct graficos *graf);
void cerrar(struct graficos *graf);

//Menu simple.
void seleccion_juego(char *juego);

#endif
