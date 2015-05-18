#ifndef UTILIDADES_H_
#define UTILIDADES_H_

#include <GL/glut.h>
#include <GL/gl.h>
#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>


typedef float Vector [4];

//Valor Absoluto
int ValAbs(float val)
{ 
	return val<0 ? -1 : 1;
}

//Devuelve el residuo de a+1/mod.
int rot(int a, int mod)
{
	return (a+1) % mod;
}

//Tiene que ver con los graficos, con el plot.
float snap(float val, float grid)
{
	float chop_val = (int)(val * 1000.0) / 1000.0; // Para evitar pequeños errores de punto flotante
	float snapped = (int)((chop_val/grid)+ValAbs(chop_val) * 0.5) * grid;
	return(snapped);
}

float afloor_snap(float val, float grid)
{
	float chop_val = (int)(val * 1000.0) / 1000.0;  // Para evitar pequeños errores de punto flotante
	float snapped = (int)((chop_val/grid)) * grid;
	return(snapped);
}

//retorna el menor
float min(float a, float b)
{
	return a>b ? b : a;
}

//retorna el mayor
float max(float a, float b)
{
	return a>b ? a : b;
}

//Le asigna valores a Vertex
void doVertex(Vector v)
{
	glVertex3f(v[0], v[1], v[2]);
}

//tiene que ver con la diferencia de tiempo
float timeDiff(timeval a, timeval b)
{
	return (float)(b.tv_sec - a.tv_sec) + ((float)(b.tv_usec - a.tv_usec) * .000001);
}

//Truncar valores reales a 4 cifras
float truncar(float num){
	num = num*10000;
	int aux_trun = num;
	return ((float)aux_trun)/(float)10000;
}

//Si dos valores son iguales retornar verdad, de lo contrario, retorna falso
float comparar(float x1, float x2){

	int y1=x1*(float)1000;
	int y2=x2*(float)1000;

	if(y1 == y2)
	return true;
	else
	return false;
}
#endif
