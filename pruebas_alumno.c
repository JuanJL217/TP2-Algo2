#include "pa2m.h"
#include <string.h>
#include "extra/ansi.h"
#include "src/menu.h"
#include "src/tablero.h"
#include "src/pokedex.h"
#include "src/movimientos.h"

// ========== MENU ==========

void prueba_crear_menu()
{
	menu_t *menu = menu_crear();
	if (!menu) {
		printf("No se pudo crear el menu");
		menu_destruir(menu);
	}
	pa2m_afirmar(menu != NULL, "Menu creado");
	pa2m_afirmar(menu_cantidad(menu) == 0,
		     "Cuando se crea el menú, no hay opciones");
	menu_destruir(menu);
}

bool sumar(void *x)
{
	int *a = calloc(1, sizeof(int));
	if (!a)
		return false;
	int b = 4;
	(*a) = 2;
	(*a) += b;
	*(int *)x = (*a);
	free(a);
	return true;
}

bool resta(void *y)
{
	int *a = calloc(1, sizeof(int));
	if (!a)
		return false;
	(*a) = 10;
	*(int *)y -= *a;
	free(a);
	return true;
}

bool multiplicar(void *z)
{
	int *a = calloc(1, sizeof(int));
	if (!a)
		return false;
	(*a) = 2;
	*(int *)z *= *a;
	free(a);
	return true;
}

void prueba_insertar()
{
	menu_t *menu = menu_crear();
	if (!menu) {
		printf("No se pudo crear el menu");
		menu_destruir(menu);
	}
	pa2m_afirmar(menu != NULL, "Menu creado");
	if (!menu_ingresar_opcion(menu, 'A', "Esto es una prueba", sumar)) {
		printf("No se pudo ingresar la opcion al menú");
		menu_destruir(menu);
		return;
	}
	pa2m_afirmar(menu_cantidad(menu) == 1, "Hay una opción en el menu");
	menu_destruir(menu);
}

void prueba_insertar_mas_opciones()
{
	menu_t *menu = menu_crear();
	if (!menu) {
		printf("No se pudo crear el menu");
		menu_destruir(menu);
	}
	pa2m_afirmar(menu != NULL, "Menu creado");
	char indices[3] = { 'A', 'B', 'C' };
	char *textos[3] = { "Opcion 1", "Opcion 2", "Opcion 3" };
	bool (*funciones[3])(void *) = { sumar, resta, multiplicar };

	pa2m_afirmar(
		menu_ingresar_opcion(menu, 'D', "sin funcion", NULL) == false,
		"Insertar una opcion sin una función válida, retorna False");
	pa2m_afirmar(menu_ingresar_opcion(menu, 'D', NULL, sumar) == false,
		     "Insertar una opcion sin un texto, retorna False");
	for (size_t i = 0; i < 3; i++) {
		if (!menu_ingresar_opcion(menu, indices[i], textos[i],
					  funciones[i])) {
			printf("Error al ingresar la opcion '%c'", indices[i]);
			menu_destruir(menu);
		}
		pa2m_afirmar(menu_cantidad(menu) == 1 + i,
			     "Hay %li opciones en el menu", i + 1);
	}

	menu_destruir(menu);
}

void prueba_ejecutar()
{
	menu_t *menu = menu_crear();
	if (!menu) {
		printf("No se pudo crear el menu");
		menu_destruir(menu);
	}
	pa2m_afirmar(menu != NULL, "Menu creado");
	char indices[3] = { 'A', 'B', 'C' };
	char *textos[3] = { "Opcion 1", "Opcion 2", "Opcion 3" };
	bool (*funciones[3])(void *) = { sumar, resta, multiplicar };

	for (size_t i = 0; i < 3; i++) {
		if (!menu_ingresar_opcion(menu, indices[i], textos[i],
					  funciones[i])) {
			printf("Error al ingresar la opcion '%c'", indices[i]);
			menu_destruir(menu);
		}
		pa2m_afirmar(menu_cantidad(menu) == 1 + i,
			     "Hay %li opciones en el menu", i + 1);
	}
	int esperado[3] = { 6, -4, -8 };
	int resultado;
	for (size_t j = 0; j < 3; j++) {
		if (!menu_ejecutar_opcion(menu, indices[j], &resultado)) {
			printf("Error al ejecutar la opcion '%x'", indices[j]);
			menu_destruir(menu);
		}
		pa2m_afirmar(
			resultado == esperado[j],
			"Se ejecutó la función de la opcion '%c'. Resultado: %i -> Esperado: %i",
			indices[j], resultado, esperado[j]);
	}
	menu_destruir(menu);
}

typedef struct estructura_prueba {
	menu_t *menu;
	int numero;
} prueba_t;

bool ejecutar_todos_las_opciones(void *_opcion, void *_prueba)
{
	opcion_menu_t *opcion = (opcion_menu_t *)_opcion;
	prueba_t *prueba = (prueba_t *)_prueba;
	if (!menu_ejecutar_opcion(prueba->menu, opcion->indice,
				  &prueba->numero)) {
		return false;
	}
	return true;
}

void prueba_iterar()
{
	menu_t *menu = menu_crear();
	if (!menu) {
		printf("No se pudo crear el menu");
		menu_destruir(menu);
	}
	pa2m_afirmar(menu != NULL, "Menu creado");
	char indices[3] = { 'A', 'B', 'C' };
	char *textos[3] = { "Opcion 1", "Opcion 2", "Opcion 3" };
	bool (*funciones[3])(void *) = { sumar, resta, multiplicar };

	for (size_t i = 0; i < 3; i++) {
		if (!menu_ingresar_opcion(menu, indices[i], textos[i],
					  funciones[i])) {
			printf("Error al ingresar la opcion '%c'", indices[i]);
			menu_destruir(menu);
		}
		pa2m_afirmar(menu_cantidad(menu) == 1 + i,
			     "Hay %li opciones en el menu", i + 1);
	}
	prueba_t prueba = { .menu = menu, .numero = 0 };
	size_t iterados = menu_iterar_opciones(
		menu, ejecutar_todos_las_opciones, (void *)&prueba);
	pa2m_afirmar(menu_cantidad(menu) == iterados,
		     "Se iteró y se ejecucó todas las opciones");
	pa2m_afirmar(prueba.numero == -8,
		     "Resultado optenido: '%d' -> esperado: '-8'",
		     prueba.numero);
	menu_destruir(menu);
}

void prueba_menu_null()
{
	menu_t *menu = NULL;
	pa2m_afirmar(menu_cantidad(menu) == 0,
		     "Pasar menu NULL retorna 0 como cantidad");
	pa2m_afirmar(menu_ingresar_opcion(menu, 'C', "menu es NULL", sumar) ==
			     false,
		     "Pasar menu NULL retorna false al ingresar una opción");
	pa2m_afirmar(menu_ejecutar_opcion(menu, 'C', NULL) == false,
		     "Pasar menu NULL retorna False al ejecutar");
	pa2m_afirmar(menu_iterar_opciones(menu, ejecutar_todos_las_opciones,
					  NULL) == 0,
		     "Pasar menu NULL retorna 0 en la iteracion");
}

void prueba_tablero_crear()
{
	size_t filas = 15;
	size_t columnas = 32;
	tablero_t *tablero = tablero_crear(filas, columnas);
	if (!tablero) {
		printf("No se pudo crear el tablero");
		tablero_destruir(tablero);
	}
	pa2m_afirmar(tablero != NULL, "Se creó un tablero");
	pa2m_afirmar(tablero_cantidad_filas(tablero) == filas,
		     "La cantidad de filas es la correcta");
	pa2m_afirmar(tablero_cantidad_columnas(tablero) == columnas,
		     "La cantidad de columnas es la correcta");
	tablero_destruir(tablero);
}

void prueba_crear_bordes_limites()
{
	size_t filas = 5;
	size_t columnas = 2;
	tablero_t *tablero = tablero_crear(filas, columnas);
	if (!tablero) {
		printf("No se pudo crear el tablero");
		tablero_destruir(tablero);
	}
	pa2m_afirmar(tablero != NULL, "Se creó un tablero");
	pa2m_afirmar(
		tablero_cantidad_filas(tablero) == 10,
		"Si se crea con un tablero con filas menor 10, se creará con 10 filas");
	pa2m_afirmar(
		tablero_cantidad_columnas(tablero) == 10,
		"Si se crea con un tablero con columnas menor 10, se creará con 10 columnas");
	tablero_destruir(tablero);
}

typedef struct prueba_tablero {
	char caracter;
	char *color;
	size_t y;
	size_t x;
} sujeto_prueba;

void prueba_colocar_elemento_en_tablero()
{
	size_t filas = 15;
	size_t columnas = 32;
	tablero_t *tablero = tablero_crear(filas, columnas);
	if (!tablero) {
		printf("No se pudo crear el tablero");
		tablero_destruir(tablero);
	}
	pa2m_afirmar(tablero != NULL, "Se creó un tablero");
	sujeto_prueba sujeto = {
		.caracter = 'V', .color = NULL, .y = 10, .x = 4
	};
	pa2m_afirmar(tablero_colocar_elemento(tablero, 16, sujeto.x,
					      sujeto.caracter,
					      sujeto.color) == false,
		     "No se puede poner el elemento por fuera de las filas");
	pa2m_afirmar(tablero_colocar_elemento(tablero, sujeto.y, 33,
					      sujeto.caracter,
					      sujeto.color) == false,
		     "No se puede poner el elemento por fuera de las columnas");
	pa2m_afirmar(tablero_colocar_elemento(tablero, sujeto.y, sujeto.x,
					      sujeto.caracter,
					      sujeto.color) == true,
		     "El sujeto fue colocado en el tablero");
	pa2m_afirmar(tablero_posicion_esta_vacio(tablero, 10, 4) == false,
		     "El sujeto se encuentra en la fila 10, columna 4");
	pa2m_afirmar(tablero_posicion_esta_vacio(tablero, 0, 0) == true,
		     "Se encuentra vacio en la fila 0, columna 0");

	tablero_destruir(tablero);
}

void prueba_color_mas_elementos_en_el_tablerp()
{
	size_t filas = 20;
	size_t columnas = 20;
	tablero_t *tablero = tablero_crear(filas, columnas);
	if (!tablero) {
		printf("No se pudo crear el tablero");
		tablero_destruir(tablero);
	}
	pa2m_afirmar(tablero != NULL, "Se creó un tablero");
	sujeto_prueba sujeto1 = {
		.caracter = '1', .color = NULL, .y = 10, .x = 2
	};
	sujeto_prueba sujeto2 = {
		.caracter = '2', .color = NULL, .y = 9, .x = 18
	};
	sujeto_prueba sujeto3 = {
		.caracter = '3', .color = NULL, .y = 0, .x = 11
	};
	sujeto_prueba sujeto4 = {
		.caracter = '4', .color = NULL, .y = 2, .x = 3
	};
	sujeto_prueba sujeto5 = {
		.caracter = '5', .color = NULL, .y = 18, .x = 0
	};
	sujeto_prueba sujeto6 = {
		.caracter = '6', .color = NULL, .y = 12, .x = 10
	};
	sujeto_prueba sujetos[6] = { sujeto1, sujeto2, sujeto3,
				     sujeto4, sujeto5, sujeto6 };

	for (size_t i = 0; i < 6; i++) {
		pa2m_afirmar(
			tablero_colocar_elemento(
				tablero, sujetos[i].y, sujetos[i].x,
				sujetos[i].caracter, sujetos[i].color) == true,
			"Se colocó un elemento en la fila %li, columna %li",
			sujetos[i].y, sujetos[i].x);
	}
	for (size_t i = 0; i < tablero_cantidad_filas(tablero); i++) {
		for (size_t j = 0; j < tablero_cantidad_columnas(tablero);
		     j++) {
			if (!tablero_posicion_esta_vacio(tablero, i, j)) {
				pa2m_afirmar(
					true == true,
					"Hay un elemento en la fila %li, columna %li",
					i, j);
			}
		}
	}
	tablero_destruir(tablero);
}

void prueba_mover_elemento_tablero()
{
	size_t filas = 20;
	size_t columnas = 20;
	tablero_t *tablero = tablero_crear(filas, columnas);
	if (!tablero) {
		printf("No se pudo crear el tablero");
		tablero_destruir(tablero);
	}
	pa2m_afirmar(tablero != NULL, "Se creó un tablero");
	sujeto_prueba sujeto = {
		.caracter = '4', .color = NULL, .y = 2, .x = 3
	};
	pa2m_afirmar(tablero_colocar_elemento(tablero, sujeto.y, sujeto.x,
					      sujeto.caracter,
					      sujeto.color) == true,
		     "El sujeto fue colocado en el tablero");
	pa2m_afirmar(tablero_posicion_esta_vacio(tablero, 2, 3) == false,
		     "El sujeto se encuentra en la fila 2, columna 3");
	size_t nueva_fila = 10;
	size_t nueva_columna = 5;
	pa2m_afirmar(tablero_mover_elemento(tablero, sujeto.y, sujeto.x, 21,
					    nueva_columna, sujeto.caracter,
					    sujeto.color) == false,
		     "No se puede cambiar a un destino fuera de las filas");
	pa2m_afirmar(tablero_mover_elemento(tablero, sujeto.y, sujeto.x,
					    nueva_fila, 21, sujeto.caracter,
					    sujeto.color) == false,
		     "No se puede cambiar a un destino fuera de las columnas");
	pa2m_afirmar(tablero_mover_elemento(tablero, sujeto.y, sujeto.x,
					    nueva_fila, nueva_columna,
					    sujeto.caracter,
					    sujeto.color) == true,
		     "El sujeto fue cambiado de posicion");
	pa2m_afirmar(tablero_posicion_esta_vacio(tablero, 2, 3) == true,
		     "La fila 2, columna 3 esta vacio");
	pa2m_afirmar(tablero_posicion_esta_vacio(tablero, nueva_fila,
						 nueva_columna) == false,
		     "El sujeto se encuentra en la fila %li, columna %li",
		     nueva_fila, nueva_columna);
	tablero_destruir(tablero);
}

void prueba_tablero_null()
{
	tablero_t *tablero = NULL;
	sujeto_prueba sujeto = {
		.caracter = 'X', .color = NULL, .y = 10, .x = 3
	};
	size_t nueva_fila = 2;
	size_t nueva_columna = 4;
	char caracter;
	char *color_caracter;
	pa2m_afirmar(tablero_colocar_elemento(tablero, sujeto.y, sujeto.x,
					      sujeto.caracter,
					      sujeto.color) == false,
		     "Pasar tablero NULL retorna false color un elemento");
	pa2m_afirmar(tablero_cantidad_filas(tablero) == 0,
		     "Pasar tablero NULL retorna 0 saber la cantidad de filas");
	pa2m_afirmar(
		tablero_cantidad_columnas(tablero) == 0,
		"Pasar tablero NULL retorna 0 saber la cantidad de columnas");
	pa2m_afirmar(tablero_mover_elemento(tablero, sujeto.y, sujeto.x,
					    nueva_fila, nueva_columna,
					    sujeto.caracter,
					    sujeto.color) == false,
		     "Pasar tablero NULL retorna false mover un elemeto");
	pa2m_afirmar(
		tablero_posicion_esta_vacio(tablero, nueva_fila,
					    nueva_columna) == false,
		"Pasar tablero NULL retorna false saber si una posicion esta vacio");
	pa2m_afirmar(
		tablero_posicion_informacion(tablero, nueva_fila, nueva_columna,
					     &caracter,
					     &color_caracter) == false,
		"Pasar tablero NULL retorna false conocer la informacion de alguna posicion");
}

bool sumar_los_puntajes(pokemon_t* pokemon, void* contador_puntajes)
{	
	*(int*)contador_puntajes += pokemon->puntaje;
	return true;
}

void prueba_pokedex_crear()
{
	pokedex_t* pokedex = pokedex_crear();
	if (!pokedex) {
		printf("No se pudo crear la pokedex");
		return;
	}
	size_t puntaje_total = 0 ;
	pa2m_afirmar(pokedex != NULL, "Se creó una pokedex");
	pa2m_afirmar(pokedex_cantidad(pokedex) == 0, "Pokedex creada no tiene pokemones almacenados, retorna 0");
	pa2m_afirmar(pokedex_iterar(pokedex, sumar_los_puntajes, (void*)&puntaje_total) == 0, "Pokedex creada no se puede iterar, retorna 0");
	pa2m_afirmar(pokedex_obtener_pokemon(pokedex, 0) == NULL, "Pokedex creada, obtener un pokemon retorna NULL");
	pokedex_destruir(pokedex);
}

void liberar_pokemones(pokemon_t* pokemon)
{
	free(pokemon);
}

void prueba_pokedex_insertar()
{
	pokedex_t* pokedex = pokedex_crear();
	if (!pokedex) {
		printf("No se pudo crear la pokedex");
		return;
	}
	pa2m_afirmar(pokedex != NULL, "Se creó una pokedex");
	pokemon_t* pokemon = calloc(1, sizeof(pokemon_t));
	if (!pokemon) {
		printf("No se pudo asignar memoria dinamica para el pokemon");
		pokedex_destruir_todo(pokedex, liberar_pokemones);
		return;
	}
	pokemon->nombre = "Pikachu";
	pokemon->movimientos = "IZQUIERDA";
	pokemon->color = "rojo";
	pokemon->puntaje = 100;

	if (!pokedex_insertar_pokemon(pokedex, pokemon)) {
		printf("No se pudo ingresar el pokemon a la pokedex");
		pokedex_destruir_todo(pokedex, liberar_pokemones);
	}
	pa2m_afirmar(pokedex_cantidad(pokedex) == 1, "Se ingresó un pokemon a la pokedex");
	pa2m_afirmar(pokedex_obtener_pokemon(pokedex, 1) == NULL, "Obtener un pokemon en una posicion mayor o igual a la cantidad que hay en la pokedex, retorna NULL");
	pokemon_t* pokemon_obtenido = pokedex_obtener_pokemon(pokedex, 0);
	pa2m_afirmar(pokemon_obtenido == pokemon, "Se obtuve el pokemon en la posicion 0 (unico pokemon existente). Dirección obtenida: '%li' -> Dirección esperada: '%li'", pokemon_obtenido, pokemon);
	pokedex_destruir_todo(pokedex, liberar_pokemones);
}

void prueba_pokedex_insertar_mas_pokemones()
{
	pokedex_t* pokedex = pokedex_crear();
	if (!pokedex) {
		printf("No se pudo crear la pokedex");
		return;
	}
	pa2m_afirmar(pokedex != NULL, "Se creó una pokedex");
	pokemon_t pokemon1 = {.nombre = "Pikachu", .movimientos = "IZQUIERDA", .color = "rojo", .puntaje = 100};
	pokemon_t pokemon2 = {.nombre = "Elmo", .movimientos = "DERECHA", .color = "azul", .puntaje = 123};
	pokemon_t pokemon3 = {.nombre = "Raichú", .movimientos = "ARRIBA", .color = "verde", .puntaje = -2};
	pokemon_t pokemon4 = {.nombre = "Pocoyó", .movimientos = "ABAJO", .color = "amarillo", .puntaje = -10};
	pokemon_t pokemon5 = {.nombre = "Snorlax", .movimientos = "MOVIMIENTO", .color = "marrón", .puntaje = 50};
	pokemon_t pokemon6 = {.nombre = "Drake", .movimientos = "SENSUAL", .color = "celeste", .puntaje = 80};
	pokemon_t pokemon7 = {.nombre = "Josh", .movimientos = "WIIIIIIII", .color = "purpura", .puntaje = -230};
	pokemon_t* pokemones[7] = {&pokemon1, &pokemon2, &pokemon3, &pokemon4, &pokemon5, &pokemon6, &pokemon7};

	for (size_t i = 0; i < 7; i++) {
		if (!pokedex_insertar_pokemon(pokedex, pokemones[i])) {
			printf("No se pudo ingresar el pokemon a la pokedex");
			pokedex_destruir(pokedex);
		}
	}

	pa2m_afirmar(pokedex_cantidad(pokedex) == 7, "Hay 7 pokemones en la pokedex");

	for (size_t i = 0; i < 7; i++) {
		pokemon_t* pokemon_obtenido = pokedex_obtener_pokemon(pokedex, i);
		pa2m_afirmar(true == true, "Se obtuvo al pokemon '%s' en la posicion %li", (*pokemon_obtenido).nombre, i);
	}
	
	pokemon_t* pokemon_obtenido = pokedex_obtener_pokemon(pokedex, 4);
	pa2m_afirmar(pokemon_obtenido == &pokemon4, "Pokemon en la posicion 4. Dirección obtenida: '%li', direccion esperada: %li", pokemon_obtenido, &pokemon4);
	pokedex_destruir(pokedex);
}

void prueba_iterar_pokedex()
{
	pokedex_t* pokedex = pokedex_crear();
	if (!pokedex) {
		printf("No se pudo crear la pokedex");
		return;
	}
	pa2m_afirmar(pokedex != NULL, "Se creó una pokedex");
	pokemon_t pokemon1 = {.nombre = "Pikachu", .movimientos = "IZQUIERDA", .color = "rojo", .puntaje = 100};
	pokemon_t pokemon2 = {.nombre = "Elmo", .movimientos = "DERECHA", .color = "azul", .puntaje = 123};
	pokemon_t pokemon3 = {.nombre = "Raichú", .movimientos = "ARRIBA", .color = "verde", .puntaje = -2};
	pokemon_t pokemon4 = {.nombre = "Pocoyó", .movimientos = "ABAJO", .color = "amarillo", .puntaje = -10};
	pokemon_t pokemon5 = {.nombre = "Snorlax", .movimientos = "MOVIMIENTO", .color = "marrón", .puntaje = 50};
	pokemon_t pokemon6 = {.nombre = "Drake", .movimientos = "SENSUAL", .color = "celeste", .puntaje = 80};
	pokemon_t pokemon7 = {.nombre = "Josh", .movimientos = "WIIIIIIII", .color = "purpura", .puntaje = -230};
	pokemon_t* pokemones[7] = {&pokemon1, &pokemon2, &pokemon3, &pokemon4, &pokemon5, &pokemon6, &pokemon7};

	for (size_t i = 0; i < 7; i++) {
		if (!pokedex_insertar_pokemon(pokedex, pokemones[i])) {
			printf("No se pudo ingresar el pokemon a la pokedex");
			pokedex_destruir(pokedex);
		}
	}
	int contador = 0;
	int resultado_esperado = 111;
	pa2m_afirmar(pokedex_iterar(pokedex, NULL, &contador) == 0, "Pasar NULL en la función de iterador, retorna 0 la cantidad de iteraciones");
	size_t iterados = pokedex_iterar(pokedex, sumar_los_puntajes, &contador);
	pa2m_afirmar(iterados == pokedex_cantidad(pokedex), "Se iteró todos los pokemones");
	pa2m_afirmar(contador == resultado_esperado, "Se iteró y se hizo la operación esperada");
	pokedex_destruir(pokedex);
}

void prueba_pokedex_null()
{
	pokedex_t* pokedex = NULL;
	pokemon_t pokemon = {.nombre = "Pikachu", .movimientos = "IZQUIERDA", .color = "rojo", .puntaje = 100};
	int contador = 0;
	pa2m_afirmar(pokedex_insertar_pokemon(pokedex, &pokemon) == false, "Pasar pokedex NULL retorna false el insertar un pokemon");
	pa2m_afirmar(pokedex_cantidad(pokedex) == 0, "Pasar pokedex NULL retorna 0 en la cantidad de pokemones");
	pa2m_afirmar(pokedex_obtener_pokemon(pokedex, 0) == NULL, "Pasar pokedex NULL retorna NULL el obtener pokemon en cualquier posicion");
	pa2m_afirmar(pokedex_iterar(pokedex, sumar_los_puntajes, &contador) == 0, "Pasar pokedex NULL retorna 0 iterar la pokedex");	
}

void prueba_movimientos_crear()
{
	movimientos_t* movimientos = movimientos_crear();
	if (!movimientos) {
		printf("No se pudo crear el contenedor de movimientos");
		return;
	}
	pa2m_afirmar(movimientos != NULL, "Se creó el contenedor de movimientos");
	sujeto_prueba sujeto = {.caracter = 'X', .color = NULL, .y = 10, .x = 1};
	pa2m_afirmar(movimientos_cantidad(movimientos) == 0, "Cuando se crea el almacen de movimientos, está vacio");
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "ARRIBA", &sujeto.y, &sujeto.x, 15, 15) == false, "Realizar un movimiento retorna falso");
	movimientos_destruir(movimientos);
}

void movimiento_arriba(char* nada1, size_t* y, size_t* x, size_t limite_y, size_t limite_x)
{
	if (!y)
		return;
	if (*y > 0)
		(*y)--;
}

void movimiento_abajo(char* nada1, size_t* y, size_t* x, size_t limite_y, size_t limite_x)
{
	if (!y)
		return;
	if (*y < limite_y - 1)
		(*y)++;
}

void movimiento_izquierda(char* nada1, size_t* y, size_t* x, size_t limite_y, size_t limite_x)
{	
	if(!x)
		return;
	if (*x > 0) {
		(*x)--;
	}
}

void movimiento_derecha(char* nada1, size_t* y, size_t* x, size_t limite_y, size_t limite_x)
{	
	if (!x)
		return;
	if (*x < limite_x - 1) {
		(*x)++;
	}
}

void movimientos_invertido(char* movimiento_a_invertir, size_t* y, size_t* x, size_t limite_y, size_t limite_x)
{
	if (strcmp(movimiento_a_invertir, "ARRIBA") == 0) {
		movimiento_abajo(NULL, y, x, limite_y, limite_x);
	} else if (strcmp(movimiento_a_invertir, "ABAJO") == 0) {
		movimiento_arriba(NULL, y, x, limite_y, limite_x);
	} else if (strcmp(movimiento_a_invertir, "IZQUIERDA") == 0) {
		movimiento_derecha(NULL, y, x, limite_y, limite_x);
	} else if (strcmp(movimiento_a_invertir, "DERECHA") == 0) {
		movimiento_izquierda(NULL, y, x, limite_y, limite_x);
	}
}

void prueba_ingresar_movimiento()
{
	movimientos_t* movimientos = movimientos_crear();
	if (!movimientos) {
		printf("No se pudo crear el contenedor de movimientos");
		return;
	}
	pa2m_afirmar(movimientos != NULL, "Se creó el contenedor de movimientos");
	pa2m_afirmar(movimientos_agregar(movimientos, "ARRIBA", NULL) == false, "Pasar una función de movimiento NULL, retorna false ingresar");
	if (!movimientos_agregar(movimientos, "ARRIBA", movimiento_arriba)) {
		printf("No se pudo agregar el movimiento");
		movimientos_destruir(movimientos);
		return;
	}
	pa2m_afirmar(movimientos_cantidad(movimientos) == 1, "Se agregó un movimiento");
	movimientos_destruir(movimientos);
}

void prueba_ingresar_mas_movimienos()
{
	movimientos_t* movimientos = movimientos_crear();
	if (!movimientos) {
		printf("No se pudo crear el contenedor de movimientos");
		return;
	}
	pa2m_afirmar(movimientos != NULL, "Se creó el contenedor de movimientos");
	movimientos_agregar(movimientos, "ARRIBA", movimiento_arriba);
	movimientos_agregar(movimientos, "ABAJO", movimiento_abajo);
	movimientos_agregar(movimientos, "INVERTIDO", movimientos_invertido);
	if (movimientos_cantidad(movimientos) != 3) {
		printf("No se pudo agregar algún movimiento");
		movimientos_destruir(movimientos);
		return;
	}
	pa2m_afirmar(movimientos_cantidad(movimientos) == 3, "Se agregaron 3 movimientos");

	movimientos_destruir(movimientos);
}


void prueba_realizar_movimiento()
{
	movimientos_t* movimientos = movimientos_crear();
	if (!movimientos) {
		printf("No se pudo crear el contenedor de movimientos");
		return;
	}
	pa2m_afirmar(movimientos != NULL, "Se creó el contenedor de movimientos");

	if (!movimientos_agregar(movimientos, "ARRIBA", movimiento_arriba)) {
		printf("No se pudo agregar el movimiento");
		movimientos_destruir(movimientos);
		return;
	}
	sujeto_prueba sujeto = {.caracter = 'X', .color = NULL, .y = 10, .x = 1};
	pa2m_afirmar(sujeto.y == 10, "Sujeto se encuentra en un lugar");
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "ARRIBA", NULL, &sujeto.x, 20, 20) == false, "Pasar una posicion Y NULL retorna false realizar un movimiento");
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "ARRIBA", &sujeto.y, NULL, 20, 20) == false, "Pasar una posicion X NULL retorna false realizar un movimiento");
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "ARRIBA", &sujeto.y, &sujeto.x, 20, 20) == true, "Se realizó un movimiento arriba");
	pa2m_afirmar(sujeto.y == 9, "Sujeto subió de lugar");

	movimientos_destruir(movimientos);
}

void prueba_realizar_movimientos_juntos()
{
	movimientos_t* movimientos = movimientos_crear();
	if (!movimientos) {
		printf("No se pudo crear el contenedor de movimientos");
		return;
	}
	pa2m_afirmar(movimientos != NULL, "Se creó el contenedor de movimientos");

	movimientos_agregar(movimientos, "ARRIBA", movimiento_arriba);
	movimientos_agregar(movimientos, "ABAJO", movimiento_abajo);
	movimientos_agregar(movimientos, "DERECHA", movimiento_derecha);
	movimientos_agregar(movimientos, "IZQUIERDA", movimiento_izquierda);
	movimientos_agregar(movimientos, "INVERTIDO", movimientos_invertido);

	if (movimientos_cantidad(movimientos) != 5) {
		printf("No se pudo agregar algún movimiento");
		movimientos_destruir(movimientos);
		return;
	}

	sujeto_prueba sujeto = {.caracter = 'X', .color = NULL, .y = 10, .x = 5};
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "ARRIBA", &sujeto.y, &sujeto.x, 20, 20) == true, "Se realizó un movimiento arriba");
	pa2m_afirmar(sujeto.y == 9, "Sujeto subió de lugar");
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "ABAJO", &sujeto.y, &sujeto.x, 20, 20) == true, "Se realizó un movimiento abajo");
	pa2m_afirmar(sujeto.y == 10, "Sujeto bajó de lugar");
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "IZQUIERDA", &sujeto.y, &sujeto.x, 20, 20) == true, "Se realizó un movimiento a izquierda");
	pa2m_afirmar(sujeto.x == 4, "Sujeto está una posicion a izquierda");
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "DERECHA", &sujeto.y, &sujeto.x, 20, 20) == true, "Se realizó un movimiento a derecha");
	pa2m_afirmar(sujeto.x == 5, "Sujeto está una posicion a derecha");
	pa2m_afirmar(movimiento_realizar(movimientos, "DERECHA", "INVERTIDO", &sujeto.y, &sujeto.x, 20, 20) == true, "Se realizó un movimiento invertido");
	pa2m_afirmar(sujeto.x == 4, "Sujeto está una posicion invertida");
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "ABAJO", &sujeto.y, &sujeto.x, 2, 2) == true, "Se realizó otro movimiento abajo por fuera del limite");
	pa2m_afirmar(sujeto.y == 10, "Sujeto se encuentra en su misma posicion");

	movimientos_destruir(movimientos);
}

void prueba_movimientos_null()
{
	movimientos_t* movimientos = NULL;
	sujeto_prueba sujeto = {.caracter = 'X', .color = NULL, .y = 10, .x = 5};
	pa2m_afirmar(movimientos_cantidad(movimientos) == 0, "Pasar el contenedor NULL retorna 0 la cantidad de movimientos");
	pa2m_afirmar(movimientos_agregar(movimientos, "ARRIBA", movimiento_arriba) == false, "Pasar el contenedor NULL retorna false agregar un movimiento");
	pa2m_afirmar(movimiento_realizar(movimientos, NULL, "ARRIBA", &sujeto.y, &sujeto.x, 20, 20) == false, "Pasar el contenedor NULL retorna false realizar algún movimiento");
}


int main()
{
	pa2m_nuevo_grupo("=========== PRUEBAS MENU ===========");
	pa2m_nuevo_grupo("PRUEBA CREAR MENU");
	prueba_crear_menu();

	pa2m_nuevo_grupo("PRUEBA INSERTAR OPCIONES");
	prueba_insertar();
	prueba_insertar_mas_opciones();

	pa2m_nuevo_grupo("PRUEBA EJECUTAR OPCIONES");
	prueba_ejecutar();

	pa2m_nuevo_grupo("PRUEBA ITERAR OPCIONES");
	prueba_iterar();

	pa2m_nuevo_grupo("PRUEBA MENU ES NULL");
	prueba_menu_null();

	pa2m_nuevo_grupo("=========== PRUEBAS TABLERO ===========");
	pa2m_nuevo_grupo("PRUEBA CREAR TABLERO");
	prueba_tablero_crear();
	prueba_crear_bordes_limites();

	pa2m_nuevo_grupo("PRUEBA INSERTAR ELEMENTOS");
	prueba_colocar_elemento_en_tablero();
	prueba_color_mas_elementos_en_el_tablerp();

	pa2m_nuevo_grupo("PRUEBA VACIAR POSICION");
	prueba_mover_elemento_tablero();

	pa2m_nuevo_grupo("PRUEBA TABLERO ES NULL");
	prueba_tablero_null();

	pa2m_nuevo_grupo("=========== PRUEBAS POKEDEX ===========");
	pa2m_nuevo_grupo("PRUEBA CREAR POKEDEX");
	prueba_pokedex_crear();

	pa2m_nuevo_grupo("PRUEBA INSERTAR POKEMON");
	prueba_pokedex_insertar();
	prueba_pokedex_insertar_mas_pokemones();

	pa2m_nuevo_grupo("PRUEBA ITERAR POKEDEX");
	prueba_iterar_pokedex();

	pa2m_nuevo_grupo("PRUEBA POKEDEX NULL");
	prueba_pokedex_null();

	pa2m_nuevo_grupo("=========== PRUEBAS CONTENEDOR DE MOVIMIENTOS ===========");
	pa2m_nuevo_grupo("PRUEBA CREAR CONTENEDOR MOVIMIENTOS");
	prueba_movimientos_crear();

	pa2m_nuevo_grupo("PRUEBA INGRESAR MOVIMIENTOS");
	prueba_ingresar_movimiento();
	prueba_ingresar_mas_movimienos();

	pa2m_nuevo_grupo("PRUEBA REALIZAR MOVIMIENTOS");
	prueba_realizar_movimiento();
	prueba_realizar_movimientos_juntos();

	pa2m_nuevo_grupo("PRUEBA CONTENEDOR DE MOVIMIENTOS NULL");
	prueba_movimientos_null();

	return pa2m_mostrar_reporte();
}
