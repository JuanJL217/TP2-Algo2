#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"

/**
 * TDA Menu.
 */
typedef Lista menu_t;

/**
 * Estructura para las opciones.
 * El usuario debe usarlo para la función mostrar.
 */
typedef struct opcion_menu {
	char indice;
	char *texto;
	bool (*accion)(void *);
} opcion_menu_t;

/**
 * Se crea el menú donde almacenaremos nuestras opciones.
 */
menu_t *menu_crear();

/**
 * Se ingresa una opcion y una acción relacionada.
 * Si hubo error, retorna false.
 */
bool menu_ingresar_opcion(menu_t *menu, char indice, char *texto,
			  bool (*accion)(void *));

/**
 * Se itera las opciones en orden de insersion
 */
size_t menu_iterar_opciones(menu_t *menu, bool (*funcion)(void *, void *),
			    void *ctx);

/**
 * Retorna la cantidad de opciones en el menu.
 */
size_t menu_cantidad(menu_t *menu);

/**
 * Se busca la opcion y la ejecuta.
 * Si no se encuentra, retorna false.
 */
bool menu_ejecutar_opcion(menu_t *menu, char indice, void *ctx);

/**
 * Destruye el menu
 */
void menu_destruir(menu_t *menu);

#endif // MENU_H_
