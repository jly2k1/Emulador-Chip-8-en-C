#ifndef CHIP8_T_H
#define CHIP8_T_H

#include <stdint.h>

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
    unsigned int pantalla[64 * 32];
    
    //opcodes - instrucciones
    uint16_t opcode;
};

//funciones de inicializar, reiniciar, cicloFDE, lectura de roms, timers, graficos...
void ini_cmptes(struct chip8 *c8);
void cicloFDE(struct chip8 *c8);
//funciones de reinicio, entrada del teclado, lectura de roms, timers..
int leerROM(struct chip8 *c8 ,char *romfile);
//
//
//
#endif
