#include "mostrar_informacion.h"
#include "../extra/ansi.h"
#include "estructuras_jugabilidad.h"
#include "cola.h"
#include "verificaciones.h"
#include <stdio.h>

bool imprimir_opciones(void *_opcion, void *nada)
{
	opcion_menu_t *opcion = (opcion_menu_t *)_opcion;
	printf("   (" ANSI_COLOR_RED "%c" ANSI_COLOR_RESET ") " ANSI_COLOR_BLUE
	       "%s" ANSI_COLOR_RESET "\n",
	       opcion->indice, opcion->texto);
	return true;
}

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

void mostrar_informacion_por_pantalla(size_t *tiempo, size_t* semilla, size_t tiempo_maximo, Pila *capturados,
				      usuario *usuario, tablero_t *tablero)
{
	(*tiempo)++;
	if ((tiempo_maximo - (*tiempo/5)) > 10) {
		printf("⏳%s%li%s ", ANSI_COLOR_BLUE, tiempo_maximo - (*tiempo/5), ANSI_COLOR_RESET);
	} else {
		printf("⌛%s%li%s ", ANSI_COLOR_RED, tiempo_maximo - (*tiempo/5), ANSI_COLOR_RESET);	
	}
	printf("👣%s%li%s 💲%s%li%s %s(x%li)%s\n", ANSI_COLOR_BLUE, usuario->cantidad_pasos, ANSI_COLOR_RESET, 
	ANSI_COLOR_GREEN, usuario->puntaje, ANSI_COLOR_RESET, ANSI_COLOR_YELLOW, usuario->multiplicador, ANSI_COLOR_RESET);
	dibujar_tablero(tablero);
	printf("🌱 %s%li%s\n", ANSI_COLOR_CYAN, *semilla, ANSI_COLOR_RESET);
	if (!pila_esta_vacía(capturados)) {
		pokemon_seleccionado *pokemon =
			(pokemon_seleccionado *)pila_tope(capturados);
		printf("Último Pokemon capturado: %s%s%s\n", pokemon->color,
		       pokemon->nombre, ANSI_COLOR_RESET);
	}
	printf("Cantidad capturados: %li\n", pila_cantidad(capturados));
}

bool mostrar_primer_grupo_maximo(void *_grupo_formado, void *cantidad_maxima)
{
	Cola *grupo_formado = (Cola *)_grupo_formado;
	if (cola_cantidad(grupo_formado) == *(size_t *)cantidad_maxima) {
		while (!cola_esta_vacía(grupo_formado)) {
			pokemon_seleccionado *pokemon =
				(pokemon_seleccionado *)cola_desencolar(
					grupo_formado);
			printf("%s%c%s", pokemon->color, pokemon->caracter,
			       ANSI_COLOR_RESET);
			destruir_pokemones_seleccionados((void*)pokemon);
		}
		printf("\n");
		return false;
	}
	return true;
}

void mostrar_resultados(Lista* grupos_formados, size_t maximo_grupo_formado, size_t multiplicador_maximo, size_t puntaje_personaje)
{
	printf("Maximo grupo formado: ");
	if (maximo_grupo_formado >= 2) {
		lista_iterar_elementos(grupos_formados, mostrar_primer_grupo_maximo, (void*)&maximo_grupo_formado);
	} else {
		printf("\nNigun grupo formado. Necesitas un grupo de 2 como mínimo\n");
	}
	printf("Multiplicador maximo: %li\n", multiplicador_maximo);
	printf("Tu puntaje final fue de: %li\n", puntaje_personaje);
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