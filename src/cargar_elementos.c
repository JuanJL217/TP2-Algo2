#include "cargar_elementos.h"
#include "../extra/ansi.h"
#include <string.h>
#include <stdio.h>
#include "csv.h"
#include "split.h"
#include "time.h"
#include <stdlib.h>
#include <stdbool.h>

const char *NOMBRE_ARCHIVO = "pokedex.csv";
const char SEPARADOR = ',';

bool agregar_cadena(const char *str, void *ctx)
{
	char *nuevo = malloc(strlen(str) + 1);
	if (!nuevo) {
		printf("No se pudo dar memoria para el string, al pokemon leido");
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

pokedex_t *pokedex_cargada()
{
	pokedex_t *pokedex = pokedex_crear();
	if (!pokedex) {
		printf("No se pudo crear la Pokedex\n");
		return NULL;
	}

	struct archivo_csv *archivo_pokemones =
		abrir_archivo_csv(NOMBRE_ARCHIVO, SEPARADOR);
	if (!archivo_pokemones) {
		printf("No se pudo crear el archivo de lecura\n");
		pokedex_destruir(pokedex);
		return NULL;
	}

	pokemon_t pokemon_leido;
	pokemon_leido.nombre = NULL;
	pokemon_leido.color = NULL;
	pokemon_leido.movimientos = NULL;

	void *punteros[] = { &pokemon_leido.nombre, &pokemon_leido.puntaje,
			     &pokemon_leido.color, &pokemon_leido.movimientos };

	bool (*funciones[])(const char *,
			    void *) = { agregar_cadena, agregar_numero,
					agregar_cadena, agregar_cadena };

	size_t lineas_leidas = 0;
	while (leer_linea_csv(archivo_pokemones, 4, funciones, punteros) == 4) {
		pokemon_t *pokemon = calloc(1, sizeof(pokemon_t));
		if (!pokemon) {
			printf("No se pudo obtener memoria para almacenar al pokemon leído\n");
			cerrar_archivo_csv(archivo_pokemones);
			pokedex_destruir(pokedex);
			return false;
		}
		*pokemon = pokemon_leido;
		if (!pokedex_insertar_pokemon(pokedex, pokemon)) {
			printf("No se pudo almacenar el pokemon en la Pokedex");
			cerrar_archivo_csv(archivo_pokemones);
			pokedex_destruir(pokedex);
			return false;
		}
		lineas_leidas++;
	}

	cerrar_archivo_csv(archivo_pokemones);

	if (lineas_leidas == 0) {
		printf("No se ha leído nada. Archivo vacío\n");
		pokedex_destruir(pokedex);
		return NULL;
	}

	return pokedex;
}

hash_t *colores_cargados()
{
	hash_t *colores = hash_crear(15);
	if (!colores) {
		printf("No se pudo crear el Hash");
		return NULL;
	}
	hash_insertar(colores, "ROJO", ANSI_COLOR_RED, NULL);
	hash_insertar(colores, "AZUL", ANSI_COLOR_BLUE, NULL);
	hash_insertar(colores, "VERDE", ANSI_COLOR_GREEN, NULL);
	hash_insertar(colores, "MAGENTA", ANSI_COLOR_MAGENTA, NULL);
	hash_insertar(colores, "AMARILLO", ANSI_COLOR_YELLOW, NULL);
	hash_insertar(colores, "NEGRO", ANSI_COLOR_BLACK, NULL);
	hash_insertar(colores, "CIAN", ANSI_COLOR_CYAN, NULL);
	hash_insertar(colores, "BLANCO", ANSI_COLOR_WHITE, NULL);

	if (hash_cantidad(colores) != 8) {
		printf("Falló agregar colores al Hash\n");
		hash_destruir(colores);
		return NULL;
	}
	return colores;
}

void movimiento_norte(char *nada1, size_t *fila, size_t *columna,
		      size_t limite_y, size_t limite_x)
{
	if (!fila)
		return;
	if (*fila > 0) {
		(*fila)--;
	}
}

void movimiento_sur(char *nada1, size_t *fila, size_t *columna, size_t limite_y,
		    size_t limite_x)
{
	if (!fila)
		return;
	if (*fila < limite_y - 1) {
		(*fila)++;
	}
}

void movimiento_este(char *nada1, size_t *fila, size_t *columna,
		     size_t limite_y, size_t limite_x)
{
	if (!columna)
		return;
	if (*columna < limite_x - 1) {
		(*columna)++;
	}
}

void movimiento_oeste(char *nada1, size_t *fila, size_t *columna,
		      size_t limite_y, size_t limite_x)
{
	if (!columna)
		return;
	if (*columna > 0) {
		(*columna)--;
	}
}

void movimiento_igual(char *movimiento_anterior, size_t *fila, size_t *columna,
		      size_t limite_y, size_t limite_x)
{
	if (*movimiento_anterior == 'N') {
		movimiento_norte(NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'S') {
		movimiento_sur(NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'E') {
		movimiento_este(NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'O') {
		movimiento_oeste(NULL, fila, columna, limite_y, limite_x);
	}
}

void movimiento_invertido(char *movimiento_anterior, size_t *fila,
			  size_t *columna, size_t limite_y, size_t limite_x)
{
	if (*movimiento_anterior == 'N') {
		movimiento_sur(NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'S') {
		movimiento_norte(NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'E') {
		movimiento_oeste(NULL, fila, columna, limite_y, limite_x);
	} else if (*movimiento_anterior == 'O') {
		movimiento_este(NULL, fila, columna, limite_y, limite_x);
	}
}

void movimiento_al_azar(char *movimiento_anterior, size_t *fila,
			size_t *columna, size_t limite_y, size_t limite_x)
{
	int movimiento_random = rand() % 4;
	if (movimiento_random == 0) {
		movimiento_norte(NULL, fila, columna, limite_y, limite_x);
	} else if (movimiento_random == 1) {
		movimiento_sur(NULL, fila, columna, limite_y, limite_x);
	} else if (movimiento_random == 2) {
		movimiento_este(NULL, fila, columna, limite_y, limite_x);
	} else if (movimiento_random == 3) {
		movimiento_oeste(NULL, fila, columna, limite_y, limite_x);
	}
}

movimientos_t *movimientos_cargados()
{
	movimientos_t *movimientos = movimientos_crear();
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
		printf("Falló agregar mos movimientos\n");
		return NULL;
	}
	return movimientos;
}