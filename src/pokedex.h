#ifndef POKEDEX_H
#define POKEDEX_H

#include <stdlib.h>
#include <stdbool.h>

/**
 *TDA Pokedex.
 */
typedef struct pokedex pokedex_t;

/**
 *Estructura para las opciones.
 *El usuario debe usarlo para la función mostrar.
 */
typedef struct pokemon pokemon_t;

/**
 *Se crea el menú donde almacenaremos nuestras opciones.
 */
pokedex_t *pokedex_crear();

/**
 *Se ingresa una opcion y una acción relacionada.
 *Si hubo error, retorna false.
 */
bool pokedex_cargar(pokedex_t *pokedex, char *archivo, char separador,
		    pokemon_t pokemon, size_t columnas,
		    bool (*funciones[])(const char *, void *),
		    void *punteros[]);

/**
 *Se itera las opciones en orden de insersion
 *Se puede editar cómo mostrar cada línea.
 */
size_t pokedex_iterar(pokedex_t *pokedex,
		      bool (*funcion_mostrar)(void *, void *), void *ctx);

/**
 *Retorna la cantidad de opciones en el menu.
 */
size_t pokedex_cantidad(pokedex_t *pokedex);

/**
 *Se busca la opcion y la ejecuta.
 *Si no se encuentra, retorna false.
 */
pokemon_t *pokedex_obtener_pokemon(pokedex_t *pokedex, size_t posicion);

/**
 *Libera toda la memoria.
 */
void pokedex_destruir(pokedex_t *pokedex, void *destructor)

#endif // MENU_H_
