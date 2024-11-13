#include "cola.h"
#include "lista.h"

const size_t INICIO_COLA = 0;

struct cola {
	Lista *elementos;
};

Cola *cola_crear()
{
	Cola *cola = malloc(sizeof(Cola));
	if (!cola) {
		return NULL;
	}
	Lista *lista = lista_crear();
	if (!lista) {
		free(cola);
		return NULL;
	}
	cola->elementos = lista;
	return cola;
}

void cola_destruir(Cola *cola)
{
	if (!cola)
		return;
	lista_destruir(cola->elementos);
	free(cola);
}

void cola_destruir_todo(Cola *cola, void (*f)(void *))
{
	if (!cola)
		return;
	lista_destruir_todo(cola->elementos, f);
	free(cola);
}

size_t cola_cantidad(Cola *cola)
{
	if (!cola)
		return 0;
	return lista_cantidad_elementos(cola->elementos);
}

void *cola_frente(Cola *cola)
{
	if (!cola)
		return NULL;
	void *frente;
	return lista_obtener_elemento(cola->elementos, INICIO_COLA, &frente) ?
		       frente :
		       NULL;
}

bool cola_encolar(Cola *cola, void *cosa)
{
	if (!cola)
		return false;
	return lista_agregar_al_final(cola->elementos, cosa);
}

void *cola_desencolar(Cola *cola)
{
	if (!cola)
		return NULL;
	void *elemento_desencolado;
	return lista_quitar_elemento(cola->elementos, INICIO_COLA,
				     &elemento_desencolado) ?
		       elemento_desencolado :
		       NULL;
}

bool cola_esta_vacía(Cola *cola)
{
	if (!cola)
		return false;
	return cola_cantidad(cola) == 0;
}