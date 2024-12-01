#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "extra/engine.h"
#include "extra/ansi.h"
#include "src/hash.h"
#include "src/menu.h"
#include "src/pokedex.h"
#include "src/logica_del_juego.h"
#include "src/verificaciones.h"
#include "src/mostrar_informacion.h"

const size_t CANTIDAD_OPCIONES_MENU_PRINCIPAL = 6;
const size_t CANTIDAD_OPCIONES_EXTRAS = 4;
const size_t CANTIDAD_OBJETIVOS_INICIAL = 7;
const size_t FILAS_INICIAL = 15;
const size_t COLUMNAS_INICIAL = 32;
const size_t TIEMPO_INICIAL = 60;

booleanos crear_banderas()
{
	booleanos banderas = { .menu_seguir = true,
				   .opciones_seguir = false,
			       .pokedex = NULL,
			       .colores = NULL,
				   .semilla = NULL,
				   .cantidad_objetivos = CANTIDAD_OBJETIVOS_INICIAL,
				   .cantidad_filas = FILAS_INICIAL,
				   .cantidad_columas = COLUMNAS_INICIAL,
				   .tiempo_maximo = TIEMPO_INICIAL};
	return banderas;
}


// ---------------- OPCIONES EXTRAS --------------------

bool agregar_opciones_extras(menu_t* opciones_extras)
{
	menu_ingresar_opcion(opciones_extras, 'M', "Modificar cantidad de Pokemones en Juego", modificar_cantidad_pokemones);
	menu_ingresar_opcion(opciones_extras, 'T', "Modifcar Tiempo", modificar_tiempo);
	menu_ingresar_opcion(opciones_extras, 'F', "Modificar cantidad Filas y Columnas", modificar_dimensiones_tablero);
	menu_ingresar_opcion(opciones_extras, 'V', "Volver al menu", volver_al_menu);

	if (menu_cantidad(opciones_extras) != CANTIDAD_OPCIONES_EXTRAS) {
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
		menu_iterar_opciones(opciones_extras, imprimir_opciones, NULL);
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

// ---------------- OPCIONES PRINCIPALES --------------------

bool agregar_opciones_al_menu(menu_t *menu)
{	
	menu_ingresar_opcion(menu, 'P', "Mostrar pokedex", mostrar_pokemones);
	menu_ingresar_opcion(menu, 'J', "Iniciar juego", jugar_con_semilla_random);
	menu_ingresar_opcion(menu, 'S', "Iniciar juego con semilla",
			     jugar_con_semilla);
	menu_ingresar_opcion(menu, 'O', "Otras opciones", opciones_jugar);
	menu_ingresar_opcion(menu, 'R', "Restablecer valores predeterminados", restablecer_valores);
	menu_ingresar_opcion(menu, 'Q', "Salir", salir_del_menu);

	if (menu_cantidad(menu) != CANTIDAD_OPCIONES_MENU_PRINCIPAL) {
		menu_destruir(menu);
		return false;
	}
	return true;
}

void destruir_pokemones(pokemon_t* pokemon)
{
	free(pokemon->nombre);
	free(pokemon->color);
	free(pokemon->movimientos);
	free(pokemon);
}

void destruir_datos(pokedex_t *pokedex, hash_t *colores)
{
	if (pokedex) {
		pokedex_destruir_todo(pokedex, destruir_pokemones);
	}
	if (colores) {
		hash_destruir(colores);
	}
}

int main()
{
	menu_t *menu = menu_crear();
	if (!menu) {
		printf("No se pudo crear el menú");
		return -1;
	}

	if (!agregar_opciones_al_menu(menu)) {
		printf("Fallo en la creación de las opciones del menu");
		return -2;
	}
	booleanos banderas = crear_banderas();
	char texto[MAX_CARACTERES];
	while (banderas.menu_seguir) {
		menu_iterar_opciones(menu, imprimir_opciones, NULL);
		if (es_caracter(texto)) {
			char mayuscula = (char)toupper((unsigned char)texto[0]);
			menu_ejecutar_opcion(menu, mayuscula,
					     (void *)&banderas);
		}
	}
	menu_destruir(menu);
	if (banderas.pokedex || banderas.colores) {
		destruir_datos(banderas.pokedex, banderas.colores);
	}
	return 0;
}
