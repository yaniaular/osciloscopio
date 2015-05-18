#include "ventana.h"

Ventana::Ventana(): QWidget()
{	
	setFixedSize(712, 550); //Tamanio de la Ventana
	setWindowTitle(tr("OsciloscopioMC")); //Nombre ventana
	opengl_plot = new Escena(this);//Se agrega el plano cartesiano a la ventana
	opengl_plot->setGeometry(QRect(10, 10, 650, 381));
	
	//Etiqueta que corresponde a la funcion que muestra la amplitud
	label_amplitud = new QLabel("Amplitud (VPP)",this);
	label_amplitud->setObjectName(QString::fromUtf8("label_amplitud"));
	label_amplitud->setGeometry(QRect(15, 440, 100, 16));

	//Etiqueta que corresponde a la funcion que muestra la frecuencia
	label_frecuencia = new QLabel("Frecuencia (hz)",this);
	label_frecuencia->setObjectName(QString::fromUtf8("label_frecuencia"));
	label_frecuencia->setGeometry(QRect(14, 470, 96, 16));
	
	//Pantallita que muestra la amplitud actual de la onda
	lcd_amplitud = new QLCDNumber(this);
	lcd_amplitud->setObjectName(QString::fromUtf8("lcd_amplitud"));
	lcd_amplitud->setGeometry(QRect(130, 435, 64, 23));
	lcd_amplitud->setNumDigits(7);
	lcd_amplitud->setAutoFillBackground(true);

	//Pantallita que muestra la frecuencia actual de la onda	
	lcd_frecuencia = new QLCDNumber(this);
	lcd_frecuencia->setObjectName(QString::fromUtf8("lcd_frecuencia"));
	lcd_frecuencia->setGeometry(QRect(130, 465, 64, 23));
	lcd_frecuencia->setNumDigits(7);
	lcd_frecuencia->setAutoFillBackground(true);

	//Etiqueta que corresponde a la funcion que muestra el periodo
	Label_Periodo = new QLabel("Periodo (ms)",this);
    Label_Periodo->setObjectName(QString::fromUtf8("label"));
    Label_Periodo->setGeometry(QRect(30, 500, 82, 17));
	
	//Pantallita que muestra el periodo actual de la onda	
	Lcd_Periodo = new QLCDNumber(this);
    Lcd_Periodo->setObjectName(QString::fromUtf8("Lcd_Periodo"));
    Lcd_Periodo->setGeometry(QRect(130, 496, 64, 23));
	Lcd_Periodo->setNumDigits(7);
	Lcd_Periodo->setAutoFillBackground(true);

	//Se cambia los colores de las pantallitas con la informacion de la onda
	QPalette Pal = lcd_amplitud->palette();
	Pal.setColor(QPalette::Normal, QPalette::Window, Qt::black);
	lcd_amplitud->setPalette(Pal);
	lcd_frecuencia->setPalette(Pal);
	Lcd_Periodo->setPalette(Pal);
	
	//Se crea el boton que permite detener y animar la forma de onda
	button_detener_continuar = new QPushButton("Detener/Continuar",this);
	button_detener_continuar->setObjectName(QString::fromUtf8("button_detener_continuar"));
	button_detener_continuar->setGeometry(QRect(210, 480, 131, 25));

	//Se crean y configuran los slider respectivos que se encargan del movimiento
	//de lo onda a traves de los ejes X y Y
	slider_pos_x = new QSlider(this);
	slider_pos_x->setObjectName(QString::fromUtf8("slider_pos_x"));
	slider_pos_x->setGeometry(QRect(240, 400, 201, 20));
	slider_pos_x->setOrientation(Qt::Horizontal);
	slider_pos_x->setRange(-100, 100);
	slider_pos_x->setValue(0);
	label_pos_x = new QLabel("POS X",this);
	label_pos_x->setObjectName(QString::fromUtf8("label_pos_x"));
	label_pos_x->setGeometry(QRect(450, 400, 41, 16));
	slider_pos_y = new QSlider(this);
	slider_pos_y->setObjectName(QString::fromUtf8("slider_pos_y"));
	slider_pos_y->setGeometry(QRect(680, 80, 20, 201));
	slider_pos_y->setOrientation(Qt::Vertical);
	slider_pos_y->setRange(-100,100);
	slider_pos_y->setValue(0);
													
	//Creamos el SpinBox que permite cambiar la cantidad de voltios por division
	SpinBox_VoltDivs = new QDoubleSpinBox(this);
    SpinBox_VoltDivs->setObjectName(QString::fromUtf8("doubleSpinBox"));
    SpinBox_VoltDivs->setGeometry(QRect(420, 490, 71, 25));
	SpinBox_VoltDivs->setDecimals(3);
	SpinBox_VoltDivs->setRange(0.00625,0.50);
	SpinBox_VoltDivs->setValue(0.25);
	SpinBox_VoltDivs->setSingleStep(0.00625);
	
	//Creamos el SpinBox que permite cambiar la cantidad de tiempo por division
    SpinBox_TimeDivs = new QDoubleSpinBox(this);
    SpinBox_TimeDivs->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
    SpinBox_TimeDivs->setGeometry(QRect(575, 490, 71, 25));
	SpinBox_TimeDivs->setDecimals(3);
	SpinBox_TimeDivs->setRange(0.125,2.500);
	SpinBox_TimeDivs->setSingleStep(0.125);
	SpinBox_TimeDivs->setValue(1.250);
	
	//Creamos el SpinBox que permite cambiar la intensidad de la onda
	SpinBox_Grosor = new QSpinBox(this);
	SpinBox_Grosor->setRange(1,5);
	SpinBox_Grosor->setValue(1);
	SpinBox_Grosor->setGeometry(QRect(280, 510,  50, 27));
	
	//Etiquetas con los nombres de cada funcion
	labe_pos_y = new QLabel("POS Y",this);
	labe_pos_y->setObjectName(QString::fromUtf8("labe_pos_y"));
	labe_pos_y->setGeometry(QRect(670, 60, 41, 16));
	label_ch1_volts_div = new QLabel("CH1 VOLT/DIVS (v)",this);
	label_ch1_volts_div->setObjectName(QString::fromUtf8("label_ch1_volts_div"));
	label_ch1_volts_div->setGeometry(QRect(390, 470, 132, 20));
	label_ch1_time_divs = new QLabel("CH1 TIME/DIVS (ms)",this);
	label_ch1_time_divs->setObjectName(QString::fromUtf8("label_ch1_time_divs"));
	label_ch1_time_divs->setGeometry(QRect(545, 470, 140, 20));
	line = new QFrame(this);
	line->setObjectName(QString::fromUtf8("line"));
	line->setGeometry(QRect(0, 420, 751, 16));
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	line_2 = new QFrame(this);
	line_2->setObjectName(QString::fromUtf8("line_2"));
	line_2->setGeometry(QRect(360, 430, 20, 110));
	line_2->setFrameShape(QFrame::VLine);
	line_2->setFrameShadow(QFrame::Sunken);

	line_4 = new QFrame(this);
	line_4->setObjectName(QString::fromUtf8("line_4"));
	line_4->setGeometry(QRect(190, 430, 20, 110));
	line_4->setFrameShape(QFrame::VLine);
	line_4->setFrameShadow(QFrame::Sunken);

	QMetaObject::connectSlotsByName(this);	

	//Actualiza la forma de onda cada 5 microsegundos
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), opengl_plot, SLOT(animate()));
	timer->start(50);

	//Actualiza la frecuencia y la amplitud cada 2 segundos
	QTimer *timer2 = new QTimer(this);
	connect(timer2, SIGNAL(timeout()), this, SLOT(actualizar_fre_y_ampli()));
	timer2->start(2000);

	//Etiqueta del nombre de la funcion Grosor
	Label_Intensidad = new QLabel("Grosor",this);
	Label_Intensidad->setGeometry(QRect(230,517,43,16));

	//Se configuran los evento de boton y otras funciones del osciloscopioMC
	connect(SpinBox_Grosor, SIGNAL(valueChanged(int)), opengl_plot, SLOT(setGrosor(int)) );
	connect(SpinBox_TimeDivs,SIGNAL(valueChanged(double)), opengl_plot, SLOT(setMult_dt(double)) );
	connect(SpinBox_VoltDivs,SIGNAL(valueChanged(double)), opengl_plot, SLOT(setMult_volt(double)) );
	connect(slider_pos_y, SIGNAL(valueChanged(int)), opengl_plot, SLOT(setposY(int)));
	connect(slider_pos_x, SIGNAL(valueChanged(int)), opengl_plot, SLOT(setposX(int)));	
	connect(button_detener_continuar, SIGNAL(clicked()), opengl_plot, SLOT(detener()) );

}
//Nos permite actualizar los valores de la frecuencia y la amplitud en las pantallas
void Ventana::actualizar_fre_y_ampli()
{
	if(lcd_amplitud->value()!=opengl_plot->getAmplitud())	
		lcd_amplitud->display(opengl_plot->getAmplitud());

	if(lcd_frecuencia->value()!=opengl_plot->getFrecuencia()){
		lcd_frecuencia->display(opengl_plot->getFrecuencia());
		Lcd_Periodo->display(1/opengl_plot->getFrecuencia() );
	}
}
