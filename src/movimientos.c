#include "movimientos.h"
#include "../extra/ansi.h"
#include "../extra/engine.h"
#include <stdlib.h>
#include "time.h"
#include <stdbool.h>

const size_t CANTIDAD_MOVIMIENTOS = 7;

typedef struct {
	void (*funcion_movimieno)(char *, char *, size_t *, size_t *, size_t, size_t);
} funcion_movimiento_t;

movimientos_t* movimientos_crear()
{
	return hash_crear(15);
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
	func->funcion_movimieno(movimiento_externo, movimiendo_realizar, fila, columna, fila_limite, columna_limite);
	return true;
}

void destruir_funciones(void *_funciones)
{
	funcion_movimiento_t *funciones = (funcion_movimiento_t *)_funciones;
	free(funciones);
}


bool movimientos_agregar(movimientos_t* movimientos, char *clave, void (*funcion)(char *, char *, size_t *, size_t *, size_t, size_t)) {
    funcion_movimiento_t *movimiento = calloc(1, sizeof(funcion_movimiento_t));
    if (!movimiento)
		return false;
    movimiento->funcion_movimieno = funcion;
    if (!hash_insertar(movimientos, clave, movimiento, NULL)) {
        free(movimiento);
        return false;
    }
    return true;
}


void movimientos_destruir(movimientos_t *movimientos)
{
	hash_destruir_todo(movimientos, destruir_funciones);
}
