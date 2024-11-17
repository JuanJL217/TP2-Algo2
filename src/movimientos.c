#include "movimientos.h"
#include "../extra/ansi.h"
#include "../extra/engine.h"
#include <stdlib.h>
#include "time.h"
#include <stdio.h>

typedef struct {
    void (*funcion)(char*, size_t*, size_t*);
} funcion_movimiento_t;

movimientos_t *movimientos_crear()
{
    return hash_crear(15);
}

size_t movimientos_cantidad(movimientos_t *movimientos)
{
    return hash_cantidad(movimientos);
}

void movimiento_norte(char* movimiento_anterior, size_t* fila, size_t* columna)
{   
    if (!fila || !columna)
        return;
    (*fila)--;
}

void movimiento_sur(char* movimiento_anterior, size_t* fila, size_t* columna)
{  
    if (!fila || !columna)
        return;
    (*fila)++;
}

void movimiento_este(char* movimiento_anterior, size_t* fila, size_t* columna)
{
    if (!fila || !columna)
        return;
    (*columna)++;
}

void movimiento_oeste(char* movimiento_anterior, size_t* fila, size_t* columna)
{
    if (!fila || !columna)
        return;
    (*columna)--;
}

void movimiento_igual(char* movimiento_anterior, size_t* fila, size_t* columa)
{
    if (*movimiento_anterior == 'N') {
        movimiento_norte(movimiento_anterior, fila, columa);
    } else if (*movimiento_anterior == 'S') {
        movimiento_sur(movimiento_anterior, fila, columa);
    } else if (*movimiento_anterior == 'E') {
        movimiento_este(movimiento_anterior, fila, columa);
    } else if (*movimiento_anterior == 'O') {
        movimiento_oeste(movimiento_anterior, fila, columa);
    }
}

void movimiento_invertido(char* movimiento_anterior, size_t* fila, size_t* columa)
{
    if (*movimiento_anterior == 'N') {
        movimiento_sur(movimiento_anterior, fila, columa);
    } else if (*movimiento_anterior == 'S') {
        movimiento_norte(movimiento_anterior, fila, columa);
    } else if (*movimiento_anterior == 'E') {
        movimiento_oeste(movimiento_anterior, fila, columa);
    } else if (*movimiento_anterior == 'O') {
        movimiento_este(movimiento_anterior, fila, columa);
    }
}

void movimiento_al_azar(char* movimiento_anterior, size_t* fila, size_t* columna)
{
    srand((unsigned int)time(NULL));
    int movimiento_random = rand() % 4;
    if (movimiento_random == 0) {
        movimiento_norte(movimiento_anterior, fila, columna);
    } else if (movimiento_random == 1) {
        movimiento_sur(movimiento_anterior, fila, columna);
    } else if (movimiento_random == 2) {
        movimiento_este(movimiento_anterior, fila, columna);
    } else if (movimiento_random == 3) {
        movimiento_norte(movimiento_anterior, fila, columna);
    }
}

bool movimiento_realizar(movimientos_t* movimientos, char* tipo_movimiento, size_t* fila, size_t* columna)
{
    if (!movimientos || !fila | !columna) {
        return false;
    }
    funcion_movimiento_t* func = (funcion_movimiento_t*)hash_buscar(movimientos, tipo_movimiento);
    if (!func)
        return false;
    func->funcion(tipo_movimiento, fila, columna);
    return true;
}

void destruir_funciones(void* _funciones) 
{
    funcion_movimiento_t* funciones = (funcion_movimiento_t*)_funciones;
    free(funciones);
}

bool movimientos_cargar(movimientos_t *movimientos)
{   
    funcion_movimiento_t *norte = malloc(sizeof(funcion_movimiento_t));
    norte->funcion = movimiento_norte;
    funcion_movimiento_t *sur = malloc(sizeof(funcion_movimiento_t));
    sur->funcion = movimiento_sur;
    funcion_movimiento_t *este = malloc(sizeof(funcion_movimiento_t));
    este->funcion = movimiento_este;
    funcion_movimiento_t *oeste = malloc(sizeof(funcion_movimiento_t));
    oeste->funcion = movimiento_oeste;
    funcion_movimiento_t *igual = malloc(sizeof(funcion_movimiento_t));
    igual->funcion = movimiento_igual;
    funcion_movimiento_t *invertido = malloc(sizeof(funcion_movimiento_t));
    invertido->funcion = movimiento_invertido;
    funcion_movimiento_t *azar = malloc(sizeof(funcion_movimiento_t));
    azar->funcion = movimiento_al_azar;
    if ( !norte || !sur || !este || !oeste || !igual || !invertido || !azar) {
        free(norte);
        free(sur);
        free(este);
        free(oeste);
        free(igual);
        free(invertido);
        free(azar);
        return false;
    }
    hash_insertar(movimientos, "N", norte, NULL);
    hash_insertar(movimientos, "S", sur, NULL);
    hash_insertar(movimientos, "E", este, NULL);
    hash_insertar(movimientos, "O", oeste, NULL);
    hash_insertar(movimientos, "J", igual, NULL);
    hash_insertar(movimientos, "I", invertido, NULL);
    hash_insertar(movimientos, "R", azar, NULL);
    if (hash_cantidad(movimientos) != 7) {
        return false;
    }
    return true;
}



void movimientos_destruir(movimientos_t *movimientos)
{
    hash_destruir_todo(movimientos, destruir_funciones);
}
