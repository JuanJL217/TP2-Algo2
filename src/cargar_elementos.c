#include "cargar_elementos.h"
#include "../extra/ansi.h"
#include <string.h>
#include <stdio.h>
#include "csv.h"
#include "split.h"
#include "time.h"
#include <stdlib.h>
#include <stdbool.h>

const char* NOMBRE_ARCHIVO = "pokedex.csv";
const char SEPARADOR = ',';

bool agregar_cadena(const char *str, void *ctx)
{
	char *nuevo = malloc(strlen(str) + 1);
	if (!nuevo) {
		return false;
	}
	strcpy(nuevo, str);
	*(char **)ctx = nuevo;
	return true;
}

bool agregar_numero(const char *str, void *ctx)
{
	return sscanf(str, "%d", (int *)ctx) == 1;
}

pokedex_t* pokedex_cargada()
{   
    pokedex_t* pokedex = pokedex_crear();
    if (!pokedex) {
        return NULL;
    }

	struct archivo_csv *archivo_pokemones = abrir_archivo_csv(NOMBRE_ARCHIVO, SEPARADOR);
	if (!archivo_pokemones) {
        pokedex_destruir(pokedex);
		return NULL;
	}

	pokemon_t pokemon;
	pokemon.nombre = NULL;
	pokemon.color = NULL;
	pokemon.movimientos = NULL;

	void* punteros[] = {&pokemon.nombre, &pokemon.puntaje, &pokemon.color, &pokemon.movimientos};

	bool (*funciones[])(const char *, void *) = { agregar_cadena, agregar_numero, agregar_cadena,
				agregar_cadena};

	size_t lineas_leidas = 0;
	while (leer_linea_csv(archivo_pokemones, 4, funciones,
			      punteros) == 4) {
		pokemon_t *nueva_ubicacion_pokemon = calloc(1, sizeof(pokemon_t));
		if (!nueva_ubicacion_pokemon) {
			cerrar_archivo_csv(archivo_pokemones);
            pokedex_destruir(pokedex);
			return false;
		}
		*nueva_ubicacion_pokemon = pokemon;
		if (!pokedex_insertar_pokemon(pokedex, nueva_ubicacion_pokemon)) {
			cerrar_archivo_csv(archivo_pokemones);
            pokedex_destruir(pokedex);
			return false;
		}
		lineas_leidas++;
	}

	cerrar_archivo_csv(archivo_pokemones);

	if (lineas_leidas == 0) {
        pokedex_destruir(pokedex);
		return NULL;
	}

	return pokedex;
}

colores_t* colores_cargados()
{   
    colores_t* colores = colores_crear();
    if (!colores) {
        return NULL;
    }

	colores_agregar(colores, "ROJO", ANSI_COLOR_RED);
	colores_agregar(colores, "AZUL", ANSI_COLOR_BLUE);
	colores_agregar(colores, "VERDE", ANSI_COLOR_GREEN);
	colores_agregar(colores, "MAGENTA", ANSI_COLOR_MAGENTA);
	colores_agregar(colores, "AMARILLO", ANSI_COLOR_YELLOW);
	colores_agregar(colores, "NEGRO", ANSI_COLOR_BLACK);
	colores_agregar(colores, "CIAN", ANSI_COLOR_CYAN);
	colores_agregar(colores, "BLANCO", ANSI_COLOR_WHITE);

	if (colores_cantidad(colores) != 8) {
		colores_destruir(colores);
		return NULL;
	}
	return colores;
}

void movimiento_norte(char *nada1, char *nada2, size_t *fila, size_t *columna,
		      size_t limite_y, size_t limite_x)
{
	if (!fila || !columna)
		return;
	if (*fila > 0) {
		(*fila)--;
	}
}

void movimiento_sur(char *nada1, char *nada2, size_t *fila, size_t *columna,
		    size_t limite_y, size_t limite_x)
{
	if (!fila || !columna)
		return;
	if (*fila < limite_y - 1) {
		(*fila)++;
	}
}

void movimiento_este(char *nada1, char *nada2, size_t *fila, size_t *columna,
		     size_t limite_y, size_t limite_x)
{
	if (!fila || !columna)
		return;
	if (*columna < limite_x - 1) {
		(*columna)++;
	}
}

void movimiento_oeste(char *nada1, char *nada2, size_t *fila, size_t *columna,
		      size_t limite_y, size_t limite_x)
{
	if (!fila || !columna)
		return;
	if (*columna > 0) {
		(*columna)--;
	}
}

void movimiento_igual(char *movimiento_anterior, char *movimiento_hacer,
		      size_t *fila, size_t *columna, size_t limite_y,
		      size_t limite_x)
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

void movimiento_invertido(char *movimiento_anterior, char *movimiento_hacer,
			  size_t *fila, size_t *columna, size_t limite_y,
			  size_t limite_x)
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

void movimiento_al_azar(char *movimiento_anterior, char *movimiento_hacer,
			size_t *fila, size_t *columna, size_t limite_y,
			size_t limite_x)
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

movimientos_t* movimientos_cargados() {
    movimientos_t* movimientos = movimientos_crear();
    if (!movimientos)
        return NULL;

    movimientos_agregar(movimientos, "N", movimiento_norte);
	movimientos_agregar(movimientos, "S", movimiento_sur);
    movimientos_agregar(movimientos, "E", movimiento_este);
    movimientos_agregar(movimientos, "O", movimiento_oeste);
    movimientos_agregar(movimientos, "J", movimiento_igual);
    movimientos_agregar(movimientos, "I", movimiento_invertido);
    movimientos_agregar(movimientos, "R", movimiento_al_azar);

	if (movimientos_cantidad(movimientos) != 7) {
	    movimientos_destruir(movimientos);
        return NULL;
	}
    return movimientos;
}