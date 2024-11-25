#ifndef COLORES_H
#define COLORES_H

#include "hash.h"

typedef hash_t colores_t;

/**
 *
 */
colores_t *colores_crear();

/**
 * 
 */
bool colores_agregar(colores_t* colores, char* color, char* codigo);


/**
 * 
 */
size_t colores_cantidad(colores_t *colores);

/**
 * 
 */
char *color_obtener(colores_t *colores, char *color);

/**
 * 
 */
void colores_destruir(colores_t *);

#endif // CSV_H
