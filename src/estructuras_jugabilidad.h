#ifndef _ESTRUCTURAS_JUGABILIDAD_H_
#define _ESTRUCTURAS_JUGABILIDAD_H_

#include <stdbool.h>
#include <stddef.h>
#include "lista.h"
#include "pila.h"
#include "logica_del_juego.h"
#include "movimientos.h"
#include "tablero.h"

typedef struct jugador_en_juego {
	char *direccion;
	char caracter;
	char *color;
	size_t cantidad_pasos;
	size_t puntaje;
	size_t multiplicador;
	size_t multiplicador_maximo;
	size_t y;
	size_t x;
} usuario;

typedef struct pokemon_en_juego {
	char *nombre;
	char caracter;
	char *color;
	char *movimientos;
	size_t puntaje;
	size_t y;
	size_t x;
} pokemon_seleccionado;

typedef struct informacion_para_el_juego {
	usuario *personaje;
	Lista *pokemones_seleccionados;
	Lista *posiciones_pokemones_capturados;
	tablero_t *tablero;
	movimientos_t *movimientos;
	banderas_t* banderas;
	Pila *pokemones_capturados;
	Lista *grupos_formados;
	size_t maximo_grupo_formado;
	size_t contador_grupos;
	size_t tiempo;
	size_t posicion_pokemon_eliminado;
} informacion_juego;

void destruir_pokemones_seleccionados(void *_pokemon);

#endif /* _ESTRUCTURAS_JUGABILIDAD_H_ */