#ifndef LOGICA_H
#define LOGICA_H

#include <stdlib.h>
#include "hash.h"
#include "pokedex.h"

#define MAX_CARACTERES 20

typedef struct booleanos {
	bool menu_seguir;
	bool opciones_seguir;
	pokedex_t *pokedex;
	hash_t *colores;
	size_t *semilla;
	size_t cantidad_objetivos;
	size_t cantidad_filas;
	size_t cantidad_columas;
	size_t tiempo_maximo;
} booleanos;

/**
 * 
 */
bool mostrar_pokemones(void *banderas);

/**
 * 
 */
bool jugar_partida(void *banderas);

/**
 * 
 */
bool jugar_con_semilla(void *banderas);

/**
 * 
 */
bool opciones_jugar(void* banderas);

/**
 * 
 */
bool salir_del_menu(void *banderas);

bool restablecer_valores(void *banderas);

#endif // LOGICA_H
