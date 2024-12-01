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
 * Muestra a los pokemones en orden alfabetico.
 */
bool mostrar_pokemones(void *banderas);

/**
 * Crea el juego con un número random.
 */
bool jugar_con_semilla_random(void *banderas);

/**
 * Crea el juego con una semilla dada por el usuario, que
 * cada que vuelva a poner la misma semilla, debe ser
 * los imsmos pokemones en las mismas posiciones.
 */
bool jugar_con_semilla(void *banderas);

/**
 * Cierra el programa
 */
bool salir_del_menu(void *banderas);

/**
 * Reestablece cualquier modificación que se haya hecho
 * al juego.
 */
bool restablecer_valores(void *banderas);

/**
 *  Cambia la cantidad de pokemones por la que desee el usuario.
 */
bool modificar_cantidad_pokemones(void *banderas);

/**
 * Cambia el tiempo máximo en el cual se desarrolla el juego.
 */
bool modificar_tiempo(void *banderas);

/**
 * Crea el juego con un tablero de tamaño deseable por el usuario.
 */
bool modificar_dimensiones_tablero(void *banderas);

/**
 * Cierra el menú de las opciones extras y vuelve 
 * al menú principal.
 */
bool volver_al_menu(void *banderas);

#endif // LOGICA_H