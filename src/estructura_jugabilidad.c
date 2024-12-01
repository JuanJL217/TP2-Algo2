#include "estructuras_jugabilidad.h"
#include "pokedex.h"

void destruir_pokemones_seleccionados(void *_pokemon)
{	
	pokemon_seleccionado* pokemon = (pokemon_seleccionado *)_pokemon;
	free(pokemon->nombre);
	free(pokemon->movimientos);
	free(pokemon);
}