#include <GL/glut.h>
#include <GL/gl.h>
#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include "EntradaAudio.h"

//Constructor de la clase EntradaAudio(entrada de audio) 
EntradaAudio::EntradaAudio()
{
	//En esta parte del codigo se inicializan los valores iniciales 
	//de la clase EntraAudio
	pause = false;// no esta detenido
	buffer_blocks = 50;//cantidad de blockes de buffer 50
	bytes_per_frame = 2;//bytes por muestra  2, 16 bits
	read_frames = 1000;//Tamaño del vector de buffer
	write_padding = read_frames * 4;
	front_padding = read_frames;
	avg_write_interval = 0.1;

	read_bytes = read_frames * bytes_per_frame;
	data = new char[read_bytes];

	//Amplitud y Frecuencia de la onda
	amplitud=0;
	frecuencia=0;

	data_start = read_frames * 2;
	data_end = data_size-1;
	data_write = 0;
	data_size = buffer_blocks * read_frames;
	float_data = new float[data_size];

	rate = 44100; //Tasa de flujo
	dt = 1.0 / (float)rate; // delta t, tiempo existente entre cada muestra
	periods = 2;
	periodsize = 8192;
	size = exact_size = (periodsize * periods)>>2;

	if( initDevice() < 0 )
	{
		exit(1);
	}
	
	pthread_create(&capture_thread, NULL, audioCapture, (void*)this); //Comienza a capturar el hilo
}
    
int EntradaAudio::index( int i )
{
	while( i < 0 )
		i += data_size;
	
	return i % data_size;
}
   
//Aqui es donde se captura el sonido
void* EntradaAudio::audioCapture(void* a)
{
	EntradaAudio* ai = (EntradaAudio*) a;

	float inv256 = 1.0 / 256.0;
	float inv256_2 = inv256*inv256;

	while( true )
	{
	
		int n;
		if( ! ai->pause )
		{
			while((n = snd_pcm_readi(ai->pcm_handle, ai->data, ai->read_frames)) < 0 )
			{
				snd_pcm_prepare(ai->pcm_handle);
				
			}

			byte b;
			int write_ptr, read_ptr;

			//Se Transforma los valores capturados a una escala mas pequeña
			//para poder dibujar la forma de onda y visualizarla de manera
			//correcta
			for( int i = 0; i < n; i++ )
			{
				read_ptr = i * 2;
				write_ptr = ai->index(ai->data_write + i);
				b.char_val = ai->data[read_ptr];
				ai->float_data[write_ptr] = (float)ai->data[read_ptr+1]*inv256 + (float)b.uchar_val*inv256_2;
			}

			ai->data_end   = ai->data_write;
			ai->data_write = ai->index(ai->data_write+n);
			ai->data_start = ai->data_end - (ai->data_size - ai->write_padding);
			timeval t;
			gettimeofday(&t, NULL);
			ai->avg_write_interval = ai->avg_write_interval * 0.7 + timeDiff(ai->last_write, t) * 0.3;
			ai->last_write = t;
		}
		else
		{
			usleep(10000);
		}
	}
	
	fprintf(stderr, "Captura de hilo de salida.\n");
}
//Fin de captura de sonido
   
void EntradaAudio::getTimeSpan(float* time, int* index, int num)
{
	float sh;

	if( pause )
	{
		sh = 0;
	}
	else
	{
		timeval t;
		gettimeofday(&t, NULL);
		sh = timeDiff(last_write, t);
	}

	float min_time = 0;
	float max_time = (data_end - data_start) * dt;

	for( int i=0; i<num; i++ )
	{
		if( time[i] < min_time )
			time[i] = min_time;
		
		if( time[i] > max_time )
			time[i] = max_time;

		index[i] = data_end - front_padding - (int)((time[i]-sh)*(float)rate);
		time[i] = sh + ((data_end - index[i] - front_padding) * dt);
	}
}
//Inicializacion del dispositivo PCM , verificando que no haya errores
int EntradaAudio::initDevice()
{
	stream = SND_PCM_STREAM_CAPTURE;
	pcm_name = strdup("plughw:0,0");// Iniciando pcm_name.
	snd_pcm_hw_params_alloca(&hwparams);// Asignar la estructura snd_pcm_hw_params_t en la pila

	//Abrir PCM (Codigo de pulso modulado),  El último parámetro de 
	//esta función es el modo. Si esto se establece en 0, el modo estándar es utilizado. 
	//Otros posibles valores son SND_PCM_NONBLOCK y SND_PCM_ASYNC. Si SND_PCM_NONBLOCK
	//acceden a leer y escribir en el dispositivo PCM.
	//Si SND_PCM_ASYNC se especifica, SIGIO se emitirá cada vez que un
	//período ha sido completamente procesados por la tarjeta de sonido. 
	if (snd_pcm_open(&pcm_handle, pcm_name, stream, 0) < 0)
	{
		fprintf(stderr, "Error al abrir el dispositivo PCM %s\n", pcm_name);
		return(-1);
	}

	//Inicializamos hwparams con el espacio de configuración completa
	if (snd_pcm_hw_params_any(pcm_handle, hwparams) < 0) {
		fprintf(stderr, "No puede ser configurado este dispositivo PCM.\n");
		return(-1);
	}

	//Establecer el tipo de acceso. Esto puede ser 
	//SND_PCM_ACCESS_RW_INTERLEAVED o SND_PCM_ACCESS_RW_NONINTERLEAVED.
	if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
	{
		fprintf(stderr, "Error estableciendo el acceso.\n");
		return(-1);
	}

	// Establecer formato de muestra
	if (snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE) < 0)
	{
		fprintf(stderr, "Error estableciendo el formato.\n");
		return(-1);
	}

	// Establecer la frecuencia de muestreo. Si la frecuencia exacta no es soportada
	// por el hardware, se usara la frecuencia mas cercana posible.
	exact_rate = rate;
	
	if (snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, (uint*)&exact_rate, 0) < 0)
	{
		fprintf(stderr, "Error configurando la frecuencia.\n");
		return(-1);
	}
	
	if (rate != exact_rate)
	{
		fprintf(stderr, "La frecuencia %d Hz no es soportada por tu hadware.\n \
						==> En su lugar usa %d Hz.\n", rate, exact_rate);
	}

	// Establecer numero de canales
	if (snd_pcm_hw_params_set_channels(pcm_handle, hwparams, 1) < 0)
	{
		fprintf(stderr, "Error estableciendo los canales.\n");
		return(-1);
	}

	//Establecer el número de períodos. Periodos usados son llamados fragmentos.
	if (snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0) < 0) 
	{
		fprintf(stderr, "Error estableciendo los periodos.\n");
		return(-1);
	}
	
	// Establecer el tamaño del búfer (en marcos). La latencia resultante está dada por
	// latency = periodsize * periods / (rate * bytes_per_frame)
	if (snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hwparams, (snd_pcm_uframes_t*)&exact_size) < 0)
	{
		fprintf(stderr, "Error estableciendo el tamanio del buffer.\n");
		return(-1);
	}
	
	if( size != exact_size )
	{
		fprintf(stderr, "El tamanio del Buffer %d no es soportado, En su lugar usar %d.\n", size, exact_size);
	}
	
	//Ajusta parametros de HW al dispositivo PCM y prepara el dispositivo
	if (snd_pcm_hw_params(pcm_handle, hwparams) < 0)
	{
		fprintf(stderr, "Error estableciendo los parametros de HW.\n");
		return(-1);
	}
	
	return 1;
}
//Consultamos la amplitud actual
float EntradaAudio::getAmplitud()
{
	return amplitud;	
}
//Consultamos frecuencia actual
float EntradaAudio::getFrecuencia()
{
	return frecuencia;	
}
