#include "pa2m.h"
#include "src/menu.h"
#include "src/lista.h"

void prueba_crear_menu()
{
	menu_t* menu = menu_crear();
	if (!menu) {
		printf("No se pudo crear el menu");
		menu_destruir(menu);
	}
	pa2m_afirmar(menu != NULL, "Menu creado");
	pa2m_afirmar(menu_cantidad(menu) == 0, "Hay 0 opciones en el menu");
	menu_destruir(menu);
}

bool sumar(void* x)
{
	int* a = calloc(1, sizeof(int));
	if (!a)
		return false;
	int b = 4;
	(*a) = 2;
	(*a) += b;
	*(int*)x = (*a);
	free(a);
	return true;
}

bool resta(void* y)
{
	int* a = calloc(1, sizeof(int));
	if (!a)
		return false;
	(*a) = 10;
	*(int*)y -= *a;
	free(a);
	return true;
}

bool multiplicar(void* z)
{
	int* a = calloc(1, sizeof(int));
	if (!a)
		return false;
	(*a) = 2;
	*(int*)z *= *a;
	free(a);
	return true;
}

void prueba_insertar()
{
	menu_t* menu = menu_crear();
	if (!menu) {
		printf("No se pudo crear el menu");
		menu_destruir(menu);
	}
	menu_ingresar_opcion(menu, 'A', "Esto es una prueba", sumar);
	pa2m_afirmar(menu_cantidad(menu) == 1, "Hay una opción en el menu");
	int resultado;
	if (!menu_ejecutar_opcion(menu, 'A', (void*)&resultado)) {
		printf("Error al ejecutar la opción 'A");
		menu_destruir(menu);
	}
	pa2m_afirmar(resultado == 6, "Se ejecutó la función de suma. Resultado: %d -> Esperado: 6", resultado);
	menu_destruir(menu);
}

void prueba_insertar_mas_opciones()
{
	menu_t* menu = menu_crear();
	if (!menu) {
		printf("No se pudo crear el menu");
		menu_destruir(menu);
	}
	char indices[3] = {'A', 'B', 'C'};
	char* textos[3] = {"Opcion 1", "Opcion 2", "Opcion 3"};
	bool (*funciones[3])(void*) = {sumar, resta, multiplicar};

	for (size_t i = 0; i < 3; i++) {
		if (!menu_ingresar_opcion(menu, indices[i], textos[i], funciones[i])) {
			printf("Error al ingresar la opcion '%c'", indices[i]);
			menu_destruir(menu);
		}
		pa2m_afirmar(menu_cantidad(menu) == 1 + i, "Hay %li opciones en el menu", i+1);
	}
	int esperado[3] = {6, -4, -8};
	int resultado;
	for (size_t j = 0; j < 3; j++) {
		if (!menu_ejecutar_opcion(menu, indices[j], &resultado)) {
			printf("Error al ejecutar la opcion '%x'", indices[j]);
			menu_destruir(menu);
		}
		pa2m_afirmar(resultado == esperado[j], "Se ejecutó la función de la opciones '%c'. Resultado: %i -> Esperado: %i", indices[j], resultado, esperado[j]);
	}
	menu_destruir(menu);
}


int main()
{
	pa2m_nuevo_grupo("PRUEBA CREAR MENU");
	prueba_crear_menu();

	pa2m_nuevo_grupo("PRUEBA INSERTAR OPCIONES");
	prueba_insertar();
	prueba_insertar_mas_opciones();

	
	return pa2m_mostrar_reporte();
}
