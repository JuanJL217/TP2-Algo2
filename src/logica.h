#ifndef LOGICA_H
#define LOGICA_H

#include <stdlib.h>


typedef struct booleanos {
	bool menu_seguir;
	pokedex_t *pokedex;
	colores_t *colores;
    size_t* semilla;
} booleanos;

/**
 * 
 */
bool mostrar_pokemones(void* banderas);

/**
 * 
 */
bool jugar_partida(void* banderas);

/**
 * 
 */
bool jugar_con_semilla(void* banderas);

/**
 * 
 */
bool salir_del_menu(void* banderas);

#endif // SPLIT_H_
