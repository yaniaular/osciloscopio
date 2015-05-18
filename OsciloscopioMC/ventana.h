#ifndef VENTANA_H
#define VENTANA_H

#include <QtGui>
#include <QButtonGroup>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>
#include <qwt_knob.h>
#include <QSpinBox>
#include <QWidget>
#include "Escena.h"

class Ventana : public QWidget
{
    Q_OBJECT

	public:
		Escena *opengl_plot;
		QDoubleSpinBox *SpinBox_VoltDivs;
    	QDoubleSpinBox *SpinBox_TimeDivs;
		QSpinBox       *SpinBox_Grosor;
		QLabel *label_amplitud;
		QLabel *label_frecuencia;
		QLabel *Label_Periodo;
		QLCDNumber *lcd_amplitud;
		QLCDNumber *lcd_frecuencia;
		QLCDNumber *Lcd_Periodo;
		QPushButton *button_detener_continuar;
		QSlider *slider_pos_x;
		QLabel *label_pos_x;
		QSlider *slider_pos_y;
		QLabel *labe_pos_y;
		QLabel *label_ch1_volts_div;
		QLabel *label_ch1_time_divs;
		QLabel *Label_Intensidad;
		QFrame *line;
		QFrame *line_2;
		QFrame *line_3;
		QFrame *line_4;
		Ventana();
	
	public slots:
		void actualizar_fre_y_ampli();
};
#endif
