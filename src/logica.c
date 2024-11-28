#include "../extra/engine.h"
#include "../extra/ansi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "verificaciones.h"
#include "lista.h"
#include "pokedex.h"
#include "tablero.h"
#include "logica.h"
#include "movimientos.h"
#include "pila.h"
#include "cola.h"
#include "cargar_elementos.h"
#include "menu.h"

const char CARACTER_USUARIO = '&';
const size_t CANTIDAD_FILAS = 15;
const size_t CANTIDAD_COLUMAS = 35;
const size_t CANTIDAD_POKEMONES_EN_TABLERO = 7;
const size_t TIEMPO_MAXIMO = 60;

typedef struct jugador_en_juego {
	char caracter;
	char *color;
	size_t cantidad_pasos;
	size_t puntaje;
	size_t multiplicador;
	size_t multiplicador_maximo;
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
	char *direccion_usuario;
	size_t posicion_pokemon_eliminado;
	informacion_t *informacion;
} juego_t;

void dibujar_tablero(tablero_t *tablero)
{	
	char* color_bordes = ANSI_COLOR_GREEN;
	size_t cantidad_filas = tablero_cantidad_filas(tablero);
	size_t cantidad_columnas = tablero_cantidad_columnas(tablero);
	
	printf("%s┏%s", color_bordes, ANSI_COLOR_RESET);
	for (size_t columna = 0; columna < cantidad_columnas; columna++) {
		printf("%s━%s", color_bordes, ANSI_COLOR_RESET);
	}
	printf("%s┓%s\n", color_bordes, ANSI_COLOR_RESET);

	for (size_t fila = 0; fila < cantidad_filas; fila++) {
		printf("%s┃%s", color_bordes, ANSI_COLOR_RESET);
		for (size_t columna = 0; columna < cantidad_columnas;
		     columna++) {
			char caracter;
			char* color_caracter;
			tablero_posicion_informacion(tablero, fila, columna, &caracter, &color_caracter);
			printf("%s%c%s", color_caracter, caracter, ANSI_COLOR_RESET);
		}
		printf("%s┃%s", color_bordes, ANSI_COLOR_RESET);
		printf("\n");
	}

	printf("%s┗%s", color_bordes, ANSI_COLOR_RESET);
	for (size_t columna = 0; columna < cantidad_columnas; columna++) {
		printf("%s━%s", color_bordes, ANSI_COLOR_RESET);
	}
	printf("%s┛%s\n", color_bordes, ANSI_COLOR_RESET);
}

bool imprimir_pokemones(pokemon_t *pokemon, void *_colores)
{
	hash_t *colores = (hash_t *)_colores;
	char *color = hash_buscar(colores, pokemon->color);
	printf("  %s~%s %s%s (%s) -> %s%d puntos\n", color, ANSI_COLOR_RESET,
	       color, pokemon->nombre, pokemon->movimientos, ANSI_COLOR_RESET,
	       pokemon->puntaje);
	return true;
}

bool mostrar_pokemones(void *_banderas)
{
	booleanos *banderas = (booleanos *)_banderas;
	banderas->pokedex = pokedex_cargada();
	banderas->colores = colores_cargados();
	if (!banderas->pokedex || !banderas->colores) {
		return false;
	}
	borrar_pantalla();
	pokedex_iterar(banderas->pokedex, imprimir_pokemones,
		       (void *)banderas->colores);
	return true;
}

void pokemon_movimiento(pokemon_seleccionado* pokemon , tablero_t* tablero, movimientos_t* movimientos, char* movimiento_usuario)
{
	for (size_t i = 0; i < strlen(pokemon->movimientos); i++) {
		char movimiento[2];
		movimiento[0] = pokemon->movimientos[i];
		movimiento[1] = '\0';
		size_t filas = tablero_cantidad_filas(tablero);
		size_t columnas = tablero_cantidad_columnas(tablero);
		movimiento_realizar(movimientos, movimiento_usuario, movimiento,
					&pokemon->y, &pokemon->x, filas, columnas);
	}
}

bool movimientos_pokemones(void *_pokemon, void *_dato)
{
	pokemon_seleccionado *pokemon = (pokemon_seleccionado *)_pokemon;
	juego_t *juego = (juego_t *)_dato;
	size_t y_antes = pokemon->y;
	size_t x_antes = pokemon->x;
	if (juego->direccion_usuario) {
		pokemon_movimiento(pokemon, juego->informacion->tablero, juego->informacion->movimientos, juego->direccion_usuario);
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

bool seleccion_de_pokemon(pokedex_t *pokedex, hash_t *colores,
			  Lista *seleccionados, tablero_t *tablero)
{
	size_t fila = 1 + (size_t)rand() % tablero_cantidad_filas(tablero);
	size_t columna =
		1 + (size_t)rand() % tablero_cantidad_columnas(tablero);
	size_t posicion_pokemon = (size_t)rand() % pokedex_cantidad(pokedex);
	pokemon_t *pokemon =
		pokedex_obtener_pokemon(pokedex, posicion_pokemon);
	char *color_obtenido = hash_buscar(colores, pokemon->color);
	pokemon_seleccionado *pokemon_objetivo =
		calloc(1, sizeof(pokemon_seleccionado));
	if (!pokemon_objetivo) {
		printf("No se pudo asignar memoria para el nuevo pokemon seleccionado");
		return false;
	}
	pokemon_objetivo->nombre = pokemon->nombre;
	pokemon_objetivo->caracter = pokemon->nombre[0];
	pokemon_objetivo->color = color_obtenido;
	pokemon_objetivo->movimientos = pokemon->movimientos;
	pokemon_objetivo->puntaje = (size_t)pokemon->puntaje;
	pokemon_objetivo->x = (size_t)columna;
	pokemon_objetivo->y = (size_t)fila;
	if (!lista_agregar_al_final(seleccionados, pokemon_objetivo)) {
		printf("No se pudo seleccionar un pokemon");
		return false;
	}
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
	if (!cola_encolar(nuevo_grupo, (void *)pokemon)) {
		printf("No se pudo encolar el pokemon capturado");
		return false;
	}
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
			if (usuario->multiplicador > usuario->multiplicador_maximo) {
				usuario->multiplicador_maximo = usuario->multiplicador;
			}
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

void mostrar_informacion_por_pantalla(size_t *tiempo, size_t tiempo_maximo, Pila *capturados,
				      usuario *usuario, tablero_t *tablero)
{
	(*tiempo)++;
	if ((tiempo_maximo - (*tiempo/5)) > 10) {
		printf("⏳%s%li%s\n", ANSI_COLOR_BLUE, tiempo_maximo - (*tiempo/5), ANSI_COLOR_RESET);
	} else {
		printf("⌛%s%li%s\n", ANSI_COLOR_RED, tiempo_maximo - (*tiempo/5), ANSI_COLOR_RESET);	
	}
	printf("👣%s%li%s 💲%s%li%s %s(x%li)%s\n", ANSI_COLOR_BLUE, usuario->cantidad_pasos, ANSI_COLOR_RESET, 
	ANSI_COLOR_GREEN, usuario->puntaje, ANSI_COLOR_RESET, ANSI_COLOR_YELLOW, usuario->multiplicador, ANSI_COLOR_RESET);
	dibujar_tablero(tablero);
	if (!pila_esta_vacía(capturados)) {
		pokemon_seleccionado *pokemon =
			(pokemon_seleccionado *)pila_tope(capturados);
		printf("Último Pokemon capturado: %s%s%s\n", pokemon->color,
		       pokemon->nombre, ANSI_COLOR_RESET);
	}
	printf("Cantidad capturados: %li\n", pila_cantidad(capturados));
}

void usuario_movimiento(tablero_t* tablero, usuario* usuario, char* tipo_movimiento, movimientos_t* movimientos, size_t fila_actual, size_t columna_actual)
{
	size_t filas = tablero_cantidad_filas(tablero);
	size_t columnas = tablero_cantidad_columnas(tablero);
	movimiento_realizar(movimientos, NULL, tipo_movimiento,
				&usuario->y, &usuario->x, filas, columnas);
	usuario->cantidad_pasos++;
	tablero_mover_elemento(tablero, fila_actual, columna_actual,
			usuario->y, usuario->x,
			usuario->caracter,
			usuario->color);
}

int logica_juego(int entrada, void *_dato)
{
	borrar_pantalla();
	informacion_t *dato = (informacion_t *)_dato;
	juego_t juego = { .direccion_usuario = NULL,
			  .informacion = dato,
			  .posicion_pokemon_eliminado = 0 };
	size_t x_antes = juego.informacion->personaje->x;
	size_t y_antes = juego.informacion->personaje->y;

	if (entrada == TECLA_DERECHA) {
		juego.direccion_usuario = "E";
	} else if (entrada == TECLA_IZQUIERDA) {
		juego.direccion_usuario = "O";
	} else if (entrada == TECLA_ARRIBA) {
		juego.direccion_usuario = "N";
	} else if (entrada == TECLA_ABAJO) {
		juego.direccion_usuario = "S";
	}

	if (juego.direccion_usuario) {
		usuario_movimiento(dato->tablero, dato->personaje, juego.direccion_usuario, dato->movimientos, y_antes, x_antes);
	}

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
	mostrar_informacion_por_pantalla(&dato->tiempo, dato->banderas->tiempo_maximo, 
					 dato->pokemones_capturados,
					 dato->personaje, dato->tablero);
	if ((dato->tiempo/5) == dato->banderas->tiempo_maximo) {
		entrada = 'q';
	}
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
	for (size_t i = 0; i < banderas->cantidad_objetivos; i++) {
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
	banderas->pokedex = pokedex_cargada();
	banderas->colores = colores_cargados();
	movimientos_t *movimientos = movimientos_cargados();
	if (!banderas->pokedex || !banderas->colores || !movimientos) {
		return false;
	}
	tablero_t *tablero = tablero_crear(banderas->cantidad_filas, banderas->cantidad_columas);
	if (!tablero)
		return false;
	esconder_cursor();
	if (!banderas->semilla) {
		srand((unsigned int)time(NULL));
	} else {
		srand((unsigned int)*banderas->semilla);
	}
	usuario jugador = { .caracter = CARACTER_USUARIO,
			    .color = ANSI_COLOR_WHITE,
			    .cantidad_pasos = 0,
			    .puntaje = 0,
			    .multiplicador = 1,
				.multiplicador_maximo = 1,
			    .y = 0,
			    .x = 0 };
	informacion_t *juego = inicializar_informacion(tablero, banderas,
						       &jugador, movimientos);
	if (!juego)
		return false;
	tablero_colocar_elemento(tablero, 0, 0, jugador.caracter, ANSI_COLOR_WHITE);
	game_loop(logica_juego, (void *)juego);
	printf("Maximo(s) grupo(s) formado(s):\n");
	if (lista_iterar_elementos(juego->grupos_formados,
			       mostrar_grupos_maximo_formado,
			       &juego->maximo_grupo_formado) == 0) {
		printf("Nigun grupo formado\n");
	}
	printf("Multiplicador maximo: %li\n", juego->personaje->multiplicador_maximo);
	printf("Tu puntaje final fue de: %li\n", juego->personaje->puntaje);
	mostrar_cursor();
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

// JUGAR CON SEMILLA

bool jugar_con_semilla(void *_banderas)
{
	char texto[MAX_CARACTERES];
	size_t semilla;
	bool seguir = true;
	while (seguir) {
		printf("Ingrese una semilla (solo valor numerico): ");
		if (es_numero(texto)) {
			semilla = strtoull(texto, NULL, 10);
			seguir = false;
		}
	}
	booleanos *banderas = (booleanos *)_banderas;
	banderas->semilla = &semilla;
	jugar_partida((void *)banderas);
	return true;
}

// OPCIONES ADICIONALES

bool imprimir_opciones_extras(void *_opcion, void *nada)
{
	opcion_menu_t *opcion = (opcion_menu_t *)_opcion;
	printf("   (" ANSI_COLOR_RED "%c" ANSI_COLOR_RESET ") " ANSI_COLOR_BLUE
	       "%s" ANSI_COLOR_RESET "\n",
	       opcion->indice, opcion->texto);
	return true;
}

bool modificar_cantidad_pokemones(void* _banderas)
{
	booleanos* banderas = (booleanos*)_banderas;
	char texto[MAX_CARACTERES];
	bool seguir = true;
	while (seguir) {
		printf("Ingrese la cantidad de objetivos (solo valor numerico): ");
		if (es_numero(texto)) {
			banderas->cantidad_objetivos = strtoull(texto, NULL, 10);
			seguir = false;
		}
	}
	return true;
}

bool modificar_tiempo(void* _banderas)
{
	booleanos* banderas = (booleanos*)_banderas;
	char texto[MAX_CARACTERES];
	bool seguir = true;
	while (seguir) {
		printf("Ingrese la duración del juego (solo valor numerico): ");
		if (es_numero(texto)) {
			banderas->tiempo_maximo = strtoull(texto, NULL, 10);
			seguir = false;
		}
	}
	return true;
}

size_t pedir_numero(const char *mensaje) {
    char texto[MAX_CARACTERES];
    while (true) {
        printf("%s", mensaje);
        if (es_numero(texto)) {
            return strtoull(texto, NULL, 10);
    	}
	}
}

bool modificar_dimensiones_tablero(void* _banderas)
{
	booleanos* banderas = (booleanos*)_banderas;
    banderas->cantidad_filas = pedir_numero("Ingrese cantidad de filas (solo valor numérico): ");
    banderas->cantidad_columas = pedir_numero("Ingrese cantidad de columnas (solo valor numérico): ");
	return true;
}

bool cambiar_caracter(void* _banderas)
{
	return true;
}

bool volver_al_menu(void* _banderas)
{	
	booleanos* banderas = (booleanos*)_banderas;
	banderas->opciones_seguir = false;
	return true;
}

bool agregar_opciones_extras(menu_t* opciones_extras)
{
	menu_ingresar_opcion(opciones_extras, 'M', "Modificar cantidad de Pokemones en Juego", modificar_cantidad_pokemones);
	menu_ingresar_opcion(opciones_extras, 'T', "Modifcar Tiempo", modificar_tiempo);
	menu_ingresar_opcion(opciones_extras, 'F', "Modificar cantidad Filas y Columnas", modificar_dimensiones_tablero);
	menu_ingresar_opcion(opciones_extras, 'C', "Elegir caracter preferido", cambiar_caracter);
	menu_ingresar_opcion(opciones_extras, 'V', "Volver al menu", volver_al_menu);

	if (menu_cantidad(opciones_extras) != 5) {
		menu_destruir(opciones_extras);
		return false;
	}
	return true;
}

bool opciones_jugar(void* _banderas)
{	
	booleanos* banderas = (booleanos*)_banderas;
	menu_t *opciones_extras = menu_crear();
	if (!opciones_extras) {
		printf("No se pudo crear el menú de opciones\n");
		salir_del_menu(_banderas);
		return false;
	}

	if (!agregar_opciones_extras(opciones_extras)) {
		printf("Fallo en la creación de las opciones\n");
		salir_del_menu(_banderas);
		return false;
	}
	borrar_pantalla();
	char texto[MAX_CARACTERES];
	banderas->opciones_seguir = true;
	while (banderas->opciones_seguir) {
		menu_iterar_opciones(opciones_extras, imprimir_opciones_extras, NULL);
		if (es_caracter(texto)) {
			char mayuscula = caracter_mayuscula(texto);
			menu_ejecutar_opcion(opciones_extras, mayuscula,
					     (void *)banderas);
		}
	}
	menu_destruir(opciones_extras);
	borrar_pantalla();
	return true;
}

// restablecer valores

bool restablecer_valores(void *_banderas) 
{
	booleanos* banderas = (booleanos*)_banderas;
	banderas->cantidad_objetivos = 7;
	banderas->cantidad_filas = 15;
	banderas->cantidad_columas = 32;
	banderas->tiempo_maximo = 60;
	return true;

}

// SALIR_DE_MENU

bool salir_del_menu(void *_banderas)
{
	(*(booleanos *)_banderas).menu_seguir = false;
	return true;
}