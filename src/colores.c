#include "colores.h"
#include "../extra/ansi.h"

const size_t CANTIDAD_COLORES = 8;

colores_t* colores_crear()
{
    return hash_crear(15);
}

bool colores_cargar(colores_t* colores)
{
	hash_insertar(colores, "ROJO", ANSI_COLOR_RED, NULL);
	hash_insertar(colores, "AZUL", ANSI_COLOR_BLUE, NULL);
	hash_insertar(colores, "VERDE", ANSI_COLOR_GREEN, NULL);
	hash_insertar(colores, "MAGENTA", ANSI_COLOR_MAGENTA, NULL);
	hash_insertar(colores, "AMARILLO", ANSI_COLOR_YELLOW, NULL);
	hash_insertar(colores, "NEGRO", ANSI_COLOR_BLACK, NULL);
	hash_insertar(colores, "CIAN", ANSI_COLOR_CYAN, NULL);
	hash_insertar(colores, "BLANCO", ANSI_COLOR_WHITE, NULL);

	if (hash_cantidad(colores) != CANTIDAD_COLORES) {
		hash_destruir(colores);
		return false;
	}
    return true;
}

size_t colores_cantidad(colores_t* colores)
{
    return hash_cantidad(colores);
}

char* color_obtener(colores_t* colores, char* color)
{
    return (char*)hash_buscar(colores, color);
}

void colores_destruir(colores_t* colores)
{
    hash_destruir(colores);
}