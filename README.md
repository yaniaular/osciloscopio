# osciloscopio

Universidad de Carabobo
Facultad Experimental de Ciencia y Tecnología
Departamento de Computación
Redes de Computadores
Prof. Antonio Castañeda

OsciloscopioMC

Yanina Aular, 19 588 966
Luis González, 19 919 029
Osval Reyes, 20 512 105
Richard Rodríguez, 19 553 202

Viernes, 11 de Febrero 2011

Introducción
------------

Un osciloscopio es un tipo de instrumento electrónico que permite visualizar voltage en función del tiempo. Su uso es aplicable a diversos campos, tales como la química, física, electrónica, entre otros.

	OsciloscopioMC es una aplicación que actúa como un osciloscopio electrónico; solo que, a diferencia de este último, no es un instrumento electrónico, si no un software que, haciendo uso de la tarjeta de sonido en donde se utilice, pretende cumplir con la misma función.

	El presente documento explicará el funcionamiento de OsciloscopioMC, así como su proceso de desarrollo, y todo lo concerniente al software que hay en este CD. Adicionalmente, se abordarán conceptos necesarios para una mejor comprensión del tema, tales como onda, frecuencia, amplitud, entre otros.
Justificación

Un osciloscopio es una herramienta de mucha utilidad, necesario en dibersos campos. Debido a que este dispositivo normalmente tiene un costo elevado, hemos decidido hacer un software que, en la medida de lo posible, cumpla el mismo propósito; usando para ello la tarjeta de sonido del equipo en donde se va a utilizar; con esto, evitamos los costos del osciloscopio y facilitamos el uso y la configuración al momento de su utilización.
Limitaciones: 
Debido a que se utiliza la tarjeta de sonido para el funcionamiento del OsciloscopioMC, está sujeto a ciertas restricciones, a saber:

Voltaje: El voltaje soportado por el Osciloscopio se encuentra entre 0V y 0.5V, lo que podría ser insuficiente dependiendo del uso que se desee darle.

Frecuencia de entrada: debido a que la tarjeta interpreta señales discretas (digitales) la frecuencia de entrada máxima que se puede procesar, estará proporcionalmente limitada por la frecuencia máxima de muestreo que soporte dicha tarjeta. Usualmente, este valor es de 48000 Hz; no obstante, para este proyecto utilizaremos 44100 Hz, ya que este valor permite procesar con precisión una frecuencia de 20000 Hz, que es mas que suficiente para el propósito de este proyecto.

Marco Teórico
-------------

Onda: Es la propagación de una perturbación (energía) a través de un medio. 

Amplitud: Distancia entre el punto más alejado de una onda y el punto de equilibrio o medio.

Amplitud pico a pico: Magnitud que representa la diferencia entre el valor máximo y el valor mínimo de una onda.

Frecuencia: Magnitud cuantitativa de repeticiones por unidad de tiempo de cualquier fenómeno o suceso periódico.

Frecuencia de muestreo: Es el número de muestras por unidad de tiempo que se toman de de una señal continua (analógica) para producir una señal discreta (digital).

Hertz (Hz): Unidad de Frecuencia y según el Sistema Internacional (S.I.), expresa la cantidad de ciclos de reloj por segundo.

Voltio: Unidad que mide la tensión y según el Sistema Internacional (S.I.), expresa la diferencia de potencial eléctrico.

Voltaje pico a pico: Magnitud que representa la diferencia de voltaje entre el máximo y el mínimo.

Período: Cantidad de tiempo necesario para completar un ciclo.

Ancho de banda: Indican el rango de frecuencias que el osciloscopio puede medir con exactitud.
Tipos de ondas.

- Onda   senoidal:  También   llamada  Sinusoidal.   Se   trata   de   una   señal analógica, puesto que existen infinitos valores entre dos puntos cualesquiera del   dominio.   De   hecho,   su   representación   es   la   gráfica   de   la   función matemática Seno.

- Onda cuadrada: Onda que alterna su valor entre dos valores extremos, sin pasar por los valores intermedios, a diferencia de lo que ocurre con la onda sinusoidal.

- Otras:  Existen   muchos   otros   tipos   de   ondas,   tales   como   triangular, cuadrática, sierra, aleatoria, entre otras; pero no serán abordadas en esta 

GUI: Interfaz Gráfica de Usuario, ventanas con las que el usuario interactúa con las aplicaciones.

Qt:  Biblioteca   para   interfaces   gráficas,   desarrollada   por   Trolltech   y posteriormente comprada por Nokia.

Alsa: (Advanced Linux Sound Architecture) es un componente del núcleo GNU/Linux, que se encarga del manejo de dispositivos de sonido y su interacción con las aplicaciones.

PThreads: Biblioteca estándar para el manejo de hilos POSIX.

Marco metodológico
------------------

En esta fase del presente trabajo se muestran técnicas y el proceso de desarrollo que fue aplicado para el OsciloscopioMChasta su versión 1.0. A continuación se describirá el proceso.

Investigar sobre las bibliotecas disponibles para el manejo del sonido y para la interfaz gráfica. Luego de unos días de investigación sobre las distintas maneras de construir la interfaz gráfica, el equipo de desarrollo optó por usar sobre C++, Qt4 y OpenGL para las gráficas. Y para el manejo del sonido, el equipo decidió utilizar la librería compartida para aplicaciones ALSA, asoundlib2, por las facilidades de uso y excelente documentación en su página oficial del Proyecto ALSA.

- Con los elementos base para el inicio del desarrollo, parte del equipo comenzó la lectura de la documentación referencia de la API de ALSA, probando al mismo tiempo los ejemplos para el uso del dispositivo, como son: la apertura, modos de apertura y escritura en el mismo.
Una vez decididas las bibliotecas que se iban a utilizar e investigado su funcionamiento, el equipo comenzó a experimentar leyendo datos del micrófono para el caso de ALSA, y a intentar graficarlos con OpenGL.

Para poder graficar los datos obtenidos del micrófono de la tarjeta de sonido, se aplicó una transformada, de manera de que. en la gráfica se correspondieran con el voltaje de entrada. Adicionalmente, se necesitó truncar algunos decimales de los datos transformados, de manera de calcular con mayor precisión la frecuencia de entrada, y evitar que la máquina complete dichos decimales con números basura.
Desarrollo
	
	Como producto de la investigación, estudio y análisis; se desarrolló el osciloscopio OsciloscopioMC en su version 1.0, y a continuación describiremos su funcionamiento.

1. Gráfica: Se visualizará la forma de onda, el tiempo se encontrará sobre el eje horizontal y la amplitud
 sobre el eje vertical.

2. Pos Y: Nos permite modificar la posición de la forma de onda en el eje Y.
 
3. Pos X: Nos permite modificar la posición de la forma de onda en el eje X.

4. CH1 TIME/DIVS (ms): Nos permite modificar la cantidad de tiempo en milisegundos que existe por división sobre el eje X.

5. CH1 VOLT/DIVS (V): Nos permite modificar la cantidad de voltios que existe por división sobre el eje Y.

6. Amplitud: Se muestra la amplitud de la onda medida en voltios pico. 

7. Frecuencia: Se muestra la frecuencia de la onda. Si una señal se repite, tiene una frecuencia. La frecuencia se mide en hert-zios (Hz) y es igual al número de veces que una señal se repite en un
 segundo (ciclos por segundo).


8. Periodo: Se muestra el periodo de la onda, esto es la cantidad de tiempo necesario para completar un ciclo.

9. Detener/Continuar: Cuando la forma de onda se encuentra en movimiento, éste botón nos permite detenerla para poder visualizarla de manera estática. Cuando se encuentra detenida, la función del botón nos permite animar la onda nuevamente.

10. Grosor: Modifica la intensidad de la onda, el rango de valores se encuentra entre 1,0 y 5,0 de grosor.










	
Conclusiones y Recomendaciones
------------------------------

	Luego de completar el desarrollo del software OsciloscopioMC, podemos concluir que:
En muchos casos, no es necesario adquirir un osciloscopio, ya que puede ser reemplazado por nuestro software o uno equivalente, lo que se reflejaría en disminución de costos.

-	Sí se paralelizan las distintas actividades de un programa, utilizando múltiples hilos, mejorará notablemente la eficiencia; especialmente en aquellos equipos que tengan varios núcleos de procesamiento.


Referencias Bibliográficas

http://www2.fices.unsl.edu.ar/~areaeyc/lme/services/Conceptos_Basicos_Osciloscopios.pdf

http://www.todoespia.com/productos/manuales/revistaTE/osciloscopio.pdf

http://es.wikipedia.org/wiki/Arquitectura_de_Sonido_Avanzada_para_Linux
