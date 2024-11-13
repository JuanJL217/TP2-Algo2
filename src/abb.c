#include "abb.h"
#include "abb_estructura_privada.h"

// --------------- Estructura para las funciones iterativas ---------------

typedef struct informacion {
	void **vector;
	size_t iteracion;
	size_t tope;
} informacion_t;

// ------------------ FUNCIONES EXTRAS ------------------

bool asignar_elementos_en_vector(void *elemento, void *vector)
{
	((informacion_t *)vector)->vector[((informacion_t *)vector)->iteracion] =
		elemento;
	return true;
}

nodo_t *nodo_crear(void *elemento)
{
	nodo_t *nodo = calloc(1, sizeof(nodo_t));
	if (!nodo)
		return NULL;
	nodo->elemento = elemento;
	return nodo;
}

nodo_t **buscar_nodo(abb_t *abb, nodo_t **nodo_actual, void *elemento)
{
	if (!*nodo_actual ||
	    abb->comparador(elemento, (*nodo_actual)->elemento) == 0) {
		return nodo_actual;
	} else if (abb->comparador(elemento, (*nodo_actual)->elemento) < 0) {
		return buscar_nodo(abb, &(*nodo_actual)->izq, elemento);
	} else {
		return buscar_nodo(abb, &(*nodo_actual)->der, elemento);
	}
}

nodo_t **buscar_predecesor_inorden(nodo_t **nodo_actual)
{
	if (!(*nodo_actual) || !(*nodo_actual)->der) {
		return nodo_actual;
	}
	return buscar_predecesor_inorden(&((*nodo_actual)->der));
}

void borrar_nodo_hoja(abb_t *abb, nodo_t **puntero_a_hijo)
{
	(*puntero_a_hijo) = NULL;
}

void borrar_nodo_con_hijos(abb_t *abb, nodo_t **puntero_a_hijo)
{
	nodo_t **nodo_reemplazo =
		buscar_predecesor_inorden(&(*puntero_a_hijo)->izq);
	nodo_t *nodo_reemplazo_guardado = *nodo_reemplazo;
	(*nodo_reemplazo) = (*nodo_reemplazo)->izq;
	nodo_reemplazo_guardado->izq = (*puntero_a_hijo)->izq;
	nodo_reemplazo_guardado->der = (*puntero_a_hijo)->der;
	(*puntero_a_hijo) = nodo_reemplazo_guardado;
}

void borrar_nodo_con_un_hijo(abb_t *abb, nodo_t **puntero_a_hijo)
{
	if ((*puntero_a_hijo)->izq)
		*puntero_a_hijo = (*puntero_a_hijo)->izq;
	else
		*puntero_a_hijo = (*puntero_a_hijo)->der;
}

// ---------------------- FUNCIONES PRINCIPALES -------------------

abb_t *abb_crear(int (*comparador)(void *, void *))
{
	if (!comparador)
		return NULL;
	abb_t *inicializar_abb = calloc(1, sizeof(abb_t));
	if (!inicializar_abb)
		return NULL;
	inicializar_abb->comparador = comparador;
	return inicializar_abb;
}

void destruir_todo_el_abb(nodo_t *nodo_actual, void (*destructor)(void *))
{
	if (!nodo_actual)
		return;
	destruir_todo_el_abb(nodo_actual->izq, destructor);
	destruir_todo_el_abb(nodo_actual->der, destructor);
	if (destructor)
		destructor(nodo_actual->elemento);
	free(nodo_actual);
}

bool abb_insertar(abb_t *abb, void *elemento)
{
	if (!abb)
		return false;

	nodo_t *nuevo_nodo = nodo_crear(elemento);
	if (!nuevo_nodo)
		return false;

	nodo_t **puntero_entre_padre_e_hijo =
		buscar_nodo(abb, &(abb->raiz), elemento);
	if (!*puntero_entre_padre_e_hijo) {
		(*puntero_entre_padre_e_hijo) = nuevo_nodo;
	} else {
		nodo_t **nodo_encontrado = buscar_predecesor_inorden(
			&((*puntero_entre_padre_e_hijo)->izq));
		if (!(*nodo_encontrado))
			*nodo_encontrado = nuevo_nodo;
		else
			(*nodo_encontrado)->der = nuevo_nodo;
	}

	abb->nodos++;
	return true;
}

bool abb_quitar(abb_t *abb, void *buscado, void **encontrado)
{
	if (!abb || !abb->raiz)
		return false;
	nodo_t **puntero_entre_padre_e_hijo =
		buscar_nodo(abb, &(abb->raiz), buscado);
	if (!*puntero_entre_padre_e_hijo)
		return false;
	nodo_t *nodo_guardado = (*puntero_entre_padre_e_hijo);
	if (!(*puntero_entre_padre_e_hijo)->izq &&
	    !(*puntero_entre_padre_e_hijo)->der)
		borrar_nodo_hoja(abb, puntero_entre_padre_e_hijo);
	else if ((*puntero_entre_padre_e_hijo)->izq &&
		 (*puntero_entre_padre_e_hijo)->der)
		borrar_nodo_con_hijos(abb, puntero_entre_padre_e_hijo);
	else
		borrar_nodo_con_un_hijo(abb, puntero_entre_padre_e_hijo);
	if (encontrado)
		*encontrado = nodo_guardado->elemento;
	free(nodo_guardado);
	abb->nodos--;
	return true;
}

void *abb_obtener(abb_t *abb, void *elemento)
{
	if (!abb || !abb->raiz)
		return NULL;
	nodo_t **puntero_entre_padre_e_hijo =
		buscar_nodo(abb, &(abb->raiz), elemento);
	if (!*puntero_entre_padre_e_hijo)
		return NULL;
	return (*puntero_entre_padre_e_hijo)->elemento;
}

size_t abb_cantidad(abb_t *abb)
{
	if (!abb)
		return 0;
	return abb->nodos;
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (!abb)
		return;
	destruir_todo_el_abb(abb->raiz, destructor);
	free(abb);
}

void abb_destruir(abb_t *abb)
{
	abb_destruir_todo(abb, NULL);
}

// ---------------------ITERADORES INTERNOS ---------------------

// INORDEN

bool recorrido_inorden(nodo_t *nodo_actual, bool (*f)(void *, void *),
		       void *ctx, informacion_t *informacion)
{
	if (!nodo_actual)
		return true;
	if (!recorrido_inorden(nodo_actual->izq, f, ctx, informacion))
		return false;
	if (informacion->iteracion == informacion->tope ||
	    !f(nodo_actual->elemento, ctx))
		return false;
	informacion->iteracion++;
	if (!recorrido_inorden(nodo_actual->der, f, ctx, informacion))
		return false;
	return true;
}

size_t abb_iterar_inorden(abb_t *abb, bool (*f)(void *, void *), void *ctx)
{
	if (!abb || !f)
		return 0;
	informacion_t informacion = { .vector = NULL,
				      .iteracion = 0,
				      .tope = abb->nodos };
	return !recorrido_inorden(abb->raiz, f, ctx, &informacion) ?
		       informacion.iteracion + 1 :
		       abb->nodos;
}

size_t abb_vectorizar_inorden(abb_t *abb, void **vector, size_t tamaño)
{
	if (!abb || !vector)
		return 0;
	informacion_t informarcion = { .vector = vector,
				       .iteracion = 0,
				       .tope = tamaño };
	return !recorrido_inorden(abb->raiz, asignar_elementos_en_vector,
				  &informarcion, &informarcion) ?
		       informarcion.iteracion :
		       abb->nodos;
}

// PREORDEN

bool recorrido_preorden(nodo_t *nodo_actual, bool (*f)(void *, void *),
			void *ctx, informacion_t *informacion)
{
	if (!nodo_actual)
		return true;
	if (informacion->iteracion == informacion->tope ||
	    !f(nodo_actual->elemento, ctx))
		return false;
	informacion->iteracion++;
	if (!recorrido_preorden(nodo_actual->izq, f, ctx, informacion) ||
	    !recorrido_preorden(nodo_actual->der, f, ctx, informacion))
		return false;
	return true;
}

size_t abb_iterar_preorden(abb_t *abb, bool (*f)(void *, void *), void *ctx)
{
	if (!abb)
		return 0;
	informacion_t informacion = { .vector = NULL,
				      .iteracion = 0,
				      .tope = abb->nodos };
	return !recorrido_preorden(abb->raiz, f, ctx, &informacion) ?
		       informacion.iteracion + 1 :
		       abb->nodos;
}

size_t abb_vectorizar_preorden(abb_t *abb, void **vector, size_t tamaño)
{
	if (!abb || !vector)
		return 0;
	informacion_t informarcion_posiciones = { .vector = vector,
						  .iteracion = 0,
						  .tope = tamaño };
	return !recorrido_preorden(abb->raiz, asignar_elementos_en_vector,
				   &informarcion_posiciones,
				   &informarcion_posiciones) ?
		       informarcion_posiciones.iteracion :
		       abb->nodos;
}

// POSTORDEN

bool recorrido_postorden(nodo_t *nodo_actual, bool (*f)(void *, void *),
			 void *ctx, informacion_t *informacion)
{
	if (!nodo_actual)
		return true;
	if (!recorrido_postorden(nodo_actual->izq, f, ctx, informacion) ||
	    !recorrido_postorden(nodo_actual->der, f, ctx, informacion))
		return false;
	if (informacion->iteracion == informacion->tope ||
	    !f(nodo_actual->elemento, ctx))
		return false;
	informacion->iteracion++;
	return true;
}

size_t abb_iterar_postorden(abb_t *abb, bool (*f)(void *, void *), void *ctx)
{
	if (!abb)
		return 0;
	informacion_t informacion = { .vector = NULL,
				      .iteracion = 0,
				      .tope = abb->nodos };
	return !recorrido_postorden(abb->raiz, f, ctx, &informacion) ?
		       informacion.iteracion + 1 :
		       abb->nodos;
}

size_t abb_vectorizar_postorden(abb_t *abb, void **vector, size_t tamaño)
{
	if (!abb || !vector)
		return 0;
	informacion_t informarcion_posiciones = { .vector = vector,
						  .iteracion = 0,
						  .tope = tamaño };
	return !recorrido_postorden(abb->raiz, asignar_elementos_en_vector,
				    &informarcion_posiciones,
				    &informarcion_posiciones) ?
		       informarcion_posiciones.iteracion :
		       abb->nodos;
}