#include "tablero.h"
#include <stdio.h>

#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RESET "\x1b[0m"

const char ESPACIO_VACIO = ' ';

typedef struct color {
	char elemento;
	char* color;
} color_t;

struct tablero {
    color_t **tabla;
	size_t filas;
	size_t columnas;
};

tablero_t *tablero_crear(size_t filas, size_t columnas)
{
    if (filas == 0 || columnas == 0)
        return NULL;
    
    tablero_t *tablero = calloc(1, sizeof(tablero_t));
    if (!tablero)
        return NULL;
    
    tablero->filas = filas;
    tablero->columnas = columnas;
    
    tablero->tabla = calloc(filas, sizeof(color_t*));
    if (!tablero->tabla) {
        free(tablero);
        return NULL;
    }
    
    for (size_t i = 0; i < filas; i++) {
        tablero->tabla[i] = calloc(columnas, sizeof(color_t));
        if (!tablero->tabla[i]) {
            for (size_t j = 0; j < i; j++) {
                free(tablero->tabla[j]);
            }
            free(tablero->tabla);
            free(tablero);
            return NULL;
        }
    }
    
    for (size_t i = 0; i < filas; i++) {
        for (size_t j = 0; j < columnas; j++) {
            tablero->tabla[i][j].elemento = ESPACIO_VACIO;
            tablero->tabla[i][j].color = ANSI_COLOR_BLACK;
        }
    }
    
    return tablero;
}

bool tablero_colocar_elemento(tablero_t* tablero, size_t fila, size_t columna, char elemento, char* color)
{   
    if (!tablero || !tablero->tabla) {
        return false;
    }
    if (!tablero || !color || fila >= tablero->filas || columna >= tablero->columnas)
        return false;
    tablero->tabla[fila][columna].elemento = elemento;
    tablero->tabla[fila][columna].color = color;
    return true;
}

bool tablero_tamanio(tablero_t* tablero, size_t* filas, size_t* columnas)
{
	if (!tablero || !filas || !columnas)
		return false;
	*filas = tablero->filas;
	*columnas = tablero->columnas;
	return true;
}

bool tablero_mover_elemento(tablero_t* tablero, size_t f_origen, size_t c_origen, size_t f_destino, size_t c_destino, char elemento, char* color)
{   
    if (!tablero || !tablero->tabla)
        return false;

    if (f_origen >= tablero->filas || c_origen >= tablero->columnas ||
        f_destino >= tablero->filas || c_destino >= tablero->columnas)
        return false;

    tablero->tabla[f_origen][c_origen].elemento = ESPACIO_VACIO;
    tablero->tabla[f_origen][c_origen].color = ANSI_COLOR_BLACK;
    tablero->tabla[f_destino][c_destino].elemento = elemento;
    tablero->tabla[f_destino][c_destino].color = color;
    return true;
}


// bool obtener_posicion(tablero_t* tablero, size_t filas, size_t columnas)
// {
//     if (!tablero)
//         return false;
    
// }
// 000
// cebolla
// tomate
// zanahoria
// huevos
// atun
// sal


void tablero_mostrar(tablero_t* tablero)
{	
    if (!tablero)
        return;
    for (size_t fila = 0; fila < tablero->columnas; fila++) {
        printf("-");
    }
    printf("\n");
    for (size_t fila = 0; fila < tablero->filas; fila++) {
        printf("|");
        for (size_t columna = 0; columna < tablero->columnas; columna++) {
            printf(" %s%c%s ", tablero->tabla[fila][columna].color, tablero->tabla[fila][columna].elemento, ANSI_COLOR_RESET);
        }
        printf("|");
        printf("\n");
    }
    for (size_t fila = 0; fila < tablero->columnas; fila++) {
        printf("-");
    }
    printf("\n"); // Aquí se agrega el salto de línea final
}


void tablero_destruir(tablero_t* tablero)
{
	for (size_t fila = 0; fila < tablero->filas; fila++) {
		free(tablero->tabla[fila]);
	}
	free(tablero->tabla);
	free(tablero);
}