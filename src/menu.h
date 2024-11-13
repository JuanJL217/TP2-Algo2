#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct menu menu_t;

/**
 *Se crea el menú donde almacenaremos nuestras opciones.
 *Pasaremos una función comparadora.
 */
menu_t* menu_crear(int (*comparador)(void *, void *));

/**
 *Se ingresa una opcion y una acción relacionada.
 *Si hubo error, retorna false.
 */
bool menu_ingresar_opciones(menu_t menu, char opcion, void (*accion)(void*));

/**
 *Se muestran las opciones del menu.
 */
void menu_mostrar_opciones(menu_t menu);

/**
 *Se selecciona una opción y se ejecuta una acción
 *Si se ingresa una incorrecta, retorna false.
 */
bool menu_ejecutar_opcion(menu_t menu, char opcion);
void menu_destruir();

#endif // MENU_H_
