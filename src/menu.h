#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <stdbool.h>

/**
 *TDA Menu.
 */
typedef struct menu menu_t;

/**
 *Estructura para las opciones.
 *El usuario debe usarlo para la función mostrar.
 */
typedef struct opcion_menu {
	char indice;
	char *texto;
	bool (*accion)(void *);
} opcion_menu_t;

/**
 *Se crea el menú donde almacenaremos nuestras opciones.
 */
menu_t *menu_crear();

/**
 *Se ingresa una opcion y una acción relacionada.
 *Si hubo error, retorna false.
 */
bool menu_ingresar_opcion(menu_t *menu, char indice, char *texto,
			  bool (*accion)(void *));

/**
 *Se itera las opciones en orden de insersion
 *Se puede editar cómo mostrar cada línea.
 */
size_t menu_iterar_opciones(menu_t *menu,
			    bool (*funcion_mostrar)(void *, void *), void *ctx);

/**
 *Retorna la cantidad de opciones en el menu.
 */
size_t menu_cantidad(menu_t *menu);

/**
 *Se busca la opcion y la ejecuta.
 *Si no se encuentra, retorna false.
 */
bool menu_ejecutar_opcion(menu_t *menu, char indice, void *ctx);

/**
 *Libera toda la memoria.
 */
void menu_destruir(menu_t *menu);

#endif // MENU_H_
