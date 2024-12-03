#ifndef TABLERO_H
#define TABLERO_H

#include <stdlib.h>
#include <stdbool.h>

/**
 *TDA Tablero.
 */
typedef struct tablero tablero_t;

/**
 * Se crea un tablero con las dimensiones que se desee
 * Cada bloque de la matriz tendrá como resultado inicial 
 * un espacio vacio ' ' y el color negro
 */
tablero_t *tablero_crear(size_t filas, size_t columnas);

/**
 * Se colo un caracter y un color en una posicion de la
 * matriz.
 * Si la posicion está por fuera del rango, retorna false
 * Si el color es NULL, se pondrá color blanco por defecto
 * para dicho caracter.
 */
bool tablero_colocar_elemento(tablero_t *tablero, size_t fila, size_t columna,
			      char elemento, char *color);

/**
 * Verifica si en dicha posicion esta vacio (' ' y negro).
 * Si se pasa una posicion por fuera del rango, retorna false
 */
bool tablero_posicion_esta_vacio(tablero_t *tablero, size_t fila,
				 size_t columna);

/**
 * Devuelve la cantidad de filas
 */
size_t tablero_cantidad_filas(tablero_t *tablero);

/**
 * Devuelve la cantidad de columnas
 */
size_t tablero_cantidad_columnas(tablero_t *tablero);

/**
 * Borra el caracter y color de la posicion de origen y lo
 * reemplaza por el valor predeterminado del tablero, y 
 * en la posicion de destino, agrega el caracter y color
 * pasado por parametro.
 * Si se pasa del rango, retorna false
 */
bool tablero_mover_elemento(tablero_t *tablero, size_t f_origen,
			    size_t c_origen, size_t f_destino, size_t c_destino,
			    char elemento, char *color);

/**
 * Se le dará la informacion del caracter y del color de la posicion
 * que se busqueda en el tablero.
 * Pasar una posicion por fuera del rango, un caracter NULL y
 * un color NULL, retorna false
 */
bool tablero_posicion_informacion(tablero_t *tablero, size_t fila,
				  size_t columna, char *caracter, char **color);

/**
 *Libera toda la memoria del tablero
 */
void tablero_destruir(tablero_t *tablero);

#endif // TABLERO_h
