#include "lista.h"

const size_t UNA_POSICION_ANTES = 1;
const size_t PRIMER_NODO = 0;

struct lista {
	struct nodo *primer_nodo;
	struct nodo *ultimo_nodo;
	size_t cantidad_elementos;
};

// -------------- ESTRUCTURA Y FUNCIONES DE NODO --------------

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_lista;

nodo_lista *nodo_crear(void *cosa)
{
	nodo_lista *nodo = malloc(sizeof(nodo_lista));
	if (!nodo)
		return NULL;
	nodo->elemento = cosa;
	nodo->siguiente = NULL;
	return nodo;
}

nodo_lista *buscar_nodo(nodo_lista *nodo_actual, size_t posicion, size_t ajuste)
{
	for (size_t i = 0; i < posicion - ajuste; i++)
		nodo_actual = nodo_actual->siguiente;

	return nodo_actual;
}

// -------------- FUNCIONES AUXILIARES --------------

bool es_el_inicio(size_t posicion)
{
	return posicion == 0;
}

bool lista_esta_vacia(Lista *lista)
{
	return lista->cantidad_elementos == 0;
}

bool posicion_fuera_del_rango(size_t posicion, size_t cantidad_elementos)
{
	return posicion > cantidad_elementos;
}

bool es_el_final(size_t posicion, size_t cantidad_elemento)
{
	return posicion == cantidad_elemento;
}

size_t posicion_ultimo_nodo(Lista *lista)
{
	return lista->cantidad_elementos - 1;
}

// -------------- LISTA --------------

Lista *lista_crear()
{
	Lista *lista = malloc(sizeof(Lista));
	lista->primer_nodo = NULL;
	lista->ultimo_nodo = NULL;
	lista->cantidad_elementos = 0;
	return lista;
}

size_t lista_cantidad_elementos(Lista *lista)
{
	return (!lista) ? 0 : lista->cantidad_elementos;
}

bool lista_agregar_elemento(Lista *lista, size_t posicion, void *cosa)
{
	if (!lista ||
	    posicion_fuera_del_rango(posicion, lista->cantidad_elementos))
		return false;

	if (es_el_final(posicion, lista->cantidad_elementos))
		return lista_agregar_al_final(lista, cosa);

	nodo_lista *nuevo_nodo = nodo_crear(cosa);
	if (!nuevo_nodo)
		return false;

	if (es_el_inicio(posicion)) {
		nuevo_nodo->siguiente = lista->primer_nodo;
		lista->primer_nodo = nuevo_nodo;
	} else {
		nodo_lista *nodo_encontrado = buscar_nodo(
			lista->primer_nodo, posicion, UNA_POSICION_ANTES);
		nuevo_nodo->siguiente = nodo_encontrado->siguiente;
		nodo_encontrado->siguiente = nuevo_nodo;
	}

	lista->cantidad_elementos++;
	return true;
}

bool lista_agregar_al_final(Lista *lista, void *cosa)
{
	if (!lista)
		return false;

	nodo_lista *nuevo_nodo = nodo_crear(cosa);
	if (!nuevo_nodo)
		return false;

	if (lista_esta_vacia(lista))
		lista->primer_nodo = nuevo_nodo;
	else
		lista->ultimo_nodo->siguiente = nuevo_nodo;

	lista->ultimo_nodo = nuevo_nodo;
	lista->cantidad_elementos++;
	return true;
}

bool lista_quitar_elemento(Lista *lista, size_t posicion,
			   void **elemento_quitado)
{
	if (!lista ||
	    posicion_fuera_del_rango(posicion, lista->cantidad_elementos) ||
	    lista_esta_vacia(lista))
		return false;

	nodo_lista *nodo_encontrado;
	nodo_lista *nodo_anterior = NULL;

	if (es_el_inicio(posicion)) {
		nodo_encontrado = lista->primer_nodo;
		lista->primer_nodo = lista->primer_nodo->siguiente;
		if (lista->cantidad_elementos == 1)
			lista->ultimo_nodo = NULL;
	} else {
		nodo_anterior = buscar_nodo(lista->primer_nodo, posicion,
					    UNA_POSICION_ANTES);
		if (posicion == posicion_ultimo_nodo(lista)) {
			nodo_encontrado = lista->ultimo_nodo;
			lista->ultimo_nodo = nodo_anterior;
			lista->ultimo_nodo->siguiente = NULL;
		} else {
			nodo_encontrado = nodo_anterior->siguiente;
			nodo_anterior->siguiente = nodo_encontrado->siguiente;
		}
	}
	if (elemento_quitado)
		*elemento_quitado = nodo_encontrado->elemento;

	free(nodo_encontrado);
	lista->cantidad_elementos--;
	return true;
}

void *lista_buscar_elemento(Lista *lista, void *buscado,
			    int (*comparador)(void *, void *))
{
	if (!lista || !comparador || lista_esta_vacia(lista))
		return NULL;

	int contador = 0;
	nodo_lista *nodo_actual = lista->primer_nodo;
	while (nodo_actual) {
		if (comparador(buscado, nodo_actual->elemento) == 0)
			return nodo_actual->elemento;
		contador++;
		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

bool lista_obtener_elemento(Lista *lista, size_t posicion,
			    void **elemento_encontrado)
{
	if (!lista || posicion >= lista->cantidad_elementos ||
	    lista_esta_vacia(lista))
		return false;

	nodo_lista *nodo_encontrado =
		buscar_nodo(lista->primer_nodo, posicion, 0);

	if (elemento_encontrado)
		*elemento_encontrado = nodo_encontrado->elemento;

	return true;
}

size_t lista_iterar_elementos(Lista *lista, bool (*f)(void *, void *),
			      void *ctx)
{
	if (!lista || !f || lista_esta_vacia(lista)) {
		return 0;
	}

	nodo_lista *nodo_actual = lista->primer_nodo;
	size_t iteraciones = 0;

	while (nodo_actual) {
		void *elemento_actual = nodo_actual->elemento;
		if (!f(elemento_actual, ctx)) {
			return iteraciones + 1;
		}
		nodo_actual = nodo_actual->siguiente;
		iteraciones++;
	}

	return lista->cantidad_elementos;
}

void lista_destruir_todo(Lista *lista, void (*destructor)(void *))
{
	if (!lista)
		return;
	nodo_lista *nodo_actual = lista->primer_nodo;
	while (nodo_actual) {
		nodo_lista *nodo_siguiente = nodo_actual->siguiente;
		if (destructor)
			destructor(nodo_actual->elemento);
		free(nodo_actual);
		nodo_actual = nodo_siguiente;
	}
	free(lista);
}

void lista_destruir(Lista *lista)
{
	if (!lista)
		return;
	lista_destruir_todo(lista, NULL);
}

// -------------- Iterador Externo --------------

struct lista_iterador {
	nodo_lista *nodo_actual;
};

Lista_iterador *lista_iterador_crear(Lista *lista)
{
	if (!lista)
		return NULL;

	Lista_iterador *iterador = malloc(sizeof(Lista_iterador));
	if (!iterador)
		return NULL;

	iterador->nodo_actual = lista->primer_nodo;
	return iterador;
}

bool lista_iterador_hay_siguiente(Lista_iterador *iterador)
{
	if (!iterador)
		return false;

	return iterador->nodo_actual != NULL;
}

void lista_iterador_avanzar(Lista_iterador *iterador)
{
	if (!iterador || !iterador->nodo_actual)
		return;

	iterador->nodo_actual = iterador->nodo_actual->siguiente;
}

void *lista_iterador_obtener_elemento_actual(Lista_iterador *iterador)
{
	if (!iterador || !lista_iterador_hay_siguiente(iterador))
		return NULL;
	return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir(Lista_iterador *iterador)
{
	if (!iterador)
		return;

	free(iterador);
}