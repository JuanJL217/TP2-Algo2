#ifndef COLORES_H
#define COLORES_H

#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

typedef hash_t colores_t;

/**
 *
 */
colores_t* colores_crear();

/**
 * 
 */
bool colores_cargar(colores_t* colores);

/**
 * 
 */
size_t colores_cantidad(colores_t* colores);

/**
 * 
 */
char* color_obtener(colores_t* colores, char* color);

/**
 * 
 */
void colores_destruir(colores_t*);


#endif // CSV_H
