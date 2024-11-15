#include "tablero.h"

struct tablero {
    char **tabla;
	size_t filas;
	size_t columnas;
};

tablero_t *tablero_crear(size_t filas, size_t columnas)
{
	if (filas <= 0 || columnas <= 0)
		return NULL;
	tablero_t *tablero = calloc(1, sizeof(tablero_t));
	if (!tablero)
		return NULL;
	tablero->filas = filas;
	tablero->columnas = columnas;
	tablero->tabla = calloc(filas, sizeof(char *));
	if (!tablero->tabla) {
		free(tablero);
		return NULL;
	}
	for (size_t i = 0; i < filas; i++) {
		tablero->tabla[i] = calloc(columnas, sizeof(char));
		if (!tablero->tabla[i]) {
			for (size_t j = 0; j < i; j++) {
				free(tablero->tabla[j]);
			}
		}
		free(tablero->tabla);
		free(tablero);
		return NULL;
	}
    for (size_t i = 0; i < filas; i++) {
        for (size_t j = 0; j < columnas; j++) {
            tablero->tabla[i][j] = ' ';
        }
    }
	return tablero;
}

void tablero_colocar_elemento(tablero_t* tablero, size_t fila, size_t columna, char elemento)
{
    tablero->tabla[fila][columna] = elemento;
}

