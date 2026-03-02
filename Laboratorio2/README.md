# Instrucciones del lab 2

En el archivo `lab2.hpp` se encuentra:

- Definicione de varios "conceptos": No es necesario ni recomendado que lean esto,
    es solo para que el código compile.
- Definicion de la clase `List`: al igual que el punto anterior, no es necesario
    ni recomendado que lean esto, es solo para que el código compile. Piensenlo
    como una "caja negra" que funciona de manera similar a los vectores de C++.
- Varias funciones incompletas: estos son sus ejercicios.

## Clase `List`

Las listas son un "subconjunto" de los vectores. Estas solo poseen los siguientes metodos:

- `List()`: constructor por defecto, crea una lista vacia.
- `List(lista)`: permite crear una lista a partir de otra.
- `lista1 = lista2`: permite clonar la lista2 y asignarla a lista1.
- `l.cons(x)`: permite agregar el elemento x al inicio de la lista l. Por ejemplo,
    si l es la lista [2, 3], entonces l.cons(1) devuelve la lista [1, 2, 3].
- `l.head()`: devuelve el primer elemento de la lista l.
    Por ejemplo, si l es la lista [1, 2, 3], entonces l.head() devuelve 1.
- `l.tail()`: devuelve la lista l sin su primer elemento.
    Por ejemplo, si l es la lista [1, 2, 3], entonces l.tail() devuelve la lista [2, 3].
- `l.isEmpty()`: devuelve true si la lista l es vacia, y false en caso contrario.
    Por ejemplo, si l es la lista [1, 2, 3], entonces l.isEmpty() devuelve false.
    Si l es la lista [], entonces l.isEmpty() devuelve true.
-  `show(l)`: devuelve una representacion en string de la lista l.
    Por ejemplo, si l es la lista [1, 2, 3], entonces show(l) devuelve "[1, 2, 3]".
    Si l es la lista [], entonces show(l) devuelve "[]".
- `l1 == l2`: devuelve true si las listas l1 y l2 son iguales, y false en caso contrario.
    Por ejemplo, si l1 es la lista [1, 2, 3] y l2 es la lista [1, 2, 3], entonces l1 == l2 devuelve true.
    Si l1 es la lista [1, 2, 3] y l2 es la lista [4, 5, 6], entonces l1 == l2 devuelve false.

## Puntuacion:

Cada pregunta vale la misma cantidad de puntos,
y el puntaje final sera en base a 100.


## Archivos:

- El archivo `lab2.hpp` sera el archivo en donde ustedes deberan responder todo.
- El archivo `main.cpp` lo pueden modificar para correr sus pruebas.

## Compilacion

Pueden compilar el codigo mediante el comando `make all`, o
mediante el comando `g++ lab2.hpp lab2.cpp -std=c++26 -o lab2.sh`

## Ejercicio 1: reduceLeft

Implementen la funcion `reduceLeft(f,init,xs)`, la cual recibe una funcion `f`, un
parametro `inti` y una lista `xs`, y devuelve el resultado de aplicar la funcion `f` a los elementos de la lista
`xs`, de izquierda a derecha, utilizando `init` como valor inicial.

Por ejemplo, si `f` es la funcion de suma, `init` es 0 y `xs` es la lista
[1, 2, 3], entonces `reduceLeft(f, init, xs)`  devuelve 6, ya que se calcula (((0 + 1) + 2) + 3).

Pueden pensar este proceso como reemplazar las `,` por el operador:

[1,2,3] -> [0 + 1 + 2 + 3] -> ((0 + 1) + 2) + 3

Deben realizar esta funcion de manera iterativa.

## Ejercicio 2: reduceLeftRec

Lo mismo que el ejercicio 1 pero de manera recursiva.

## Ejercicio 3: map

Implementen la funcion `map(f,xs)`, la cual recibe una funcion `f`, una lista `xs`,
y devuelve una nueva lista con el resultado de aplicar la funcion `f` a cada elemento de la lista `xs`.

Por ejemplo, si `f` es la funcion de multiplicacion por 2 y `xs` es la lista [1, 2, 3],
entonces `map(f, xs)` devuelve la lista [2, 4, 6].

Deben realizar esta funcion de manera iterativa.

## Ejercicio 4: mapRec

Lo mismo que el ejercicio 3 pero de manera recursiva.

## Ejercicio 5: mapReduce

Lo mismo que el ejercicio 3, pero deben utilizar unicamente la funcion `reduceLeft` para implementar esta funcion.

## Ejercicio 6: concat

Implementen la funcion `concat(xs)`, la cual recibe una lista de listas `xs`,
y devuelve una nueva lista con el resultado de concatenar todas las listas de `xs`.

Por ejemplo, si `xs` es la lista de listas [[1, 2], [3, 4], [5, 6]], entonces `concat(xs)` devuelve la lista [1, 2, 3, 4, 5, 6].

Deben realizar esta funcion usando la funcion `reduceLeft`

## Ejercicio 7: filter

Implementen la funcion `filter(f,xs)`, la cual recibe una funcion `f`, una lista `xs`,
y devuelve una nueva lista con el resultado de filtrar los elementos de la lista `xs` que cumplen la condicion dada por la funcion `f`.

Por ejemplo si `f` es la funcion que devuelve true si un numero es par y false
en caso contrario, y `xs` es la lista [1, 2, 3, 4, 5, 6],
entonces `filter(f, xs)` devuelve la lista [2, 4, 6].

Deben realizar esta funcion de manera iterativa.

## Ejercicio 8: filterRec

Lo mismo que el ejercicio 7 pero de manera recursiva.

## Ejercicio 9: filterMap

Lo mismo que el ejercicio 7, pero deben utilizar unicamente la funcion `map` para implementar esta funcion.

## Ejercicio 10: filterReduce

Lo mismo que el ejercicio 7, pero deben utilizar unicamente la funcion `reduceLeft` para implementar esta funcion.

## Ejercicio 11: groupEqual

Implementen la funcion `groupEqual(xs)`, la cual recibe una lista `xs` cuyos elementos
admiten igualdad (es decir, permiten `x == y`),
y devuelve una nueva lista de listas, donde cada sublista contiene elementos
iguales consecutivos de la lista `xs`.

Por ejemplo, si `xs` es la lista [1, 2, 2, 3, 3, 3], entonces `groupEqual(xs)` devuelve la lista de listas [[1], [2, 2], [3, 3, 3]].

Debe implementar esto usando la funcion `reduceLeft`


## Ejercicio 12: encode

Implementen la funcion `encode(xs)`, la cual recibe una lista `xs` cuyos elementos admiten igualdad (es decir, permiten `x == y`),
y devuelve una nueva lista de listas, donde cada sublista contiene dos elementos:
el primer elemento es un numero que representa la cantidad de veces que un elemento se
repite de manera consecutiva en la lista `xs`, y el segundo elemento es el elemento en si.

Por ejemplo, si `xs` es la lista [1, 2, 2, 3, 3, 3, 3], entonces `encode(xs)` devuelve la lista de listas [[1, 1], [2, 2], [4, 3]].

Debe implementar esto usando la funcion `groupEqual`

## Ejercicio 13: decode

Implementen la funcion `decode(xs)`, la cual es el inverso de la funcion encode, es
decir debe cumplirse que: `decode(encode(xs)) == xs` y que
`encode(decode(xs)) == xs`.

## Ejercicio 14: zip

Implementen la funcion `zip(xs, ys)`, la cual recibe dos listas `xs` y `ys`
y devuelve una nueva lista de pares, donde el primer elemento de cada par es un elemento
de la lista `xs` y el segundo elemento es un elemento de la lista `ys`.

Por ejemplo, si `xs` es la lista [1, 2, 3] y `ys` es la lista ['a', 'b', 'c'],
entonces `zip(xs, ys)` devuelve la lista de pares [(1, 'a'), (2, 'b'), (3, 'c')].

Si alguna de las dos listas es mas corta que la otra, el resto de
los elementos de la lista mas larga se ignoran.


## Ejercicio 15: dropEveryN

Implemente la funcion `dropEveryN(xs, n)`,
la cual recibe una lista `xs` y un numero `n`,
y devuelve una nueva lista con el resultado de eliminar cada
n-esimo elemento de la lista `xs`.

Use la funcion `zip` para implementar esta funcion.

## Ejercicio 16: sortBy

Implemente su algoritmo de sorting del laboratorio 1,
pero utilizando la funcion `f` como funcion de comparacion en vez de `<=`.

Asuma que la funcion `f` simboliza el operador de menor o igual.


## Ejercicio 17: rotateByN

Implemente la funcion `rotateByN(xs, n)`,
la cual recibe una lista `xs` y un numero `n`,
y devuelve una nueva lista con el resultado de rotar la lista
`xs` a la izquierda por `n` posiciones.

Utilice la funcion `sortBy` para implementar esta funcion.
