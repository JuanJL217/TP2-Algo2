#include "movimientos.h"
#include "../extra/ansi.h"
#include "../extra/engine.h"
#include <stdlib.h>
#include "time.h"
#include <stdbool.h>

const size_t CANTIDAD_MOVIMIENTOS = 7;

typedef struct {
	void (*funcion_movimiento)(char *, size_t *, size_t *, size_t, size_t);
} funcion_movimiento_t;

movimientos_t* movimientos_crear()
{
	return hash_crear(15);
}

bool movimientos_agregar(movimientos_t* movimientos, char *clave, void (*funcion)(char *, size_t *, size_t *, size_t, size_t)) {
	if (!movimientos || !clave || !funcion)
		return false;
    funcion_movimiento_t *movimiento = calloc(1, sizeof(funcion_movimiento_t));
    if (!movimiento)
		return false;
    movimiento->funcion_movimiento = funcion;
    if (!hash_insertar(movimientos, clave, movimiento, NULL)) {
        free(movimiento);
        return false;
    }
    return true;
}

size_t movimientos_cantidad(movimientos_t *movimientos)
{
	return hash_cantidad(movimientos);
}

bool movimiento_realizar(movimientos_t *movimientos, char *movimiento_externo,
		char *movimiendo_realizar, size_t *fila, size_t *columna, size_t fila_limite, size_t columna_limite)
{
	if (!movimientos || !fila || !columna) {
		return false;
	}
	funcion_movimiento_t *func = (funcion_movimiento_t *)hash_buscar(movimientos, movimiendo_realizar);
	if (!func)
		return false;
	func->funcion_movimiento(movimiento_externo, fila, columna, fila_limite, columna_limite);
	return true;
}

void destruir_funciones(void *_funciones)
{
	funcion_movimiento_t *funciones = (funcion_movimiento_t *)_funciones;
	free(funciones);
}

void movimientos_destruir(movimientos_t *movimientos)
{
	hash_destruir_todo(movimientos, destruir_funciones);
}