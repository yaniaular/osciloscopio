#include <QApplication>
#include "ventana.h"
#include "utilidades.h"

int main(int argc, char *argv[])
{
	//Inicializa la aplicacion
	QApplication app(argc, argv);
	//Crea la ventana	
	Ventana window;
	//Muestra la ventana por pantalla
	window.show();
	return app.exec();
}
