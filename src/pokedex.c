#include "pokedex.h"
#include <string.h>
#include <stdio.h>
#include "abb.h"
#include "lista.h"
#include "csv.h"

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

bool agregar_cadena(const char *str, void *ctx)
{
	char *nuevo = malloc(strlen(str) + 1);
	if (!nuevo) {
		return false;
	}
	strcpy(nuevo, str);
	*(char **)ctx = nuevo;
	return true;
}

bool agregar_numero(const char *str, void *ctx)
{
	return sscanf(str, "%d", (int *)ctx) == 1;
}

pokedex_t *pokedex_crear()
{
	return abb_crear(comparar_nombres_pokemon);
}

bool pokedex_cargar(pokedex_t *pokedex, char *archivo, char separador)
{
	struct archivo_csv *archivo_pokemones =
		abrir_archivo_csv((const char*)archivo, separador);
	if (!archivo_pokemones) {
		return false;
	}

	pokemon_t pokemon;
	pokemon.nombre = NULL;
	pokemon.color = NULL;
	pokemon.movimientos = NULL;

	void* punteros[] = {&pokemon.nombre, &pokemon.puntaje, &pokemon.color, &pokemon.movimientos};

	bool (*funciones[])(const char *, void *) = { agregar_cadena, agregar_numero, agregar_cadena,
				agregar_cadena};

	size_t lineas_leidas = 0;
	while (leer_linea_csv(archivo_pokemones, 4, funciones,
			      punteros) == 4) {
		pokemon_t *nueva_ubicacion_pokemon = malloc(sizeof(pokemon_t));
		if (!nueva_ubicacion_pokemon) {
			cerrar_archivo_csv(archivo_pokemones);
			return false;
		}
		*nueva_ubicacion_pokemon = pokemon;
		if (!abb_insertar(pokedex,
				  (void *)nueva_ubicacion_pokemon)) {
			cerrar_archivo_csv(archivo_pokemones);
			return false;
		}
		lineas_leidas++;
	}

	cerrar_archivo_csv(archivo_pokemones);

	if (lineas_leidas == 0) {
		return false;
	}

	return true;
}

size_t pokedex_cantidad(pokedex_t *pokedex)
{
	return abb_cantidad(pokedex);
}

size_t pokedex_iterar(pokedex_t *pokedex,
		      bool (*funcion_mostrar)(void *, void *), void *ctx)
{
	return abb_iterar_inorden(pokedex, funcion_mostrar, ctx);
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

void destruir_pokemones(void* _pokemones)
{
	pokemon_t* pokemon = (pokemon_t*)_pokemones;
	free(pokemon->nombre);
	free(pokemon->color);
	free(pokemon->movimientos);
	free(pokemon);
}

void pokedex_destruir(pokedex_t *pokedex)
{
	abb_destruir_todo(pokedex, destruir_pokemones);
}