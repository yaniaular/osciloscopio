#ifndef ESCENA_H
#define ESCENA_H

#include <QGLWidget>
#include "EntradaAudio.h"

class Escena : public QGLWidget
{	
	Q_OBJECT

	public:
		float lados[4]; // arriba, abajo, derecha, izquierda;
		float target_side[4];
		float trigger[2];
		float trigger_width;
		int t_dir;
		EntradaAudio* ea;
		float tiempos[3];  // comienzo, parada, trigger
		int indices[3];
		float grosor;
		char mult_dt;
		float mult_volt;
		float posY,posX;
		float valores[100]; //valores de amplitud predefinidos

	private:
		int elapsed;
	
	public:
		Escena(QWidget *parent);
		void calcular_frecuencia();
		void setProjection() ;
		void drawPlot();
		void dibujar_rejillas();
		void setgrosor(float g);
		float getAmplitud();
		float getFrecuencia();
		void scale(int ax, float s);
		

	public slots:
		void animate();
		void detener();
		void setGrosor(int g);
		void setMult_dt(double valor);
		void setMult_volt(double valor);
		void setposY(int valor);
		void setposX(int valor);

	protected:
		void paintGL();
};

#endif
