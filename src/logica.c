#include "../extra/engine.h"
#include "../extra/ansi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "lista.h"
#include "pokedex.h"
#include "tablero.h"
#include "colores.h"
#include "logica.h"
#include "movimientos.h"
#include "pila.h"
#include "cola.h"

const char *POSICION = "posicion";
const char *CARACTER = "caracter";
const char *COLOR = "color";
const char MI_CARACTER = '&';
const size_t CANTIDAD_POKEMONES_EN_TABLERO = 7;

typedef struct jugador_en_juego {
	char caracter;
	char *color;
	size_t cantidad_pasos;
	size_t puntaje;
	size_t multiplicador;
	size_t y;
	size_t x;
} usuario;

typedef struct pokemon_en_juego {
	char *nombre;
	char caracter;
	char *color;
	char *movimientos;
	size_t puntaje;
	size_t y;
	size_t x;
} pokemon_seleccionado;

typedef struct informacion {
	usuario *personaje;
	Lista *pokemones_seleccionados;
	Lista *pokemones_eliminados;
	tablero_t *tablero;
	movimientos_t *movimientos;
	booleanos *banderas;
	Pila *pokemones_capturados;
	Lista *grupos_formados;
	size_t maximo_grupo_formado;
	size_t contador_grupos;
	size_t tiempo;
} informacion_t;

typedef struct juego {
	char *direccion;
	size_t posicion_pokemon_eliminado;
	informacion_t *informacion;
} juego_t;

bool imprimir_pokemones(void *_pokemon, void *_colores)
{
	pokemon_t *pokemon = (pokemon_t *)_pokemon;
	colores_t *colores = (colores_t *)_colores;
	char *color = color_obtener(colores, pokemon->color);
	printf("  %s~%s %s%s (%s) -> %s%d puntos\n", color, ANSI_COLOR_RESET,
	       color, pokemon->nombre, pokemon->movimientos, ANSI_COLOR_RESET,
	       pokemon->puntaje);
	return true;
}

pokedex_t *crear_y_cargar_pokedex(pokedex_t *bandera_pokedex)
{
	if (bandera_pokedex)
		return bandera_pokedex;
	pokedex_t *pokedex = pokedex_crear();
	if (!pokedex) {
		printf("Error al iniciar la pokedex");
		return NULL;
	}
	if (!pokedex_cargar(pokedex, "pokedex.csv", ',')) {
		printf("Error al parsear datos");
		pokedex_destruir(pokedex);
		return NULL;
	}
	return pokedex;
}

colores_t *crear_y_cargar_colores(colores_t *bandera_color)
{
	if (bandera_color)
		return bandera_color;
	colores_t *colores = colores_crear();
	if (!colores) {
		printf("Error al iniciar los colores");
		return NULL;
	}
	if (!colores_cargar(colores)) {
		printf("Error al cargar los colores");
		colores_destruir(colores);
		return NULL;
	}
	return colores;
}

bool mostrar_pokemones(void *_banderas)
{
	booleanos *banderas = (booleanos *)_banderas;
	banderas->pokedex = crear_y_cargar_pokedex(banderas->pokedex);
	banderas->colores = crear_y_cargar_colores(banderas->colores);
	if (!banderas->pokedex || !banderas->colores) {
		return false;
	}
	borrar_pantalla();
	pokedex_iterar(banderas->pokedex, imprimir_pokemones,
		       (void *)banderas->colores);
	return true;
}

bool movimientos_pokemones(void *_pokemon, void *_dato)
{
	pokemon_seleccionado *pokemon = (pokemon_seleccionado *)_pokemon;
	juego_t *juego = (juego_t *)_dato;
	size_t y_antes = pokemon->y;
	size_t x_antes = pokemon->x;
	if (juego->direccion) {
		for (size_t i = 0; i < strlen(pokemon->movimientos); i++) {
			char movimiento[2];
			movimiento[0] = pokemon->movimientos[i];
			movimiento[1] = '\0';
			movimiento_realizar(juego->informacion->movimientos,
					    juego->direccion, movimiento,
					    &pokemon->y, &pokemon->x);
		}
	}
	if (juego->informacion->personaje->x == pokemon->x &&
	    juego->informacion->personaje->y == pokemon->y) {
		tablero_mover_elemento(juego->informacion->tablero, y_antes,
				       x_antes, pokemon->y, pokemon->x,
				       juego->informacion->personaje->caracter,
				       juego->informacion->personaje->color);
		size_t *posicion_pokemon_capturado = calloc(1, sizeof(size_t));
		if (!posicion_pokemon_capturado) {
			return false;
		}
		*posicion_pokemon_capturado = juego->posicion_pokemon_eliminado;
		printf("%li\n", *posicion_pokemon_capturado);
		lista_agregar_al_final(juego->informacion->pokemones_eliminados,
				       (void *)posicion_pokemon_capturado);
	} else if (juego->informacion->personaje->x == x_antes &&
		   juego->informacion->personaje->y == y_antes) {
		tablero_mover_elemento(juego->informacion->tablero, pokemon->y,
				       pokemon->x, pokemon->y, pokemon->x,
				       juego->informacion->personaje->caracter,
				       juego->informacion->personaje->color);
	} else {
		tablero_mover_elemento(juego->informacion->tablero, y_antes,
				       x_antes, pokemon->y, pokemon->x,
				       pokemon->caracter, pokemon->color);
	}
	(juego->posicion_pokemon_eliminado)++;
	return true;
}

bool seleccion_de_pokemon(pokedex_t *pokedex, colores_t *colores,
			  Lista *seleccionados, tablero_t *tablero)
{
	size_t fila = 1 + (size_t)rand() % tablero_cantidad_filas(tablero);
	size_t columna =
		1 + (size_t)rand() % tablero_cantidad_columnas(tablero);
	size_t posicion_pokemon = (size_t)rand() % pokedex_cantidad(pokedex);
	pokemon_t *pokemon =
		pokedex_obtener_pokemon(pokedex, (size_t)posicion_pokemon);
	char *color_obtenido = color_obtener(colores, pokemon->color);
	pokemon_seleccionado *pokemon_objetivo =
		calloc(1, sizeof(pokemon_seleccionado));
	if (!pokemon_objetivo)
		return false;
	pokemon_objetivo->nombre = pokemon->nombre;
	pokemon_objetivo->caracter = pokemon->nombre[0];
	pokemon_objetivo->color = color_obtenido;
	pokemon_objetivo->movimientos = pokemon->movimientos;
	pokemon_objetivo->puntaje = (size_t)pokemon->puntaje;
	pokemon_objetivo->x = (size_t)columna;
	pokemon_objetivo->y = (size_t)fila;
	lista_agregar_al_final(seleccionados, pokemon_objetivo);
	tablero_colocar_elemento(tablero, fila, columna,
				 pokemon_objetivo->caracter, color_obtenido);
	return true;
}

bool agregar_nueva_cola(Pila *capturados, pokemon_seleccionado *pokemon,
			Lista *grupos)
{
	pila_apilar(capturados, (void *)pokemon);
	Cola *nuevo_grupo = cola_crear();
	if (!nuevo_grupo) {
		printf("No se pudo crear la cola para formar grupos");
		return false;
	}
	cola_encolar(nuevo_grupo, (void *)pokemon);
	lista_agregar_al_final(grupos, (void *)nuevo_grupo);
	return true;
}

bool apilar_pokemon_captruado(Pila *capturados, pokemon_seleccionado *pokemon,
			      Lista *grupos, size_t *contador_grupos,
			      size_t *contador_grupo_maximo, usuario *usuario)
{
	usuario->puntaje += (pokemon->puntaje * usuario->multiplicador);
	if (pila_esta_vacía(capturados)) {
		if (!agregar_nueva_cola(capturados, pokemon, grupos)) {
			return false;
		}
		(*contador_grupos)++;
		(*contador_grupo_maximo)++;
	} else {
		pokemon_seleccionado *ultimo_capturado =
			(pokemon_seleccionado *)pila_tope(capturados);
		if (pokemon->caracter == ultimo_capturado->caracter ||
		    strcmp(pokemon->color, ultimo_capturado->color) == 0) {
			usuario->multiplicador++;
			Cola *grupo_formado;
			lista_obtener_elemento(
				grupos, lista_cantidad_elementos(grupos) - 1,
				(void **)&grupo_formado);
			cola_encolar(grupo_formado, (void *)pokemon);
			(*contador_grupos)++;
			if (*contador_grupos > *contador_grupo_maximo) {
				*contador_grupo_maximo = *contador_grupos;
			}
			pila_apilar(capturados, (void *)pokemon);
		} else {
			usuario->multiplicador = 1;
			(*contador_grupos) = 1;
			if (!agregar_nueva_cola(capturados, pokemon, grupos)) {
				return false;
			}
		}
	}
	return true;
}

bool procesar_pokemon_capturado(Lista *posiciones, Lista *seleccionados,
				usuario *usuario, Lista *grupos,
				Pila *capturados, size_t *contador_grupos,
				size_t *contador_grupo_maximo)
{
	size_t cantidad_pokemones_atrapados =
		lista_cantidad_elementos(posiciones);
	for (size_t i = 0; i < cantidad_pokemones_atrapados; i++) {
		size_t *posicion;
		lista_obtener_elemento(posiciones, 0, (void **)&posicion);
		lista_quitar_elemento(posiciones, 0, NULL);
		pokemon_seleccionado *pokemon;
		lista_quitar_elemento(seleccionados, *posicion - i,
				      (void **)&pokemon);
		free(posicion);
		if (i == cantidad_pokemones_atrapados - 1) {
			if (!apilar_pokemon_captruado(capturados, pokemon,
						      grupos, contador_grupos,
						      contador_grupo_maximo,
						      usuario)) {
				return false;
			}
		} else {
			free(pokemon);
		}
	}
	return true;
}

void mostrar_informacion_por_pantalla(size_t *tiempo, Pila *capturados,
				      usuario *usuario, tablero_t *tablero,
				      int *entrada)
{
	(*tiempo)++;
	printf("Tiempo: %li\n", (*tiempo) / 5);
	printf("Pasos: %li, Puntos: %li, Multiplicador: x%li\n",
	       usuario->cantidad_pasos, usuario->puntaje,
	       usuario->multiplicador);
	tablero_mostrar(tablero);
	if (!pila_esta_vacía(capturados)) {
		pokemon_seleccionado *pokemon =
			(pokemon_seleccionado *)pila_tope(capturados);
		printf("Pokemon capturado: %s%s%s\n", pokemon->color,
		       pokemon->nombre, ANSI_COLOR_RESET);
	}
	printf("Cantidad capturados: %li\n", pila_cantidad(capturados));
	if ((*tiempo) / 5 == 61) {
		*entrada = 'q';
	}
}

int logica_juego(int entrada, void *_dato)
{
	borrar_pantalla();
	informacion_t *dato = (informacion_t *)_dato;
	juego_t juego = { .direccion = NULL,
			  .informacion = dato,
			  .posicion_pokemon_eliminado = 0 };
	size_t x_antes = juego.informacion->personaje->x;
	size_t y_antes = juego.informacion->personaje->y;
	juego.direccion = NULL;
	if (entrada == TECLA_DERECHA) {
		juego.direccion = "E";
	} else if (entrada == TECLA_IZQUIERDA) {
		juego.direccion = "O";
	} else if (entrada == TECLA_ARRIBA) {
		juego.direccion = "N";
	} else if (entrada == TECLA_ABAJO) {
		juego.direccion = "S";
	}
	if (juego.direccion) {
		movimiento_realizar(dato->movimientos, NULL, juego.direccion,
				    &dato->personaje->y, &dato->personaje->x);
		dato->personaje->cantidad_pasos++;
	}
	tablero_mover_elemento(dato->tablero, y_antes, x_antes,
			       dato->personaje->y, dato->personaje->x,
			       dato->personaje->caracter,
			       dato->personaje->color);
	lista_iterar_elementos(dato->pokemones_seleccionados,
			       movimientos_pokemones, (void *)&juego);
	size_t cantidad_eliminados =
		lista_cantidad_elementos(dato->pokemones_eliminados);
	for (size_t i = 0; i < cantidad_eliminados; i++) {
		if (!seleccion_de_pokemon(
			    dato->banderas->pokedex, dato->banderas->colores,
			    dato->pokemones_seleccionados, dato->tablero)) {
			printf("Erorr al asignar memoria para almacenar el objetivo");
			entrada = 'q';
		}
	}
	if (!procesar_pokemon_capturado(
		    dato->pokemones_eliminados, dato->pokemones_seleccionados,
		    dato->personaje, dato->grupos_formados,
		    dato->pokemones_capturados, &dato->contador_grupos,
		    &dato->maximo_grupo_formado)) {
		return false;
	}
	mostrar_informacion_por_pantalla(&dato->tiempo,
					 dato->pokemones_capturados,
					 dato->personaje, dato->tablero,
					 &entrada);
	return entrada == 'q' || entrada == 'Q';
}

void destruir_pokemones_seleccionados(void *_pokemon)
{
	free((pokemon_seleccionado *)_pokemon);
}

void destruir_pokemones_capturados(void *_pokemon)
{
	free((pokemon_seleccionado *)_pokemon);
}

bool mostrar_grupos_maximo_formado(void *_grupo_formado, void *cantidad_maxima)
{
	Cola *grupo_formado = (Cola *)_grupo_formado;
	if (cola_cantidad(grupo_formado) == *(size_t *)cantidad_maxima) {
		while (!cola_esta_vacía(grupo_formado)) {
			pokemon_seleccionado *pokemon =
				(pokemon_seleccionado *)cola_desencolar(
					grupo_formado);
			printf("%s%c%s", pokemon->color, pokemon->caracter,
			       ANSI_COLOR_RESET);
			free(pokemon);
		}
		printf("\n");
	}
	return true;
}

void destruir_colas(void *_cola)
{
	cola_destruir_todo((Cola *)_cola, destruir_pokemones_capturados);
}

void destruir_almacenacimientos_pokemon(Lista *grupos_formados,
					Lista *objetivos_pokemones,
					Lista *pokemones_eliminados,
					Pila *pokemones_capturados)
{
	lista_destruir_todo(grupos_formados, destruir_colas);
	lista_destruir_todo(objetivos_pokemones,
			    destruir_pokemones_seleccionados);
	lista_destruir(pokemones_eliminados);
	pila_destruir(pokemones_capturados);
}

informacion_t *inicializar_informacion(tablero_t *tablero, booleanos *banderas,
				       usuario *usuario,
				       movimientos_t *movimientos)
{
	cargar_movimientos(movimientos);
	Lista *objetivos_pokemones = lista_crear();
	Lista *pokemones_eliminados = lista_crear();
	Lista *grupos_formados = lista_crear();
	Pila *pokemones_capturados = pila_crear();
	if (!objetivos_pokemones || !pokemones_eliminados ||
	    !pokemones_capturados || !grupos_formados) {
		destruir_almacenacimientos_pokemon(objetivos_pokemones,
						   pokemones_eliminados,
						   grupos_formados,
						   pokemones_capturados);
		return NULL;
	}
	for (size_t i = 0; i < CANTIDAD_POKEMONES_EN_TABLERO; i++) {
		seleccion_de_pokemon(banderas->pokedex, banderas->colores,
				     objetivos_pokemones, tablero);
	}
	informacion_t *informacion = calloc(1, sizeof(informacion_t));
	if (!informacion) {
		printf("No se pudo inicializar la información para el juego");
		destruir_almacenacimientos_pokemon(objetivos_pokemones,
						   pokemones_eliminados,
						   grupos_formados,
						   pokemones_capturados);
		return NULL;
	}
	informacion->personaje = usuario;
	informacion->tablero = tablero;
	informacion->movimientos = movimientos;
	informacion->pokemones_seleccionados = objetivos_pokemones;
	informacion->pokemones_eliminados = pokemones_eliminados;
	informacion->pokemones_capturados = pokemones_capturados;
	informacion->grupos_formados = grupos_formados;
	informacion->banderas = banderas;
	informacion->maximo_grupo_formado = 0;
	informacion->contador_grupos = 0;
	informacion->tiempo = 0;
	return informacion;
}

bool jugar_partida(void *_banderas)
{
	borrar_pantalla();
	booleanos *banderas = (booleanos *)_banderas;
	banderas->pokedex = crear_y_cargar_pokedex(banderas->pokedex);
	banderas->colores = crear_y_cargar_colores(banderas->colores);
	if (!banderas->pokedex || !banderas->colores) {
		return false;
	}
	tablero_t *tablero = tablero_crear(15, 32);
	if (!tablero)
		return false;
	esconder_cursor();
	if (!banderas->semilla) {
		srand((unsigned int)time(NULL));
	} else {
		srand((unsigned int)*banderas->semilla);
	}
	movimientos_t *movimientos = crear_movimiento(15, 32);
	usuario jugador = { .caracter = MI_CARACTER,
			    .color = ANSI_COLOR_WHITE,
			    .cantidad_pasos = 0,
			    .puntaje = 0,
			    .multiplicador = 1,
			    .y = 0,
			    .x = 0 };
	informacion_t *juego = inicializar_informacion(tablero, banderas,
						       &jugador, movimientos);
	if (!juego)
		return false;
	tablero_colocar_elemento(tablero, 0, 0, MI_CARACTER, ANSI_COLOR_WHITE);
	game_loop(logica_juego, (void *)juego);
	mostrar_cursor();
	printf("Maximo(s) grupo(s) formado(s): ");
	lista_iterar_elementos(juego->grupos_formados,
			       mostrar_grupos_maximo_formado,
			       &juego->maximo_grupo_formado);
	destruir_almacenacimientos_pokemon(juego->grupos_formados,
					   juego->pokemones_seleccionados,
					   juego->pokemones_eliminados,
					   juego->pokemones_capturados);
	tablero_destruir(tablero);
	movimientos_destruir(movimientos);
	free(juego);
	banderas->menu_seguir = false;
	return true;
}

bool jugar_con_semilla(void *_banderas)
{
	char texto[MAX_CARACTERES];
	size_t semilla;
	bool seguir = true;
	while (seguir) {
		printf("Ingrese una semilla (solo valor numerico): ");
		if (fgets(texto, sizeof(texto), stdin)) {
			texto[strcspn(texto, "\n")] = '\0';
			bool es_numero = true;
			for (size_t i = 0; texto[i] != '\0'; i++) {
				if (!isdigit((unsigned int)texto[i])) {
					es_numero = false;
					break;
				}
			}
			if (es_numero) {
				semilla = strtoull(texto, NULL, 10);
				seguir = false;
			}
		}
	}
	booleanos *banderas = (booleanos *)_banderas;
	banderas->semilla = &semilla;
	jugar_partida((void *)banderas);
	return true;
}

bool salir_del_menu(void *ctx)
{
	(*(booleanos *)ctx).menu_seguir = false;
	return true;
}