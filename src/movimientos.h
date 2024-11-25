#ifndef MOVIMIENTOS_H
#define MOVIMIENTOS_G

#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

typedef struct movimientos movimientos_t;

movimientos_t *crear_movimiento(size_t limite_x, size_t limite_y);

bool cargar_movimientos(movimientos_t *movimientos);

size_t movimientos_cantidad(movimientos_t *movimientos);

bool movimiento_realizar(movimientos_t *movimientos, char *movimiento_pivote,
			 char *tipo_movimiento, size_t *fila, size_t *columna);

void movimientos_destruir(movimientos_t *movimientos);

#endif // PILA_H_
