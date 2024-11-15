#include "pokedex.h"
#include <string.h>
#include "abb.h"
#include "lista.h"
#include "csv.h"

typedef struct pokemon_posicion {
	pokemon_t *pokemon;
	size_t posicion;
	size_t llegar;
} pokemon_ubicacion;

pokedex_t *pokedex_crear(int (*comparador)(void *, void *))
{
	return (pokedex_t *)abb_crear(comparador);
}

bool pokedex_cargar(pokedex_t *pokedex, char *archivo, char separador,
		    pokemon_t pokemon, size_t columnas,
		    bool (*funciones[])(const char *, void *), void *punteros[])
{
	struct archivo_csv *archivo_pokemones =
		abrir_archivo_csv((const char)archivo, separador);
	if (!archivo_pokemones) {
		return false;
	}
	size_t lineas_leidas = 0;

	while (leer_linea_csv(archivo_pokemones, columnas, funciones,
			      punteros) == columnas) {
		pokemon_t *nueva_ubicacion_pokemon = malloc(sizeof(pokemon_t));
		if (!nueva_ubicacion_pokemon) {
			return false;
		}
		*nueva_ubicacion_pokemon = pokemon;
		if (!abb_insertar((abb_t *)pokedex,
				  (void *)nueva_ubicacion_pokemon)) {
			return false;
		}
		lineas_leidas++;
	}

	if (lineas_leidas == 0) {
		return false;
	}

	return true;
}

size_t pokedex_cantidad(pokedex_t *pokedex)
{
	return abb_cantidad((abb_t *)pokedex);
}

size_t pokedex_iterar(pokedex_t *pokedex,
		      bool (*funcion_mostrar)(void *, void *), void *ctx)
{
	return abb_iterar_inorden((abb_t *)pokedex, funcion_mostrar, ctx);
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
	abb_iterar_inorden((abb_t *)pokedex, buscar_pokemon_en_orden,
			   (void *)&ubicacion);
	return ubicacion.pokemon;
}

void pokedex_destruir(pokedex_t *pokedex, void *destructor)
{
	abb_destruir_todo((abb_t *)pokedex, destructor);
}