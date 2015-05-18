#ifndef ENTRADAAUDIO_H
#define ENTRADAAUDIO_H

#include "utilidades.h"
#include <alsa/asoundlib.h>
#include <GL/glut.h>
#include <GL/gl.h>

union byte
{
	unsigned char uchar_val;
	char char_val;
};

class EntradaAudio
{
	public:
		char* data;// array de datos tipo caracter
		float* float_data; //array de datos tipo real 
		int buffer_blocks;// cantidad de bloques del "buffer"
		int data_start;//punto de inicio de la data
		int data_size;//Tamaño del array de la datos 
		int data_end;//Punto final del array de datos
		int data_write;
		int write_padding;
		int front_padding;

		//Amplitud y Frecuencia para mostrarlas por pantalla
		float amplitud;
		float periodo;
		float frecuencia; 

		bool pause;
		pthread_t capture_thread;
		timeval last_write;
		float avg_write_interval;

		// Datos de audio del dispositivo
		int bytes_per_frame;//cantidad bytes por frame 
		int read_frames, read_bytes;//cantidad de frames leidos, cantidad de bytes leidos 
		int rate; /* Frecuencia de muestreo */
		float dt;
		int exact_rate;	/* Frecuencia de muestreo devuelto por */
						/* snd_pcm_hw_params_set_rate_near */ 
		int dir;		/* exact_rate == rate --> dir =  0 */
						/* exact_rate < rate  --> dir = -1 */
						/* exact_rate > rate  --> dir =  1 */

		int periods;       // numero de periodos
		snd_pcm_uframes_t periodsize; // Tamanio del periodo (bytes)
		int size, exact_size;  // tamaño del buffer

		snd_pcm_t *pcm_handle;  // Controlador para el  dispositivo PCM 
		snd_pcm_stream_t stream;// Reproduccion del flujo

		/* Esta estructura contiene informacion acerca del  
		hardware y de como puede ser usado para especificar la     
		configuracion que se utilizara para el flujo PCM . */ 
		snd_pcm_hw_params_t *hwparams;

		/* Nombre del dispositivo PCM, ejemplo plughw:0,0
		El primer número es el numero de la  tarjeta de sonido
		el segundo número es el numero del dispositivo  */
		char *pcm_name;

	public:
		EntradaAudio();
		int index( int i );
		static void* audioCapture(void* a);
		void getTimeSpan(float* time, int* index, int num);
		int initDevice();

		float getAmplitud();
		float getFrecuencia();
};
#endif

 
