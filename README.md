<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP 2

## Repositorio de (Nombre Apellido) - (Padrón) - (Mail)

- Para compilar:

```bash
línea de compilación
```

- Para ejecutar:

```bash
línea de ejecución
```

- Para ejecutar con valgrind:
```bash
línea con valgrind
```
---
##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluir **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

### Por ejemplo:

El programa funciona abriendo el archivo pasado como parámetro y leyendolo línea por línea. Por cada línea crea un registro e intenta agregarlo al vector. La función de lectura intenta leer todo el archivo o hasta encontrar el primer error. Devuelve un vector con todos los registros creados.

<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:

```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas
El informe debe incluir lo siguiente:
  - Explicación de los TDA creados para el TP
    - Cada TDA debe tener una definición que explique qué hace y para qué se utiliza. Se debe incluir una explicación de qué datos y    operaciones importantes corresponden al TDA y cuáles son las responsabilidades del mismo.
  - Explicación de los TDAs reutilizados (de los implementados anteriormente en la materia) y para qué fueron utilizados.

  Para este TP implementé 4 TDA:

## TDA Pokedex
El TDA Pokedex es un almacen de pokemons, los cuales puedo insertar y obtener pokemones.
Lo utilizó para la opción de `Mostrar pokedex`, el cual debe mostrar en orden alfabetico cada pokemon, y para la selección de los pokemones que se hará al azar.

- `pokedex_crear`: Crea un ABB, siendo el nombre de los pokemones la función de comparación.  
- `pokedex_insertar_pokemon`: Inserta un pokemon, mejor dicho, la estructura del pokemon dado por el `pokedex.h`, ya que no será cualquier pokemon generalizado.  
- `pokedex_iterar`: Esta función llamará al recorrido inorden del ABB para mostrar en orden alfabetico cada pokemon. Decidí que sea `iterar pokedex` y no `mostrar pokedex`, porque mostrar algo o no, está dado por la iteración de los elementos, además, al poder iterar, podemos darle un diseño propio a cada linea.  
- `pokedex_cantidad`: Muestra la cantidad de pokemones almacenados en la Pokedex.  
- `pokedex_obtener_pokemon`: Esta función utiliza también el recorrido inorden el cual se teniende cuando quiero la posición de un pokemon en la pokedex, ya que, la idea de esta Pokedex es que estén los pokemones en orden alfabetico.

## TDA Menu
Para esté TDA primero pensé en que sea una estructura de Cola, porque todas las opciones que insertemos, tendremos que mostrarlo por orden de insersión, o sea, el primero que insertamos, es el primero que va a salir, pero luego pensé en que mejor es iterar las opciones para poder  mostrarlas (en vez de estar desencolando y encolando recursivamente para mostrar las opciones), con lo que llegué a la desición de que mejor es una estructura de Lista. Mencionar que no utilicé un Hash, porque al mostrar las opciones, iban a aparecer en desorde, y tampoco un ABB, porque depende cómo insertemos e iteremos, cómo se mostraría, y queremos que sea intuitivo que lo que inserto, así es el orden que quiero ver.

- El TDA Menu es una Lista de opciones con una estructura interna que llevará un `indice`, `texto` y una `funcion` a ejecutar para cada nodo.
- Para este TDA decidí que no habrá una función de busqueda, obtención ni eliminación, ya que el usuario pone los datos desde el comienzo y para fines del tp, es más practico tener funciones que usaré.
- Con el TDA Menu puedo llevar el control de algunas `banderas` por fuera de las opciones a ejecutar, por ejemplo: Si decidí `Mostrar pokedex`, lo que uno haría es llamar a la ejecución de dicha función y una vez que se termina de procesar dicha opción, se libere toda la memoria que llamé en esa ejecución, pero, si tengo en cuenta una `bandera que se encienda`, no tendría que para otra función
- El TDA Menu consiste en agregarle 

## TDA Tablero

## TDA Movimientos