#include "movimientos.h"
#include "../extra/ansi.h"
#include "../extra/engine.h"
#include <stdlib.h>
#include "time.h"

const size_t CANTIDAD_MOVIMIENTOS = 7;

typedef struct {
	void (*funcion)(char *, char *, size_t *, size_t *, size_t, size_t);
} funcion_movimiento_t;

struct movimientos {
	size_t limite_columna;
	size_t limite_fila;
	hash_t* diccionario_movimientos;
};

movimientos_t* crear_movimiento(size_t limite_y, size_t limite_x)
{
	movimientos_t* movimientos = calloc(1, sizeof(movimientos_t));
	if (!movimientos)
		return NULL;
	movimientos->limite_columna = limite_x;
	movimientos->limite_fila = limite_y;
	movimientos->diccionario_movimientos = hash_crear(15);
	if (!movimientos->diccionario_movimientos) {
		free(movimientos);
		return NULL;
	}
	return movimientos;
}

size_t movimientos_cantidad(movimientos_t *movimientos)
{
	return hash_cantidad(movimientos->diccionario_movimientos);
}

void movimiento_norte(char* nada1, char* nada2, size_t *fila, size_t *columna, size_t limite_y, size_t limite_x)
{
	if (!fila || !columna)
		return;
	if (*fila > 0) {
		(*fila)--;
	}
}

void movimiento_sur(char* nada1, char* nada2, size_t *fila, size_t *columna, size_t limite_y, size_t limite_x)
{
	if (!fila || !columna)
		return;
	if (*fila < limite_y - 1) {
		(*fila)++;
	}
}

void movimiento_este(char* nada1, char* nada2, size_t *fila, size_t *columna, size_t limite_y, size_t limite_x)
{
	if (!fila || !columna)
		return;
	if (*columna < limite_x - 1) {
		(*columna)++;
	}
}

void movimiento_oeste(char* nada1, char* nada2, size_t *fila, size_t *columna, size_t limite_y, size_t limite_x)
{
	if (!fila || !columna)
		return;
	if (*columna > 0) {
		(*columna)--;
	}
}

void movimiento_igual(char* movimiento_anterior, char* movimiento_hacer, size_t *fila, size_t *columna, size_t limite_y, size_t limite_x)
{
	if (*movimiento_anterior == 'N') {
		movimiento_norte(NULL, NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'S') {
		movimiento_sur(NULL, NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'E') {
		movimiento_este(NULL, NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'O') {
		movimiento_oeste(NULL, NULL, fila, columna, limite_y, limite_x);
	}
}

void movimiento_invertido(char* movimiento_anterior, char* movimiento_hacer, size_t *fila, size_t *columna, size_t limite_y, size_t limite_x)
{
	if (*movimiento_anterior == 'N') {
		movimiento_sur(NULL, NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'S') {
		movimiento_norte(NULL, NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'E') {
		movimiento_oeste(NULL, NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'O') {
		movimiento_este(NULL, NULL, fila, columna, limite_y, limite_x);
	}
}

void movimiento_al_azar(char* movimiento_anterior, char* movimiento_hacer, size_t *fila, size_t *columna, size_t limite_y, size_t limite_x)
{
	srand((unsigned int)time(NULL));
	int movimiento_random = rand() % 4;
	if (movimiento_random == 0) {
		movimiento_norte(NULL, NULL, fila, columna, limite_y, limite_x);
	} else if (movimiento_random == 1) {
		movimiento_sur(NULL, NULL, fila, columna, limite_y, limite_x);
	} else if (movimiento_random == 2) {
		movimiento_este(NULL, NULL, fila, columna, limite_y, limite_x);
	} else if (movimiento_random == 3) {
		movimiento_oeste(NULL, NULL, fila, columna, limite_y, limite_x);
	}
}

bool movimiento_realizar(movimientos_t* movimientos, char* movimiento_anterior, char* movimiento_hacer, size_t* fila, size_t* columna)
{
	if (!movimientos || !fila || !columna) {
		return false;
	}
	funcion_movimiento_t *func = (funcion_movimiento_t *)hash_buscar(
		movimientos->diccionario_movimientos, movimiento_hacer);
	if (!func)
		return false;
	func->funcion(movimiento_anterior, movimiento_hacer, fila, columna, movimientos->limite_fila, movimientos->limite_columna);
	return true;
}

void destruir_funciones(void *_funciones)
{
	funcion_movimiento_t *funciones = (funcion_movimiento_t *)_funciones;
	free(funciones);
}

bool cargar_movimientos(movimientos_t* movimientos)
{
	if (!movimientos) {
		return false; 
	}

	funcion_movimiento_t *norte = calloc(1, sizeof(funcion_movimiento_t));
	funcion_movimiento_t *sur = calloc(1, sizeof(funcion_movimiento_t));
	funcion_movimiento_t *este = calloc(1, sizeof(funcion_movimiento_t));
	funcion_movimiento_t *oeste = calloc(1, sizeof(funcion_movimiento_t));
	funcion_movimiento_t *igual = calloc(1, sizeof(funcion_movimiento_t));
	funcion_movimiento_t *invertido = calloc(1, sizeof(funcion_movimiento_t));
	funcion_movimiento_t *azar = calloc(1, sizeof(funcion_movimiento_t));

	if (!norte || !sur || !este || !oeste || !igual || !invertido || !azar) {
		free(norte);
		free(sur);
		free(este);
		free(oeste);
		free(igual);
		free(invertido);
		free(azar);
		return false;
	}

	norte->funcion = movimiento_norte;
	sur->funcion = movimiento_sur;
	este->funcion = movimiento_este;
	oeste->funcion = movimiento_oeste;
	igual->funcion = movimiento_igual;
	invertido->funcion = movimiento_invertido;
	azar->funcion = movimiento_al_azar;

	hash_insertar(movimientos->diccionario_movimientos, "N", norte, NULL);
	hash_insertar(movimientos->diccionario_movimientos, "S", sur, NULL);
	hash_insertar(movimientos->diccionario_movimientos, "E", este, NULL);
	hash_insertar(movimientos->diccionario_movimientos, "O", oeste, NULL);
	hash_insertar(movimientos->diccionario_movimientos, "J", igual, NULL);
	hash_insertar(movimientos->diccionario_movimientos, "I", invertido, NULL);
	hash_insertar(movimientos->diccionario_movimientos, "R", azar, NULL);

	if (hash_cantidad(movimientos->diccionario_movimientos) != CANTIDAD_MOVIMIENTOS) {
		return false;
	}

	return true; // Si todo fue correcto, retornar true
}


void movimientos_destruir(movimientos_t *movimientos)
{
	hash_destruir_todo(movimientos->diccionario_movimientos, destruir_funciones);
	free(movimientos);
}
