#include <GL/glut.h>
#include <GL/gl.h>
#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include "Escena.h"

Escena::Escena(QWidget *parent): QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	int i;
	float valor=0.0;

    elapsed = 0;
	t_dir = 1;
	lados[0] = 0.3;
	lados[1] = -0.3;
	lados[2] = 0;
	lados[3] = -0.1;
	
	for(i=0; i<4; i++ )
		target_side[i] = lados[i];

	trigger[0] = 0.0;
	trigger[1] = -0.05;
	trigger_width = 0.0002;

	ea = new EntradaAudio();
	grosor = 1;
	mult_dt=10;
	mult_volt=2.0;
	posY=posX=0;
	
	for(i=0;i<100;i++){
		valores[i] = valor;
		valor = valor + 0.005;
	}
}

void Escena::calcular_frecuencia() {
		
	double aux;
	double t_ini;
    tiempos[0] = ea->data_size * ea->dt;
    tiempos[1] = 0;
    ea->getTimeSpan(tiempos, indices, 3);
	
    aux=0;
    t_ini=0;

    ea->amplitud=0;
    int tiempo1=0;
	int tiempo2 =0;
	int contador=1;
	int k=0,band=1;

	//determinar la amplitud mayor   
	for( int i=indices[0]; i<indices[1]; i++ ){
		if(ea->amplitud<ea->float_data[ea->index(i)]){
			ea->amplitud = ea->float_data[ea->index(i)];
		}
	}

	ea->amplitud = truncar(ea->amplitud);

	//calcular frecuencia
	for( int i=indices[0]; i<indices[1]; i++ ) {
	if( comparar( ea->amplitud, truncar(ea->float_data[ea->index(i)]) ) ){
		if(tiempo1==0){
		tiempo1= ((i-indices[0])*ea->dt)*(float)1000000;
		}
		else{
			if(contador>10){			
			tiempo2 = ((i-indices[0])*ea->dt)*(float)1000000;
			ea->periodo=(float)(tiempo2-tiempo1) / (float)1000000;
			ea->frecuencia= (float)(1.0 /ea->periodo);			
	
			tiempo1=tiempo2;
			contador=0;	
			}
			else{
			tiempo1=tiempo2;contador=0;
			}	
		}		
	}
	contador+=1;
      }
			//buscar el valor exacto de la amplitud, con valores definidos
			while(k<100 && band){
				if(ea->amplitud < valores[k]){
					ea->amplitud = valores[k];
				band=0;				
				}
			k=k+1;			
			}
 
			if(ea->amplitud >= 0.225 && ea->amplitud<0.3)
				ea->amplitud = ea->amplitud-0.005;

			if(ea->amplitud >=  0.3 &&  ea->amplitud<0.370)
				ea->amplitud = ea->amplitud-0.01;

			if(ea->amplitud >=  0.37 &&  ea->amplitud<0.44)
				ea->amplitud = ea->amplitud-0.015;

			if(ea->amplitud >=  0.44)
			ea->amplitud = ea->amplitud-0.02;
}

void Escena::setMult_dt(double valor){
	valor=(float)2.5/(float)valor;
	mult_dt=valor;
}


void Escena::setMult_volt(double valor){
	mult_volt=(float)0.50/(float)valor;
}

void Escena::setposY(int valor){
	posY=(float)0.03*((float)valor/10.0);
	
}

void Escena::setposX(int valor){
	posX=(float)0.01*((float)valor/10.0);
	
}

void  Escena::setProjection()
{
	glLoadIdentity();
	glOrtho(lados[3], lados[2], lados[1], lados[0], -10, 10); 
}


//Se dibuja la forma de onda en la grafica
void  Escena::drawPlot()
{
	float dt = ea->dt;

	tiempos[0] = -lados[3];
	tiempos[1] = -lados[2];
	tiempos[2] = -trigger[1];
	ea->getTimeSpan(tiempos, indices, 3);

	if( t_dir != 0 )
	{
		int dx = 0;
		int tw = (int)(trigger_width / dt);
		
		if( t_dir < 0 )
		{
			while( dx < 5000 && ! (ea->float_data[ea->index(indices[2]+dx)] > trigger[0] && ea->float_data[ea->index(indices[2]+dx+tw)] < trigger[0] )  )
			{
				dx++;
			}
		}
		else
		{
			while( dx < 5000 && ! (ea->float_data[ea->index(indices[2]+dx)] < trigger[0] && ea->float_data[ea->index(indices[2]+dx+tw)] > trigger[0] )  )
			{
				dx++;
			}
		}
		
		if( dx == 1000 )
			dx = 0;
		
		
		indices[0] = indices[0] + dx;
		indices[0] =indices[0] - 1;
		tiempos[0] += dt;
		indices[1] += dx;
		indices[1]= indices[1] + 1;
		
	}
	
      
	int start = indices[0];
	
	glLineWidth(grosor); //Grosor de la onda
	glBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
	glEnable (GL_BLEND);
	glEnable (GL_ALPHA_TEST);
	glAlphaFunc (GL_ALWAYS, 0.5);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA);
	glDisable(GL_LIGHTING);
	glColor4f(0, 1.0, 0, 1); //Color de la onda
	glBegin(GL_LINE_STRIP);

	// perillas por defecto en volumen de captura  del dispositivo en 58%, las divisiones de voltage son de 0,1v = 1 mv
	//las perillas por defecto en volumen de captura del dispositivo en 58%, las disiones de tiempo son de 0,0025seg 

	float x =-0.1+posX;
	
	for( int i=start; i<=start+(4411/mult_dt); i++ )
	{
	glVertex2f( x , posY+0.0065+(ea->float_data[ea->index(i)]/10.75)*mult_volt );
	x += dt*mult_dt;
	}
	
	glEnd();

	glLineWidth(1.0);//Grosor de los ejes X y Y
	
	glColor4f(0, 1, 0, 0.3); //Color de los ejes
	glBegin(GL_LINES);
		glVertex2f(lados[3], trigger[0]);
		glVertex2f(lados[2], trigger[0]);
		glVertex2f(trigger[1], lados[0]);
		glVertex2f(trigger[1], lados[1]);
		glVertex2f(trigger[1]+trigger_width, lados[0]);
	glEnd();

	glColor4f(0.5, 0.5, 1, 0.5);

	glBegin(GL_LINES);
		glVertex2f(0, lados[0]);
		glVertex2f(0, lados[1]);
	glEnd();
}


void Escena::dibujar_rejillas(){	
	glLineWidth(1.0);
	glColor4f(1, 1, 1, 0.1);
	glBegin(GL_LINES);
	//Dibuja las lines verticales de la grilla			
	for( int i=-9; i<0; i++ ){
	glVertex2f(i*0.01, lados[0] < 1 ? lados[0] : 1);
	glVertex2f(i*0.01, lados[1] > -1 ? lados[1] : -1);
	}
	
	//Se dibujan las lineas horizontales de la grilla
	for( int i=-2; i<=2; i++ ){
	glVertex2f(lados[2] > 0 ? 0 : lados[2], i*0.10);
	glVertex2f(lados[3], i*0.10);
	}
	glEnd();	
}


void  Escena::setGrosor(int g)
{
	grosor = g;
}

float Escena::getAmplitud()
{ 
	return ea->getAmplitud();
}

float Escena::getFrecuencia()
{
	return ea->getFrecuencia();
}

//Glwidget
void Escena::animate()
{
    calcular_frecuencia();
    repaint();
}

void Escena::detener()
{
	ea->pause = ! ea->pause;
}

void Escena::paintGL()
{
	//Las imagenes en pantalla estan organizadas en una especie de matriz
	//Existe una pila de matrices para cada uno de los modos de matriz.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpia el buffer de imagenes en pantalla
	glClearColor( 0, 0, 0, 0.0 ); //Color fondo del osciloscopio

	glMatrixMode(GL_PROJECTION); //Orden en la matriz de las imagenes en pantalla
	glLoadIdentity(); //inicializar la matriz
	setProjection(); //Es un procedimiento que debe manipular la proyeccion de la matriz
	glMatrixMode(GL_MODELVIEW);//Orden en la matriz de las imagenes en pantalla
	//glPushMatrix();//realiza una copia de la matriz superior y
	//la pone encima de la pila, de tal forma que las dos matrices superiores son iguales
	glLoadIdentity();//inicializar la nueva matriz
	dibujar_rejillas();          
	drawPlot();
	glPopMatrix();//elimina la matriz superior, quedando en la parte
	//superior de la pila la matriz que estaba en el momento de
	//llamar a la función glPushMatrix().
	glFinish();
	
 }
 

