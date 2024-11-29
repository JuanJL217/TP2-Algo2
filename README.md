<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP 2

## Repositorio de (Juan Ernesto Juarez Lezama) - (110418) - (jjuarez@fi.uba.ar)

- Para ejecutar:

```bash
make
```
Se ejecutará tanto las pruebas como el tp2.c con valgrind.

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

---
##  Funcionamiento

### Menu principal  
Mientras voy explicando como se desarrolla la lógica del juego, primero mencionar el uso del TDA Menu, que es donde inserto las 4 opciones fundamentales para el TP, el cual decidí agregarle 2 funciones extras, que son: `Otras opciones` y `Reestablecer valores predeterminados`.  
La opción de `Otras opciones` me abre otro menu, el cual tengo las opciones de cambiar cosas del juego, como el `tiempo maximo`, la `cantidad de pokemones` en el juego y las `dimensiones del tablero`. Y la opción de `Reestablecer valores predeterminados` en el menu principal, es para volver a los ajustes predeterminados del juego.

Esto de las ediciones, así como cuando ingreso a la opción `Mostrar pokedex`, cargo los datos de los pokemons para poder mostrarlos, esta información se guarda en la variable `banderas`, con lo cual, no tengo que destruir la pokedex, porque si quiero jugar, voy a tener que volver a cargar la pokedex, entonces mejor tener la referencia de la pokedex creada en la otra función y se libera cuando el programa cierre.

```c
booleanos banderas = { .menu_seguir = true,
            .opciones_seguir = false,
            .pokedex = NULL,
            .colores = NULL,
            .semilla = NULL,
            .cantidad_objetivos = CANTIDAD_OBJETIVOS_INICIAL,
            .cantidad_filas = FILAS_INICIAL,
            .cantidad_columas = COLUMNAS_INICIAL,
            .tiempo_maximo = INICIAL};
```

### Logica del Juego  
Empezando por el uso del `Hash`, para almacenar los colores, el cual, escribiendo la clave del color ("Rojo"), obtengo su código ANSI.  

Use 3 `Listas`, Una para almacenar los pokemones seleccionados para el juego (debido a que obtengo el pokemon de la Pokedex en una posicion al azar), y la otra Lista es una lista de posiciones de los pokemones que se capturó, debido a que itero la lista de pokemones seleccionados para verificar si el usuario está en la misma posición que alguno de los pokemones (lo que significa que el usuario capturó al pokemon) entonces guardo la posición de dicho pokemon (de la lista de seleccionados) en la lista de capturados para su eliminación de la lista de seleccionados, y la otra Lista es para guardar los grupos formados cuando capturo pokemones.  

Para armar los grupos, utilicé Colas, mejor dicho, la Lista que forma los grupos es una Lista de Colas. Consiste en que, a medida que vayamos formando grupos, meterlos en una Cola, cuando se rompa el multiplicador, esa Cola lo guardo en la última posición de la Lista, como llevo un contador del mayor grupo formado, todos las Colas que tengan la dicha longitud, se mostarán, o sea, si armaste 3 grupos de 5, y 5 es tu mayor grupo formado, se mostrará todos los grupos que formaste de 5, no solo el primero que formaste. Decidí que sea una Cola, porque así podemos ver en orden que capturamos primero y último.

Para saber el último pokemon que capturamos, utilicé una Pila, ya que, con la Pila puedo ver el tope y saber la cantidad de pokemones capturados.  

### Menciones
- Cuando el usuario tiene un contador superior a uno, supongamos que 3, el siguiente pokemon que atrape, su puntuación se va a multiplicador y se le sumará al puntaje del usuario, de ahí, el multiplicador vuelve a 1.  






<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>


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