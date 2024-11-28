#ifndef POKEDEX_H
#define POKEDEX_H

#include "abb.h"

/**
 *TDA Pokedex.
 */
typedef abb_t pokedex_t;

/**
 * Estructura para el tipo de dato pokemon_t 
 * que se guardará en la pokedex
 */
typedef struct pokemon {
	char* nombre;
	char* color;
	char* movimientos;
	int puntaje;
} pokemon_t;

/**
 * Se crea una Pokedex para almacenar pokemones
 */
pokedex_t *pokedex_crear();

/**
 * Se ingresa un pokemon a la Pokedex.
 * Si el pokemon es NULL retorna fasle
 */
bool pokedex_insertar_pokemon(pokedex_t* pokedex, pokemon_t* pokemon);

/**
 * Se itera la Pokedex por orden alfabetico
 */
size_t pokedex_iterar(pokedex_t *pokedex, bool (*funcion)(pokemon_t *, void *), void *ctx);

/**
 * Retorna la cantidad de pokemones en la Pokedex
 */
size_t pokedex_cantidad(pokedex_t *pokedex);

/**
 * Se busca un pokemon en alguna posicion de la pokedex
 * Si se pasa un numero igual o mayor a la cantidad de 
 * pokemones, retorna NULL
 */
pokemon_t *pokedex_obtener_pokemon(pokedex_t *pokedex, size_t posicion);

/**
 * Destruye la pokedex
 */
void pokedex_destruir(pokedex_t *pokedex);

/**
 * Destruye la pokedex y se le pasa una funcion
 * que libera cada pokemon en Pokedex
 */
void pokedex_destruir_todo(pokedex_t *pokedex, void (*eliminar_pokemon)(pokemon_t*));

#endif // MENU_H_
