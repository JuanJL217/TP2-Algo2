#ifndef MOVIMIENTOS_H
#define MOVIMIENTOS_H

#include "hash.h"

typedef hash_t movimientos_t;

movimientos_t* movimientos_crear();

bool movimientos_agregar(movimientos_t* movimientos, char *clave, void (*funcion)(char *, char *, size_t *, size_t *, size_t, size_t));

size_t movimientos_cantidad(movimientos_t *movimientos);

bool movimiento_realizar(movimientos_t *movimientos, char *movimiento_externo,
		char *movimiendo_realizar, size_t *fila, size_t *columna, size_t fila_limite, size_t columna_limite);

void movimientos_destruir(movimientos_t *movimientos);

#endif // MOVIMIENTOS_H
