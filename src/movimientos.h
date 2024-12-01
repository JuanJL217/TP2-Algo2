#ifndef MOVIMIENTOS_H
#define MOVIMIENTOS_H

#include "hash.h"

/**
 *TDA Movimientos.
 */
typedef hash_t movimientos_t;

/**
 * Se crea el almacenamiento para guardar
 * movimientos de personajes
 */
movimientos_t* movimientos_crear();

/**
 * Se agrega una funcion de movimiento
 * Si el nombre del movimiento o la funcion
 * es NULL, retorna false
 */
bool movimientos_agregar(movimientos_t* movimientos, char *clave, void (*funcion)(char *, size_t *, size_t *, size_t, size_t));

/**
 * Devuelve la cantidad de movimientos agregados
 */
size_t movimientos_cantidad(movimientos_t *movimientos);

/**
 * Dado una palabra clave de movimento o si tambien 
 * su movimiento depende de otro movimiento, se busca el 
 * movimiento a realizar y se ejecuta
 * El usuario pondrá el limite hasta donde se puede 
 * realizar un movimiento.
 */
bool movimiento_realizar(movimientos_t *movimientos, char *movimiento_dependiente,
		char *movimiendo_realizar, size_t *fila, size_t *columna, size_t fila_limite, size_t columna_limite);

/**
 * Se destruye el almacenamiento de movimientos
 */
void movimientos_destruir(movimientos_t *movimientos);

#endif // MOVIMIENTOS_H