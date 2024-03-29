Compilación
Para compilar el programa, utiliza la orden make. Esto generará un ejecutable llamado QKP.

Ejecución
Los argumentos del programa son los siguientes:
./$ejecutable $Algoritmo $fichero_de_entrada_de_datos [$semilla]
$Algoritmo: Puede ser G para greedy o BL para búsqueda local.
$fichero_de_entrada_de_datos: Ruta del archivo de datos de entrada.
$semilla (opcional): Para el algoritmo greedy no es necesario. Para búsqueda local, si no se indica, no se fijará semilla.

Salida
El programa mostrará por pantalla la siguiente información:
W: Capacidad de la mochila.
Peso de la mochila final.
Beneficio final.
Asignación (unos y ceros).
Última línea: $beneficio $milisegundos.

Scripts
Es importante comentar todas las salidas de texto por pantalla, excepto la última línea que muestra el beneficio y el tiempo, y volver a compilar con make, si se desea utilizar alguno de los scripts.

Se desarrollaron 5 scripts para ejecutar el programa con diferentes algoritmos para cada uno de los archivos de datos de entrada:
ScriptGreedy.sh: Ejecuta el programa con el algoritmo greedy.
ScriptBL.sh: Ejecuta el programa con el algoritmo de búsqueda local.
ScriptBL-DOBLE.sh: Ejecuta el programa con el algoritmo BL usando la generación de vecinos ampliada (BL-DOBLE).
ScriptBL-MEJOR.sh: Ejecuta el programa con el algoritmo BL mejor vecino.
ScriptBL-MEJOR-DOBLE.sh: Ejecuta el programa con el algoritmo BL mejor vecino usando la generación de vecinos ampliada (BL-MEJOR-DOBLE).
Cada script realiza la media del beneficio y del tiempo para archivos de datos de tamaño y densidad comunes, y guarda los resultados en archivos en formato CSV.