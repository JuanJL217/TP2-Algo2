#include "colores.h"
#include "../extra/ansi.h"
#include <stdlib.h>
#include <stdbool.h>

colores_t *colores_crear()
{
	return hash_crear(15);
}

size_t colores_cantidad(colores_t *colores)
{
	return hash_cantidad(colores);
}

bool colores_agregar(colores_t* colores, char* color, char* codigo)
{
	return hash_insertar(colores, color, codigo, NULL);
}

char *color_obtener(colores_t *colores, char *color)
{
	return (char *)hash_buscar(colores, color);
}

void colores_destruir(colores_t *colores)
{
	hash_destruir(colores);
}