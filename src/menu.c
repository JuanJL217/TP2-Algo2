#include "menu.h"
#include "lista.h"
#include "csv.h"
#include <string.h>

opcion_menu_t *crear_opcion(const char indice, const char *texto,
			    bool (*accion)(void *))
{
	opcion_menu_t *opcion = malloc(sizeof(opcion_menu_t));
	if (!opcion)
		return NULL;
	char *string = malloc(strlen(texto) + 1);
	if (!string) {
		free(opcion);
		return NULL;
	}
	strcpy(string, texto);
	opcion->indice = indice;
	opcion->texto = string;
	opcion->accion = accion;
	return opcion;
}

menu_t *menu_crear()
{
	return (menu_t *)lista_crear();
}

bool menu_ingresar_opcion(menu_t *menu, char indice, char *texto,
			  bool (*accion)(void *))
{
	if (!menu || !texto || !accion)
		return false;
	opcion_menu_t *opcion = crear_opcion(indice, texto, accion);
	if (!opcion)
		return false;
	return lista_agregar_al_final((Lista *)menu, (void *)opcion);
}

size_t menu_cantidad(menu_t *menu)
{
	return lista_cantidad_elementos((Lista *)menu);
}

size_t menu_iterar_opciones(menu_t *menu,
			    bool (*funcion_mostrar)(void *, void *), void *ctx)
{
	return lista_iterar_elementos((Lista *)menu, funcion_mostrar, ctx);
}

int buscar_opcion(void *a, void *b)
{
	return (*(opcion_menu_t *)a).indice - (*(char *)b);
}

bool menu_ejecutar_opcion(menu_t *menu, char indice, void *ctx)
{
	opcion_menu_t *opcion = lista_buscar_elemento(
		(Lista *)menu, (void *)&indice, buscar_opcion);
	if (!opcion || !opcion->accion)
		return false;
	return opcion->accion(ctx);
}

void destruir_opciones(void *_opcion_actual)
{
	opcion_menu_t *opcion_actual = (opcion_menu_t *)_opcion_actual;
	free(opcion_actual->texto);
	free(opcion_actual);
}

void menu_destruir(menu_t *menu)
{
	lista_destruir_todo((Lista *)menu, destruir_opciones);
}