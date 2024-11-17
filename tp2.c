#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "extra/engine.h"
#include "extra/ansi.h"
#include "src/menu.h"
#include "src/pokedex.h"
#include "src/colores.h"
#include "src/logica.h"

const size_t CANTIDAD_OPCIONES = 4;

bool imprimir_opciones(void *_opcion, void *nada)
{
	opcion_menu_t *opcion = (opcion_menu_t *)_opcion;
	printf("   (" ANSI_COLOR_RED "%c" ANSI_COLOR_RESET ") " ANSI_COLOR_BLUE
	       "%s" ANSI_COLOR_RESET "\n",
	       opcion->indice, opcion->texto);
	return true;
}

bool es_caracter(char *texto)
{
	if (fgets(texto, sizeof(texto), stdin) == NULL)
		return false;
	texto[strcspn(texto, "\n")] = '\0';
	return strlen(texto) == 1;
}

bool agregar_opciones_al_menu(menu_t *menu)
{	
	menu_ingresar_opcion(menu, 'P', "Mostrar pokedex", mostrar_pokemones);
	menu_ingresar_opcion(menu, 'J', "Iniciar juego", jugar_partida);
	menu_ingresar_opcion(menu, 'S', "Iniciar juego con semilla",
			     jugar_con_semilla);
	menu_ingresar_opcion(menu, 'Q', "Salir", salir_del_menu);

	if (menu_cantidad(menu) != CANTIDAD_OPCIONES) {
		menu_destruir(menu);
		return false;
	}
	return true;
}

void destruir_datos(pokedex_t *pokedex, colores_t *colores)
{
	if (pokedex) {
		pokedex_destruir(pokedex);
	}
	if (colores) {
		colores_destruir(colores);
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
		printf("Fallo en la creación de las opciones");
		return -2;
	}
	booleanos banderas = { .menu_seguir = true,
			       .pokedex = NULL,
			       .colores = NULL,
				   .semilla = NULL};
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
