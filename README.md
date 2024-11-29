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

## Para este TP implementé 4 TDA:

### TDA Pokedex
El TDA Pokedex es un almacen de pokemones, los cuales puedo insertar y obtener pokemones.
Lo utilizó para la opción de `Mostrar pokedex`, el cual debe mostrar en orden alfabetico cada pokemon, y para la selección de los pokemones que se hará al azar.

- `pokedex_crear`: Crea un ABB, siendo el nombre de los pokemones la función de comparación.  
- `pokedex_insertar_pokemon`: Inserta un pokemon, mejor dicho, la estructura del pokemon dado por el `pokedex.h`, ya que no será cualquier pokemon generalizado.  
- `pokedex_iterar`: Esta función llamará al recorrido inorden del ABB para mostrar en orden alfabetico cada pokemon. Decidí que sea `iterar pokedex` y no `mostrar pokedex`, porque mostrar algo o no, está dado por la iteración de los elementos, además, al poder iterar, podemos darle un diseño propio a cada linea.  
- `pokedex_cantidad`: Muestra la cantidad de pokemones almacenados en la Pokedex.  
- `pokedex_obtener_pokemon`: Esta función utiliza también el recorrido inorden el cual se teniende cuando quiero la posición de un pokemon en la pokedex, ya que, la idea de esta Pokedex es que estén los pokemones en orden alfabetico.

### TDA Menu
Consiste en una Lista de opciones que el usuario ingresará con una estructura interna de `indice`, `texto` y una `funcion` a ejecutar para cada nodo. Para este TDA decidí que no habrá una función de busqueda, obtención ni eliminación, ya que el usuario pone los datos desde el comienzo y para fines del tp, es más practico tener funciones que usaré.  
Mencionar que este TDA tiene la ventaja de tener información entre funciones de cada opción como `banderas`. La idea es la siguiente: Si sé que en la opciones de `Mostrar pokemones` y `Jugar` con o sin semilla, necesito la Pokedex, entonces, si creo la Pokedex cuando quiero mostrar los Pokemones, esa información me la guardo para cuando quiera jugar, entonces no tengo que crear una nueva Pokedex con los mismos pokemones, si no que simplemente si ya lo creé en otra función, reutilizarlo.

- `menu_crear`: Crea una Lista para guardar cada opcion, la estructura de la opcion se crea internamente.  
- `menu_ingresar_opcion`: Se agrega una opcion, la cual es una estructura que se crea internamente, a la Lista de opciones.
- `menu_iterar_opciones`: Con el mismo argumento que al iterar la Pokedex, iteramos cada opción para darle el diseño que querramos para poder mostrarlo por pantalla.
- `menu_cantidad`: Muestra la cantidad de opciones que tiene el Menu.
- `menu_ejecutar_opcion`: Dado un indice, se ejecuta la opción. Se puede tener varias opciones con el mismo indice, pero solo el primero que se agrega es el que se podrá ejecutar.

### TDA Tablero
Es la representación de una matriz, hecho un tablero. Se utiliza para la lógica de la ubicación del jugador y pokemones en un tablero. Así como de obtener información. Mencionar que no implementé una función para eliminar un elemento de alguna posicion, por efectos del tp.

- `tablero_crear`: Se crea una matriz dado una cantidad de filas y columnas que el usuario decida darle. Cada posicion de la tablero tendrá una estructura que será un caracter y el color asociado al caracter.
- `tablero_colocar_elemento`: Se coloca el caracter y color (por defecto es blanco), en la posición que se desee.
- `tablero_posicion_esta_vacio`: Se verifica que no esté ocupado la posicion que el usuario ingrese.
- `tablero_mover_elemento`: Al ser el TDA Tablero independiende del juego, si alguien pone un caracer en alguna posición, para que se mueva, se debe mencionar de qué posición es el origen, y cual es el destino para ponerla vacia.
- `tablero_posicion_informacion`: Se obtiene el caracter y color pasado por punteros.

### TDA Movimientos:
Aquí, no supe si tenerlo como un TDA o no, pero dado que, como en el Tablero hace acciones como mover un elemento, en este TDA Movimiento almaceno funciones que, dado una clave, se ejecute una acción, casi parecido como un TDA Menu, con la diferencia que solo puede haber una clave, y esa clave tendrá la función a ejecutar, no puede haber otra función con la misma clave. Si bien pude haber hecho una función con muchos if, creo que una manera más elegante es almacenando todas logicas en un hash.  
Este TDA lo utilizo para realizar los movimientos de cada pokemones y también las del usuario.

- `movimientos_crear`: Crea un hash para gaurdar el nombre del movimiento (clave) y una función asociada (valor).
- `movimientos_agregar`: Se agrega la clave y la función que tiene la forma que se le pide, 1 char* 2 size_t* y 2 size_t, esto es así porque, si bien los movimientos independiente tienen una sola dirección, cuando un movimiento depende de otro, necesito saber qué tipo de movimiento se hizo para así yo poder realizar, y como no conozcó qué voy a modificar, puede ser arriba, abajo, derecha o izquierda.  
- `movimiento_realizar`: Ejecuta la función de movimiento a realizar.