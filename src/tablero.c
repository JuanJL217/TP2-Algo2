#include "tablero.h"
#include <stdio.h>
#include <string.h>
#include "../extra/ansi.h"

const char ESPACIO_VACIO = ' ';

typedef struct color {
	char elemento;
	char *color;
} color_t;

struct tablero {
	color_t **tabla;
	size_t filas;
	size_t columnas;
};

tablero_t *tablero_crear(size_t filas, size_t columnas)
{
	tablero_t *tablero = calloc(1, sizeof(tablero_t));
	if (!tablero)
		return NULL;

	if (filas < 10) {
		tablero->filas = 10;
	} else {
		tablero->filas = filas;
	}

	if (columnas < 10) {
		tablero->columnas = 10;
	} else {
		tablero->columnas = columnas;
	}

	tablero->tabla = calloc(tablero->filas, sizeof(color_t *));
	if (!tablero->tabla) {
		free(tablero);
		return NULL;
	}

	for (size_t i = 0; i < tablero->filas; i++) {
		tablero->tabla[i] = calloc(tablero->columnas, sizeof(color_t));
		if (!tablero->tabla[i]) {
			for (size_t j = 0; j < i; j++) {
				free(tablero->tabla[j]);
			}
			free(tablero->tabla);
			free(tablero);
			return NULL;
		}
	}

	for (size_t i = 0; i < tablero->filas; i++) {
		for (size_t j = 0; j < tablero->columnas; j++) {
			tablero->tabla[i][j].elemento = ESPACIO_VACIO;
			tablero->tabla[i][j].color = ANSI_COLOR_BLACK;
		}
	}

	return tablero;
}

bool tablero_colocar_elemento(tablero_t *tablero, size_t fila, size_t columna,
			      char elemento, char *color)
{
	if (!tablero || fila >= tablero->filas || columna >= tablero->columnas)
		return false;
	if (!color)
		color = ANSI_COLOR_WHITE;
	tablero->tabla[fila][columna].elemento = elemento;
	tablero->tabla[fila][columna].color = color;
	return true;
}

size_t tablero_cantidad_filas(tablero_t *tablero)
{
	return !tablero ? 0 : tablero->filas;
}

size_t tablero_cantidad_columnas(tablero_t *tablero)
{
	return !tablero ? 0 : tablero->columnas;
}

bool tablero_posicion_esta_vacio(tablero_t *tablero, size_t fila,
				 size_t columna)
{
	if (!tablero || fila >= tablero->filas || columna >= tablero->columnas)
		return false;
	if (tablero->tabla[fila][columna].elemento != ESPACIO_VACIO &&
	    strcmp(tablero->tabla[fila][columna].color, ANSI_COLOR_BLACK) !=
		    0) {
		return false;
	}
	return true;
}

bool tablero_mover_elemento(tablero_t *tablero, size_t f_origen,
			    size_t c_origen, size_t f_destino, size_t c_destino,
			    char elemento, char *color)
{
	if (!tablero || !tablero->tabla)
		return false;

	if (f_origen >= tablero->filas || c_origen >= tablero->columnas ||
	    f_destino >= tablero->filas || c_destino >= tablero->columnas)
		return false;

	if (!color)
		color = ANSI_COLOR_WHITE;

	tablero->tabla[f_origen][c_origen].elemento = ESPACIO_VACIO;
	tablero->tabla[f_origen][c_origen].color = ANSI_COLOR_BLACK;
	tablero->tabla[f_destino][c_destino].elemento = elemento;
	tablero->tabla[f_destino][c_destino].color = color;
	return true;
}

bool tablero_posicion_informacion(tablero_t *tablero, size_t fila,
				  size_t columna, char *caracter, char **color)
{
	if (!tablero || fila >= tablero->filas || columna >= tablero->columnas)
		return false;
	*caracter = tablero->tabla[fila][columna].elemento;
	*color = tablero->tabla[fila][columna].color;
	return true;
}

void tablero_destruir(tablero_t *tablero)
{
	for (size_t fila = 0; fila < tablero->filas; fila++) {
		free(tablero->tabla[fila]);
	}
	free(tablero->tabla);
	free(tablero);
}