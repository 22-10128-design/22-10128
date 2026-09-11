# Reporte Técnico - Proyecto Macondian Part II

## Modelos de Inteligencia Artificial Utilizados
- **Modelo:** Gemini (Google)

# Reporte de Desarrollo - Proyecto Macondian Simulator

## 1. ¿Cómo procesamos los datos de los sensores? (`parseAndProcessMessage`)

Para procesar las lecturas que van llegando del simulador en tiempo real, armamos una lógica paso a paso para limpiar el texto y sacar los datos que nos interesan:

1. **Recepción desde el Worker:** Un Web Worker que corre en segundo plano (`tgmr-thx-1138.js`) nos va mandando mensajes constantemente. Los recibimos mediante el evento `onmessage`.
2. **Identificación de Lotes (Batches):** Lo primero que hace la función es revisar si el mensaje es un encabezado de lote (por ejemplo, `Batch 1:`). Si es así, guardamos el número de batch actual en una referencia (`currentBatchRef`) y ponemos una marca en el log para separar los datos visualmente.
3. **Extracción con Expresión Regular (Regex):** Si el mensaje trae datos de un sensor, usamos la expresión regular `/(Y\d+|L\d+)\s+\[\s*([\d\.\,\s\-]+)\s*\]/` para dos cosas:
   - Identificar cuál sensor mandó la señal (`Y1`, `L1`, etc.).
   - Obtener la lista de números. Esa cadena la limpiamos usando `.split(',')` para separar cada dato, los convertimos a números con `parseFloat()` y filtramos cualquier cosa que no sea un número válido (`!isNaN`).
4. **Cálculo del Valor Macondo:** Ya con la lista de números limpia, se la pasamos a la función `processMacondianSensor(rawValues, 0.08)`, que es la que se encarga de calcular el valor final del sensor.
5. **Guardado y Graficación:** El resultado formateado lo guardamos en el estado `seriesLog` para mostrarlo en el monitor de texto, y también guardamos el objeto estructurado en `chartData` para que el componente del gráfico (`Chart.tsx`) se actualice automáticamente.

---

## 2. Ajustes en la Vista de Imágenes y Carrusel (Tarea adicional)

### Lógica del Carrusel
- **Rotación Secuencial:** Dejamos las imagenes en un orden fijo recorriendo los arreglos con indices (`singleIndex`, `artIndex`, `macIndex`) para que cambien en una secuencia establecida(no al azar).
- **Uso del Módulo para el Ciclo:** Usamos la operacion `% ARRAY.length` al aumentar el indice. De esta forma, cuando el carrusel llega a la última imagen, la cuenta vuelve a 0 automáticamente y no se rompe.
- **Sincronización en la Vista Doble:** Para que las dos imagenes cambien al mismo tiempo y no marea verlas desfasadas, usamos un solo `setInterval` de 2000 ms que actualiza ambos lados en el mismo momento.
- **Limpieza de Temporizadores:** Metimos la logica dentro de un `useEffect` que regresa una función de limpieza (`clearInterval`). Esto es clave para detener los contadores cuando nos cambiamos de pestaña y evitar que la memoria se llene de procesos basura.

### Cambios en la Interfaz (UI) y CSS
- **Menú Unificado:** En lugar de tener botones separados para cada carrusel en la barra principal (`ToolBar`), agrupamos todo dentro de un solo botón llamado **Image**. Dentro de esa vista agregamos unos botones secundarios para elegir si se quiere ver el carrusel simple (**Carousel**) o el dividido (**Double**).
- **Centrado de Imágenes:** Envolvimos el componente de la imagen dentro de un `div` con **Flexbox** (`display: flex`, `justifyContent: 'center'`, `alignItems: 'center'`). Con esto logramos que cualquier imagen quede bien centrada horizontal y verticalmente sin importar su tamaño.

# Lista de pronts utilizados con la IA (Gemini)
- **(Se le facilito el codigo de macondian.tsx para hacer las preguntas mas sencillas y directas de realizar)**

1.- Tengo un Web Worker enviando cadenas de texto con datos de sensores (como Y1, L1) y encabezados de Batch X:. Necesito una función para procesar ese log: extraer con regex el ID del sensor y su arreglo de números, pasarlos por processMacondianSensor(rawValues, 0.08) y guardar el resultado tanto en un estado para el monitor (seriesLog) como en un arreglo estructurado para pasárselo a la gráfica (Chart).

2.- Para la vista imageView en Macondian.tsx, quiero armar un carrusel que cambie de imagen automáticamente cada 2 segundos. Tengo las colecciones ART_IMAGES y MAC_IMAGES. Haz que cada 2000 ms se elija una imagen al azar de la lista combinada y actualice el componente <Image/>. (aqui sugirio limpiar el temporizar para que no se consuma memoria cuando no se este viendo image, es decir cuando se cambie de pestaña).

3.-El enfoque aleatorio no me sirve porque repite imágenes seguidas y si uso la vista dividida (double) con dos intervalos distintos, los paneles cambian en tiempos diferentes y se ve raro. Haz que las imágenes roten de forma estrictamente secuencial de 0 a N usando el operador de módulo % para ciclar infinitamente. Para el sub-modo double, en lugar de dos setInterval independientes con distintos tiempos, usa un único temporizador que actualice los índices de ambos paneles exactamente al mismo tiempo cada 2000 ms y por ultimo modifica el useEffect para controlar esto según el modo seleccionado que son single o double.

4.- Las imágenes dentro de los paneles no quedan bien alineadas cuando tienen resoluciones distintas y se des descuadran verticalmente. Me puedes ayudar con eso?

-**(hay otros de solucion de errores tontos que no veia que estaba mal pero no soy muy relevantes)**