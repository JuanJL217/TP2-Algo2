#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "extra/engine.h"
#include "extra/ansi.h"
#include "src/hash.h"
#include "src/menu.h"
#include "src/pokedex.h"
#include "src/logica.h"
#include "src/verificaciones.h"

const size_t CANTIDAD_OPCIONES_MENU_PRINCIPAL = 6;
const size_t CANTIDAD_OBJETIVOS_INICIAL = 7;
const size_t FILAS_INICIAL = 15;
const size_t COLUMNAS_INICIAL = 32;
const size_t TIEMPO_INICIAL = 60;

bool imprimir_opciones(void *_opcion, void *nada)
{
	opcion_menu_t *opcion = (opcion_menu_t *)_opcion;
	printf("   (" ANSI_COLOR_RED "%c" ANSI_COLOR_RESET ") " ANSI_COLOR_BLUE
	       "%s" ANSI_COLOR_RESET "\n",
	       opcion->indice, opcion->texto);
	return true;
}

bool agregar_opciones_al_menu(menu_t *menu)
{	
	menu_ingresar_opcion(menu, 'P', "Mostrar pokedex", mostrar_pokemones);
	menu_ingresar_opcion(menu, 'J', "Iniciar juego", jugar_partida);
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
	booleanos banderas = { .menu_seguir = true,
				   .opciones_seguir = false,
			       .pokedex = NULL,
			       .colores = NULL,
				   .semilla = NULL,
				   .cantidad_objetivos = CANTIDAD_OBJETIVOS_INICIAL,
				   .cantidad_filas = FILAS_INICIAL,
				   .cantidad_columas = COLUMNAS_INICIAL,
				   .tiempo_maximo = TIEMPO_INICIAL};
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
