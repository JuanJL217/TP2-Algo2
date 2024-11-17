#include "../extra/engine.h"
#include "../extra/ansi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "pokedex.h"
#include "tablero.h"
#include "colores.h"
#include "logica.h"
#include "movimientos.h"

const char *POSICION = "posicion";
const char *CARACTER = "caracter";
const char *COLOR = "color";
const char MI_CARACTER = '&';
const size_t CANTIDAD_POKEMONES_EN_TABLERO = 7;

typedef struct posicion {
	size_t x;
	size_t y;
} posicion;

bool imprimir_pokemones(void *_pokemon, void *_colores)
{
	pokemon_t *pokemon = (pokemon_t *)_pokemon;
	colores_t *colores = (colores_t *)_colores;
	char *color = color_obtener(colores, pokemon->color);
	printf("  %s~%s %s%s (%s) -> %s%d puntos\n", color, ANSI_COLOR_RESET,
	       color, pokemon->nombre, pokemon->movimientos, ANSI_COLOR_RESET,
	       pokemon->puntaje);
	return true;
}

pokedex_t *crear_y_cargar_pokedex(pokedex_t *bandera_pokedex)
{
	if (bandera_pokedex)
		return bandera_pokedex;
	pokedex_t *pokedex = pokedex_crear();
	if (!pokedex) {
		printf("Error al iniciar la pokedex");
		return NULL;
	}
	if (!pokedex_cargar(pokedex, "pokedex.csv", ',')) {
		printf("Error al parsear datos");
		pokedex_destruir(pokedex);
		return NULL;
	}
	return pokedex;
}

colores_t *crear_y_cargar_colores(colores_t *bandera_color)
{
	if (bandera_color)
		return bandera_color;
	colores_t *colores = colores_crear();
	if (!colores) {
		printf("Error al iniciar los colores");
		return NULL;
	}
	if (!colores_cargar(colores)) {
		printf("Error al cargar los colores");
		colores_destruir(colores);
		return NULL;
	}
	return colores;
}

bool mostrar_pokemones(void *_banderas)
{
	booleanos *banderas = (booleanos *)_banderas;
	banderas->pokedex = crear_y_cargar_pokedex(banderas->pokedex);
	banderas->colores = crear_y_cargar_colores(banderas->colores);
	if (!banderas->pokedex || !banderas->colores) {
		return false;
	}
	borrar_pantalla();
	pokedex_iterar(banderas->pokedex, imprimir_pokemones,
		       (void *)banderas->colores);
	return true;
}

typedef struct juego {
	posicion *posicion;
	tablero_t *tablero;
	movimientos_t* movimientos;
} juego_t;

int logica_juego(int entrada, void *dato)
{
	borrar_pantalla();
	juego_t *juego = (juego_t *)dato;
	size_t antes_x = juego->posicion->x;
	size_t antes_y = juego->posicion->y;
	printf("x: %li\n", antes_x);
	printf("y: %li\n", antes_y);
	char* direccion = NULL;
	if (entrada == TECLA_DERECHA) {
		direccion = "E";
	} else if (entrada == TECLA_IZQUIERDA) {
		direccion = "O";
	} else if (entrada == TECLA_ARRIBA) {
		direccion = "N";
	} else if (entrada == TECLA_ABAJO) {
		direccion = "S";
	}
	if (direccion) {
		movimiento_realizar(juego->movimientos, direccion, &juego->posicion->y, &juego->posicion->x);
	}
	tablero_mover_elemento(juego->tablero, antes_y, antes_x,
					juego->posicion->y, juego->posicion->x,
					MI_CARACTER, ANSI_COLOR_WHITE);
	tablero_mostrar(juego->tablero);
	return entrada == 'q' || entrada == 'Q';
}

bool jugar_partida(void *_banderas)
{
	borrar_pantalla();
	booleanos *banderas = (booleanos *)_banderas;
	banderas->pokedex = crear_y_cargar_pokedex(banderas->pokedex);
	banderas->colores = crear_y_cargar_colores(banderas->colores);
	if (!banderas->pokedex || !banderas->colores) {
		return false;
	}
	tablero_t *tablero = tablero_crear(15, 32);
	if (!tablero)
		return false;
	esconder_cursor();
    if (!banderas->semilla) {
        srand((unsigned int)time(NULL));
    } else {
        srand((unsigned int)*banderas->semilla);
    }
	movimientos_t* movimientos = movimientos_crear();
	movimientos_cargar(movimientos);
	for (size_t i = 0; i < CANTIDAD_POKEMONES_EN_TABLERO; i++) {
		int fila = 1 + rand() % (15 - 1 + 1);
		int columna = 1 + rand() % (32 + 1 - 1);
		int posicion_pokemon =
			rand() % ((int)pokedex_cantidad(banderas->pokedex));
		pokemon_t *pokemon = pokedex_obtener_pokemon(
			banderas->pokedex, (size_t)posicion_pokemon);
		char *color = color_obtener(banderas->colores, pokemon->color);
		tablero_colocar_elemento(tablero, (size_t)fila, (size_t)columna,
					 pokemon->nombre[0], color);
	}
	posicion pos = { .x = 0, .y = 0 };
	juego_t juego = { .tablero = tablero, .posicion = &pos, .movimientos = movimientos};
	tablero_colocar_elemento(tablero, 0, 0, MI_CARACTER, ANSI_COLOR_WHITE);
	game_loop(logica_juego, (void *)&juego);
	mostrar_cursor();
	tablero_destruir(tablero);
	movimientos_destruir(movimientos);
	banderas->menu_seguir = false;
	return true;
}

bool jugar_con_semilla(void *_banderas)
{
	char texto[MAX_CARACTERES];
	size_t semilla;
	bool seguir = true;
	while (seguir) {
		printf("Ingrese una semilla (solo valor numerico): ");
		if (fgets(texto, sizeof(texto), stdin)) {
			texto[strcspn(texto, "\n")] = '\0';
			bool es_numero = true;
			for (size_t i = 0; texto[i] != '\0'; i++) {
				if (!isdigit((unsigned int)texto[i])) {
					es_numero = false;
					break;
				}
			}
			if (es_numero) {
				semilla = strtoull(texto, NULL, 10);
				seguir = false;
			}
		}
	}
    booleanos* banderas = (booleanos*)_banderas;
    banderas->semilla = &semilla;
    jugar_partida((void*)banderas);
    return true;
}

bool salir_del_menu(void *ctx)
{
	(*(booleanos *)ctx).menu_seguir = false;
	return true;
}