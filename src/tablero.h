#ifndef TABLERO_H
#define TABLERO_H

#include <stdlib.h>
#include <stdbool.h>

/**
 *TDA Tablero.
 */
typedef struct tablero tablero_t;


/**
 *
 */
tablero_t* tablero_crear();

/**
 *
 */
void tablero_colocar_elemento(tablero_t* tablero, size_t fila, size_t columna, char elemento);

/**
 *
 */
size_t tablero_tamanio(tablero_t* tablero, size_t* filas, size_t* columnas);

/**
 *
 */
bool tablero_mover_elemento(tablero_t* tablero, size_t f_origen, size_t c_orgine, size_t f_destino, size_t c_destino, char elemento);

/**
 *
 */
char obtener_elemento(tablero_t* tablero, size_t filas, size_t columnas)
/**
 *
 */
void tablero_mostrar(tablero_t* tablero);

/**
 *
 */
void tablero_limpiar(tablero_t* tablero);


/**
 *Libera toda la memoria.
 */
void tablero_destruir(tablero_t* tablero);

#endif // TABLERO_h
