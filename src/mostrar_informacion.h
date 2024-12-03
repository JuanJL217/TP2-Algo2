#ifndef MOSTRAR_INFORMACION_H
#define MOSTRAR_INFORMACION_H

#include <stdlib.h>
#include <stdbool.h>
#include "tablero.h"
#include "lista.h"
#include "hash.h"
#include "pokedex.h"
#include "estructuras_jugabilidad.h"
#include "menu.h"

bool imprimir_opciones(void *_opcion, void *ctx);

void dibujar_tablero(tablero_t *tablero);

bool imprimir_pokemones(pokemon_t *pokemon, void *_colores);

void mostrar_informacion_por_pantalla(size_t *tiempo, size_t *semilla,
				      size_t tiempo_maximo, Pila *capturados,
				      usuario *usuario, tablero_t *tablero);

void mostrar_resultados(Lista *grupos_formados, size_t maximo_grupo_formado,
			size_t multiplicador_maximo, size_t puntaje_personaje);

size_t pedir_numero(const char *mensaje);

#endif // MOSTRAR_INFORMACION_H
