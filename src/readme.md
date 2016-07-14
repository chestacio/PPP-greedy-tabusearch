# Progressive Party Problem con Greedy + Tabu Search

El siguiente documento explica el proceso de compilación del código y la ejecución del programa usando el algoritmo constructivo Greedy para la solución inicial y luego el algoritmo reparador Tabu Search para explorar y explotar.


* [Instalación Base de Datos y Server](#instalacion-base-de-datos-y-server)
* [API RESTful](#api-restful)
* [Ejemplos de Request y Response](#ejemplos-de-request-y-response)
* [Configurar IP](#configurar-ip)

## Compilación

Para compilar hay que ubicarse dentro de esta carpeta en el Terminal y ejecutar:

```sh
make
```
Lo anterior genera un ejecutable llamado ```Tarea2```

## Ejecución

El programa lee dos tipos de problemas: La instancia del PPP completo (42 yates en 6 tiempos) en 6 distintas configuraciones de yates anfitriones e yates invitados, y las instancias CSPLib que corresponden a versiones reducidas del problema.

Para la ejecución de la instancia del PPP completo es mediante:
```sh
./Tarea2 PPP [Configuración] [Cantidad iteraciones] [Largo Lista Tabú]
```
Donde ```[Configuración]``` es la configuracion del PPP que puede ser ```pp1```, ```pp2```, ```pp3```, ```pp4```, ```pp5```, ```pp6```, ```m1```, ```m2``` (estas dos últimas son versiones reducidas del PPP); ```[Cantidad iteraciones]``` e ```[Largo Lista Tabú]``` corresponden a la cantidad de interaciones y el largo de la lista Tabú en el algoritmo Tabu Search.

Para la ejecución de una instancia CSPLib es mediante:
```sh
./Tarea2 CSPLib [Configuración] [Cantidad iteraciones] [Largo Lista Tabú]
```
Donde ```[Configuración]``` es la instancia CSPLib que puede ser ```Ian01```, ```Ian02```, ```Ian03```, ```Ian04```, ```Ian05```, ```Ian06```, ```Ian07```, ```Ian08```, ```Ian09```, ```Ian10```; ```[Cantidad iteraciones]``` e ```[Largo Lista Tabú]``` corresponden a la cantidad de interaciones y el largo de la lista Tabú en el algoritmo Tabu Search.

### Archivos de Salida

Todos los archivos generados por el programa se encontrarán en la carpeta Salida y tienen como nombre la instancia del problema.

### Información por pantalla en el Terminal

Durante la ejecución se muestra una serie de información.

Primero se muestra la matriz resultante del algoritmo Greedy, donde cada x_{g,t} representa el yate host que va el invitado g en el tiempo t.

Luego muestra el desglose de penalizaciones incumplidas producto de no cumplir con las restricciones, además del total.

Finalmente muestra la matriz resultante del Tabu Search con el nuevo desglose de penalizaciones de restricciones.
