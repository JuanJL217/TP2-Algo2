#include "verificaciones.h"
#include <ctype.h>
#include <string.h>

bool es_caracter(char *texto)
{
	if (fgets(texto, sizeof(texto), stdin) == NULL)
		return false;
	texto[strcspn(texto, "\n")] = '\0';
	return strlen(texto) == 1;
}

bool es_numero(char *texto) {
    if (fgets(texto, MAX_CARACTERES, stdin) == NULL)
        return false;
    texto[strcspn(texto, "\n")] = '\0';
    if (strlen(texto) == 0)
        return false;
    for (size_t i = 0; texto[i] != '\0'; i++) {
        if (!isdigit((unsigned char)texto[i]))
            return false;
    }
    return true;
}

char caracter_mayuscula(char* texto)
{
    return (char)toupper((unsigned char)texto[0]);
}
