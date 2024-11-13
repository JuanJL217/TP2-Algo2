#include "pila.h"
#include "lista.h"

const size_t INICIO_PILA = 0;

struct pila {
	Lista *elementos;
};

Pila *pila_crear()
{
	Pila *pila = malloc(sizeof(Pila));
	if (!pila)
		return NULL;

	Lista *lista = lista_crear();
	if (!lista) {
		free(pila);
		return NULL;
	}
	pila->elementos = lista;
	return pila;
}

void pila_destruir(Pila *pila)
{
	if (!pila)
		return;
	lista_destruir(pila->elementos);
	free(pila);
}

void pila_destruir_todo(Pila *pila, void (*f)(void *))
{
	if (!pila)
		return;
	lista_destruir_todo(pila->elementos, f);
	free(pila);
}

size_t pila_cantidad(Pila *pila)
{
	if (!pila)
		return 0;
	return lista_cantidad_elementos(pila->elementos);
}

void *pila_tope(Pila *pila)
{
	if (!pila)
		return NULL;
	void *tope;
	return lista_obtener_elemento(pila->elementos, INICIO_PILA, &tope) ?
		       tope :
		       NULL;
}

bool pila_apilar(Pila *pila, void *cosa)
{
	if (!pila)
		return false;
	return lista_agregar_elemento(pila->elementos, INICIO_PILA, cosa);
}

void *pila_desapilar(Pila *pila)
{
	if (!pila)
		return NULL;
	void *elemento_desapilado;
	return lista_quitar_elemento(pila->elementos, INICIO_PILA,
				     &elemento_desapilado) ?
		       elemento_desapilado :
		       NULL;
}

bool pila_esta_vacía(Pila *pila)
{
	if (!pila)
		return false;
	return pila_cantidad(pila) == 0;
}