#include "pokedex.h"
#include <string.h>
#include <stdio.h>
#include "abb.h"
#include "lista.h"
#include "csv.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct pokemon_posicion {
	pokemon_t *pokemon;
	size_t posicion;
	size_t llegar;
} pokemon_ubicacion;

int comparar_nombres_pokemon(void* _poke1, void* _poke2)
{
	pokemon_t* pokemon1 = (pokemon_t*)_poke1;
	pokemon_t* pokemon2 = (pokemon_t*)_poke2;
	return strcmp(pokemon1->nombre, pokemon2->nombre);
}

pokedex_t *pokedex_crear()
{
	return abb_crear(comparar_nombres_pokemon);
}

bool pokedex_insertar_pokemon(pokedex_t* pokedex, pokemon_t* pokemon)
{
	return abb_insertar(pokedex, (void*)pokemon);
}

size_t pokedex_cantidad(pokedex_t *pokedex)
{
	return abb_cantidad(pokedex);
}

size_t pokedex_iterar(pokedex_t *pokedex,
		      bool (*funcion_mostrar)(pokemon_t *, void *), void *ctx)
{
	return abb_iterar_inorden(pokedex, (bool (*)(void *, void *))funcion_mostrar, ctx);
}

bool buscar_pokemon_en_orden(void *_pokemon, void *_posicion)
{
	pokemon_t *pokemon = (pokemon_t *)_pokemon;
	pokemon_ubicacion *ubicacion = (pokemon_ubicacion *)_posicion;
	if (ubicacion->posicion == ubicacion->llegar) {
		ubicacion->pokemon = pokemon;
	}
	ubicacion->posicion++;
	return true;
}

pokemon_t *pokedex_obtener_pokemon(pokedex_t *pokedex, size_t posicion)
{
	if (!pokedex)
		return NULL;
	pokemon_ubicacion ubicacion = { .pokemon = NULL,
					.posicion = 0,
					.llegar = posicion };
	abb_iterar_inorden(pokedex, buscar_pokemon_en_orden,
			   (void *)&ubicacion);
	return ubicacion.pokemon;
}

void pokedex_destruir(pokedex_t *pokedex)
{
	abb_destruir(pokedex);
}

void pokedex_destruir_todo(pokedex_t *pokedex, void (*eliminar_pokemon)(pokemon_t*))
{
	abb_destruir_todo(pokedex, (void (*)(void *))eliminar_pokemon);
}