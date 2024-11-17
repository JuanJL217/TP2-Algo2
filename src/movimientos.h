#ifndef MOVIMIENTOS_H
#define MOVIMIENTOS_G

#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

typedef hash_t movimientos_t;

movimientos_t *movimientos_crear();

size_t movimientos_cantidad(movimientos_t *movimientos);

bool movimiento_realizar(movimientos_t* movimientos, char* tipo_movimiento, size_t* fila, size_t* columna);

bool movimientos_cargar(movimientos_t *movimientos);

void movimientos_destruir(movimientos_t *movimientos);

#endif // PILA_H_
