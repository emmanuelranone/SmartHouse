#include <TimeLib.h>
#include <TouchScreen.h>
#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>
#include <stdint.h>
//#include <SoftwareSerial.h>

//Definir pines para controlar la pantalla
#define XP 9   //Pin de control digital
#define YP A2  //Pin anÃ¡logo de control
#define XM A3  //Pin anÃ¡logo de control
#define YM 8   //Pin digital de control
#define MINPRESSURE 10 // presion minima
#define MAXPRESSURE 1000 // presion maxima
#define BLANCO    0x0000
#define AMARILLO  0x001F
#define CIAN      0xF800
#define VERDE     0x07E0
#define ROJO      0x07FF
#define MAGENTA   0xF81F
#define AZUL      0xFFE0
#define NEGRO     0xFFFF

UTFTGLUE myGLCD(0x9481, A2, A1, A3, A4, A0);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

extern unsigned int luz[];
extern unsigned int despertador[];
extern unsigned int V2[];



int x,y,z;  //Variables para obtener las coordenadas X y Y donde se presiona la pantalla
short TS_MINX=70;//172;
short TS_MINY=125;//173;
short TS_MAXX=925;//966;
short TS_MAXY=985;//927; 

bool leerMenu=false;
bool leerPulsacionLuces = false;
bool leerPulsacionVentilador = false;
//bool leerPulsacionPorton = false;
bool leerPulsacionDespertador = false;
bool leerPulsacionAlarma = false;
bool leerPulsacionCambioClave = false;
bool leerPulsacionDesactivarD = false;
bool leerClave = false;
bool leerPulsacionSirena = false;
bool leerPulsacionConfig = false;
bool leerPulsacionConfigHora = false;
bool leerPulsacionConfigRed = false;
bool leerPulsacionConfigCelular = false;

bool pantalla0 = true; // pantalla bloqueada
bool pantalla1 = false; // menu principal
bool pantalla2 = false; // programar despertador
bool pantalla3 = false; // desactivar despertador
bool pantalla4 = false; // desactivar alarma despertador
bool pantalla5 = false; // cambio de clave
bool pantalla6 = false; // menu luces
bool pantalla7 = false; // menu ventiladores
bool pantalla8 = false; // menu porton
bool pantalla9 = false; // menu alarma
bool pantalla10 = false; // menu configuracion
bool pantalla11 = false; // config hora
bool pantalla12 = false; // config red
bool pantalla13 = false; // config celular
bool estadoAlarma = false; // alarma del despertador
bool estadoSirena = false; // sirena alarma proteccion
bool simulador = false;


int posicion = 0; 
int posiClave = 0;
int posicionX =0; 
int x1=0;
int connectionId;

bool state = false;
unsigned long interval = 5000;
unsigned long previousMillis;

bool state2 = false;
unsigned long interval2 = 10000;
unsigned long previousMillis2;

bool state3 = false;
unsigned long interval3 = 8000;
unsigned long previousMillis3;

String ssid, pass;
char clave[4] = {'1','2','3','4'}; 
char nuevaClave[8]{'0','0','0','0','0','0','0','0'}; 
char hora[4]; 
char horaActual[4];
time_t t;

//SoftwareSerial Serial1(19,18); //RX,TX

void setup() {	
	myGLCD.InitLCD(1);  //Inicia en modo horizontal 
	setTime(00, 00, 01, 01, 01, 2018);


	//MODULO WIFI
 Serial1.begin(115200);
	Serial.begin(115200);
	
	
	pantalla0 = true;

  // Serial1.println("Conectando a Wifi...\n");
   Serial.println("PRUEBA..");
   
	pinMode(41, OUTPUT); // sirena / alarma		
	pinMode(49, OUTPUT); // PATIO
	pinMode(51, OUTPUT); // PLANTA BAJA
  pinMode(53, OUTPUT); // PLANTA ALTA

  pinMode(46, INPUT);  // Sensor PIR
  
  pinMode(47, OUTPUT); // Ventilador Living   
  
  
	pinMode(44, OUTPUT); // Porton? PIR
  digitalWrite(44,1); // alimento el pir

  previousMillis = millis();
  previousMillis2 = millis();
  previousMillis3 = millis();
}



void webserver()

{    web("<!DOCTYPE HTML>");
     web("<html lang=\"es-ar\">"); //una pagina web necesita esto <HTML> y </HTML> es el inicio y fin del documento    
     web("<head> ");    //nombre de la pestaÃ±a que llevara la pagina
     web("<meta charset=\"utf-8\"/>");
     web("<title>Trabajo de Campo</title>");
     web("</head> "); 
     
     web("<body><h1> <FONT SIZE=\"5\" COLOR=\"red\">           PROYECTO CASA DOMOTICA </h1>"); //Titulo del inicio de la pagina
     web("<body><h2> <FONT SIZE=\"5\" COLOR=\"red\">  LISTA DE COMPONENTES  </h2>"); //
     web(" <BODY BGCOLOR=\"white\"> ");   

     web("<input type=\"button\" onclick=\"location.href='http://192.168.0.38/?pin=53' \" value=\"P.ALTA\" name=\"boton1\" style='width:180px; height:100px'/>");    
     web("<input type=\"button\" onclick=\"location.href='http://192.168.0.38/pin=51' \" value=\"P.BAJA\" name=\"boton2\" style='width:180px; height:100px'/>");
     web("<input type=\"button\" onclick=\"location.href='http://192.168.0.38/pin=49' \" value=\"PATIO\" name=\"boton6\" style='width:180px; height:100px'/>");
     
     web("<BR><BR><input type=\"button\" onclick=\"location.href='http://192.168.0.38/pin=47' \" value=\"VENTILADOR COMEDOR\" name=\"boton7\" style='width:180px; height:100px'/>");
    
     web("</html>");
    // cerrar();
     x1=1;

    }


void web(String text)

{Serial1.print("AT+CIPSEND=0,");  //Se establece una longitud de datos, en este caso, un mÃ¡ximo de 2048 bytes (2048 caracteres) que se enviarÃ¡n a la conexiÃ³n "0".
 Serial1.println(text.length());
 if (Serial1.find(">"))             // Si se recibe el mensaje
 {Serial1.println(text);
  Serial1.println(text);            //mandamos el mensaje por el wifi
  delay(10);
  while ( Serial1.available() > 0 )
  {if (  Serial1.find("SEND OK") )  //buscamos "ok" y luego salimos
   break;
  }
 }
}

void cerrar()
{
    //String clo = "AT+CIPCLOSE=0";
    //clo += connectionId;
    Serial1.println("AT+CIPCLOSE=0"); 
 
}


// ***************************************** NUMEROS EN PANTALLA *****************************************************
void llenarNumeros() {
	//Rectangulos para los numeros de la clave
	myGLCD.setColor(AMARILLO);
	myGLCD.drawRoundRect(30, 170, 100, 205);
	myGLCD.drawRoundRect(30, 215, 100, 250);
	myGLCD.drawRoundRect(110, 170, 180, 205);
	myGLCD.drawRoundRect(110, 215, 180, 250);
	myGLCD.drawRoundRect(190, 170, 260, 205);
	myGLCD.drawRoundRect(190, 215, 260, 250);
	myGLCD.drawRoundRect(270, 170, 340, 205);
	myGLCD.drawRoundRect(270, 215, 340, 250);
	myGLCD.drawRoundRect(350, 170, 420, 205);
	myGLCD.drawRoundRect(350, 215, 420, 250);

	//NÃºmeros en los rectÃ¡ngulos de la clave
	myGLCD.setTextSize(2);
	myGLCD.setTextColor(CIAN);
	myGLCD.print("0", 60, 180);
	myGLCD.print("1", 140, 180);
	myGLCD.print("2", 220, 180);
	myGLCD.print("3", 300, 180);
	myGLCD.print("4", 380, 180);
	myGLCD.print("5", 60, 225);
	myGLCD.print("6", 140, 225);
	myGLCD.print("7", 220, 225);
	myGLCD.print("8", 300, 225);
	myGLCD.print("9", 380, 225);
}


// ***************************************** EFECTO A LOS BOTONES *****************************************************
void agregarEfecto(int n, int o, int p, int q, int r, int s, int t)
{
	{
		myGLCD.setColor(AMARILLO);
		myGLCD.fillRoundRect(n, o, p, q);
		myGLCD.setTextSize(2);
		myGLCD.setTextColor(ROJO);
		myGLCD.printNumI(r, s, t);
		delay(150);
		myGLCD.setColor(NEGRO);
		myGLCD.fillRoundRect(n, o, p, q);
		myGLCD.setColor(AMARILLO);
		myGLCD.drawRoundRect(n, o, p, q);
		myGLCD.setTextColor(CIAN);
		myGLCD.printNumI(r, s, t);
		//return '0';                      
	}
}


// ***************************************** LEER PULSACION DEL PANEL *****************************************************
void lecturaPanel()
{
	TSPoint p = ts.getPoint(); // Realizamos lectura de las coordenadas


	pinMode(XM, OUTPUT); // La librerÃ­a utiliza estos pines como entrada y salida
	pinMode(YP, OUTPUT); // por lo que es necesario declararlos como salida justo
						 // despues de realizar una lectura de coordenadas.       
	x = map(p.x, TS_MINX, TS_MAXX, 0, 480);// + 60;//myGLCD.width()
	y = map(p.y, TS_MINY, TS_MAXY, 0, 350);// + 20;//myGLCD.height()
	z = p.z;
}



// ***************************************** VERIFICAR NUMERO PULSADO *****************************************************
char numeroPulsado()
{
	lecturaPanel();

	if (x >= 30 && x <= 100 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(30, 170, 100, 205, 0, 60, 180);
		return '0';
	}
	if (x >= 110 && x <= 180 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(110, 170, 180, 205, 1, 140, 180);
		return '1';
	}
	if (x >= 190 && x <= 260 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(190, 170, 260, 205, 2, 220, 180);
		return '2';
	}
	if (x >= 270 && x <= 340 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(270, 170, 340, 205, 3, 300, 180);
		return '3';
	}
	if (x >= 350 && x <= 420 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(350, 170, 420, 205, 4, 380, 180);
		return '4';
	}
	if (x >= 30 && x <= 100 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(30, 215, 100, 250, 5, 60, 225);
		return '5';
	}
	if (x >= 110 && x <= 180 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(110, 215, 180, 250, 6, 140, 225);
		return '6';
	}
	if (x >= 190 && x <= 260 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(190, 215, 260, 250, 7, 220, 225);
		return '7';
	}
	if (x >= 270 && x <= 340 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(270, 215, 340, 250, 8, 300, 225);
		return '8';
	}
	if (x >= 350 && x <= 420 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		agregarEfecto(350, 215, 420, 250, 9, 380, 225);
		return '9';
	}
	return numeroPulsado();
}

// ***************************************** LEER PULSACION MENU ALARMA *****************************************************
void leerPulsacionMenuAlarma() {
	lecturaPanel();
	if (x >= 40 && x <= 440 && y >= 270 && y <= 300 && (z > MINPRESSURE && z < MAXPRESSURE))  // MENU PRINCIPAL
	{
		leerPulsacionAlarma = false;
		delay(300);
		pantalla1 = true;
		return;
	}	
	if (x >= 90 && x <= 410 && y >= 115 && y <= 140 && (z > MINPRESSURE && z < MAXPRESSURE))  // activar simulador
	{
		if (simulador)
		{
			myGLCD.setTextSize(2);
			myGLCD.setColor(BLANCO);			
			myGLCD.fillRoundRect(90, 115, 410, 140);
			myGLCD.setColor(NEGRO);
			myGLCD.print("ACTIVAR SIMULADOR", 120, 120);
			myGLCD.fillRoundRect(245, 83, 400, 110);
			
			myGLCD.setColor(BLANCO);
			myGLCD.fillCircle(63, 128, 13);
			
			myGLCD.setColor(255, 0, 255);
			myGLCD.print("DESACTIVADO ", 250, 85);
			simulador = false;
			delay(200);
		}
		else
		{
			myGLCD.setTextSize(2);
			myGLCD.setColor(BLANCO);
			myGLCD.fillRoundRect(90, 115, 410, 140);
			myGLCD.setColor(NEGRO);
			myGLCD.print("DESACTIVAR SIMULADOR", 120, 120);
			myGLCD.fillRoundRect(245, 83, 400, 110);

			myGLCD.setColor(ROJO);
			myGLCD.fillCircle(63, 128, 6);
			
			myGLCD.setColor(255, 0, 255);
			myGLCD.print("ACTIVADO ", 250, 85);
			simulador = true;
			delay(200);
		}

	}
	
	if (x >= 50 && x <= 430 && y >= 185 && y <= 230 && (z > MINPRESSURE && z < MAXPRESSURE))  // activar alarma
	{
		if (estadoSirena)
		{
			myGLCD.setTextSize(3);
			myGLCD.setColor(BLANCO);
			myGLCD.fillRoundRect(50, 185, 430, 230);
			myGLCD.setTextSize(3);
			myGLCD.setColor(NEGRO);
			myGLCD.print("ACTIVAR ALARMA", 120, 200);
			
			estadoSirena = false;

			myGLCD.setTextSize(2);
			myGLCD.setColor(NEGRO);
			myGLCD.fillRoundRect(148, 54, 380, 84);
			myGLCD.setColor(255,0,255);
			myGLCD.print("DESACTIVADA", 150, 55);
			delay(200);
		}
		else
		{
			myGLCD.setTextSize(3);
			myGLCD.setColor(BLANCO);
			myGLCD.fillRoundRect(50, 185, 430, 230);
			myGLCD.setTextSize(3);
			myGLCD.setColor(NEGRO);
			myGLCD.print("DESACTIVAR ALARMA", 100, 200);
			
			//leerPulsacionAlarma = false;				

		
			myGLCD.setTextSize(2);
			myGLCD.setColor(NEGRO);
			myGLCD.fillRoundRect(148, 54, 380, 84);
			myGLCD.setColor(255, 0, 255);
			myGLCD.print("ACTIVADA", 150, 55);
			delay(200);
			estadoSirena = true;
			//pantalla1 = true;
		}
	}
}

// ***************************************** MENU ALARMA  *****************************************************
void menuAlarma() {
	leerPulsacionAlarma = true;

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);
	myGLCD.setColor(VERDE);
	myGLCD.fillRoundRect(40, 270, 440, 300);
	myGLCD.setTextSize(3);
	myGLCD.setColor(NEGRO);
	myGLCD.print("MENU PRINCIPAL", 120, 275);

	myGLCD.setColor(VERDE);
	myGLCD.drawRoundRect(40, 10, 440, 260);

	myGLCD.setTextSize(2);
	myGLCD.setColor(NEGRO);
	myGLCD.print("CONFIGURACION DE LA ALARMA", 90, 5);

	myGLCD.setBackColor(NEGRO);
	if (estadoSirena == false)			// estado de la alarma desactivada
	{
		myGLCD.setColor(BLANCO);
		myGLCD.print("ESTADO:", 50, 55);
		myGLCD.setColor(255,0,255);
		myGLCD.print("DESACTIVADA", 150, 55);

		myGLCD.setColor(BLANCO);
		myGLCD.fillRoundRect(50, 185, 430, 230);
		myGLCD.setTextSize(3);
		myGLCD.setColor(NEGRO);
		myGLCD.print("ACTIVAR ALARMA", 120, 200);
	}
	else								// estado alarma activada
	{
		myGLCD.setTextSize(2);
		myGLCD.setColor(BLANCO);
		myGLCD.print("ESTADO:", 50, 55);
		myGLCD.setColor(255,0,255);
		myGLCD.print("ACTIVADA", 150, 55);

		myGLCD.setColor(BLANCO);
		myGLCD.fillRoundRect(50, 185, 430, 230);
		myGLCD.setTextSize(3);
		myGLCD.setColor(NEGRO);
		myGLCD.print("DESACTIVAR ALARMA", 100, 200);
	}

	if (simulador == false)						// estado de simulador desactivado
	{
		myGLCD.setTextSize(2);
		myGLCD.setColor(BLANCO);
		myGLCD.print("SIMULADOR PRES:", 50, 85);
		myGLCD.setColor(255, 0, 255);
		myGLCD.print("DESACTIVADO ", 250, 85);

		myGLCD.setColor(BLANCO);
		myGLCD.fillCircle(63, 128, 13);
		myGLCD.fillRoundRect(90, 115, 410, 140);
		myGLCD.setColor(NEGRO);
		myGLCD.print("ACTIVAR SIMULADOR", 120, 120);
		
	}
	else										// estado de simulador activado
	{
		myGLCD.setTextSize(2);
		myGLCD.setColor(BLANCO);
		myGLCD.print("SIMULADOR PRES:", 50, 85);
		myGLCD.setColor(255, 0, 255);
		myGLCD.print("ACTIVADO ", 250, 85);
				
		myGLCD.setColor(BLANCO);
		myGLCD.fillCircle(63, 128, 13);
		myGLCD.setColor(ROJO);
		myGLCD.fillCircle(63, 128, 6);

		myGLCD.setColor(BLANCO);
		myGLCD.fillRoundRect(90, 115, 410, 140);
		myGLCD.setColor(NEGRO);
		myGLCD.print("DESACTIVAR SIMULADOR", 120, 120);
	}
	
}

// ***************************************** LEER PULSACION MENU INFORM *****************************************************
void leerPulsacionMenuConfigCelular()
{
	lecturaPanel();
	if (x >= 40 && x <= 440 && y >= 270 && y <= 300 && (z > MINPRESSURE && z < MAXPRESSURE))  // MENU PRINCIPAL
	{
		leerPulsacionConfigCelular = false;
		delay(300);
		pantalla1 = true;
		return;
	}
}


// ***************************************** MENU INFORM**********************************************************
void menuConfigCelular()
{
	leerPulsacionConfigCelular = true;

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);
	myGLCD.setColor(VERDE);
	myGLCD.fillRoundRect(40, 270, 440, 300);
	myGLCD.setTextSize(3);
	myGLCD.setColor(NEGRO);
	myGLCD.print("MENU PRINCIPAL", 120, 275);

	myGLCD.setColor(255, 0, 255);
	myGLCD.print("UNIVERSIDAD DE MORON", 60, 20);
	myGLCD.print("TRABAJO DE CAMPO", 90, 50);
	myGLCD.print("ALUMNO: EMMANUEL RANONE", 20, 110);
	myGLCD.print("MATRICULA: 3701-1615", 20, 140);
	myGLCD.print("13/02/2020", 20, 170);



}



// ***************************************** LEER PULSACION MENU CONFIG HORA ACTUAL *****************************************************
void leerPulsacionMenuConfigHora()
{
	if (posicion == 0) posicionX = 175;
	if (posicion == 1) posicionX = 200;
	if (posicion == 2) posicionX = 265;
	if (posicion == 3) posicionX = 290;

	lecturaPanel();

	if (posicion < 4) {
		if (x >= 30 && x <= 100 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(30, 170, 100, 205, 0, 60, 180);
			horaActual[posicion] = '0';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("0", posicionX, 105);
			posicion++;

			delay(350);
		}
		if (x >= 110 && x <= 180 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(110, 170, 180, 205, 1, 140, 180);
			horaActual[posicion] = '1';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("1", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 190 && x <= 260 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(190, 170, 260, 205, 2, 220, 180);
			horaActual[posicion] = '2';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("2", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 270 && x <= 340 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(270, 170, 340, 205, 3, 300, 180);
			horaActual[posicion] = '3';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("3", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 350 && x <= 420 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(350, 170, 420, 205, 4, 380, 180);
			horaActual[posicion] = '4';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("4", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 30 && x <= 100 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(30, 215, 100, 250, 5, 60, 225);
			horaActual[posicion] = '5';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("5", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 110 && x <= 180 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(110, 215, 180, 250, 6, 140, 225);
			horaActual[posicion] = '6';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("6", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 190 && x <= 260 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(190, 215, 260, 250, 7, 220, 225);
			horaActual[posicion] = '7';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("7", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 270 && x <= 340 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(270, 215, 340, 250, 8, 300, 225);
			horaActual[posicion] = '8';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("8", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 350 && x <= 420 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(350, 215, 420, 250, 9, 380, 225);
			horaActual[posicion] = '9';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("9", posicionX, 105);
			posicion++;
			delay(350);
		}
	}
	if (x >= 15 && x <= 215 && y >= 270 && y <= 310 && (z > MINPRESSURE && z < MAXPRESSURE))  //OK
	{
		if (posicion == 4)
		{			
			posicion = 0;
			leerPulsacionConfigHora = false;
			delay(300);

			int h1, h2, m1, m2;

			h1 = (horaActual[0] - 48) * 10;
			h2 = (horaActual[1] - 48);
			m1 = (horaActual[2] - 48) * 10;
			m2 = (horaActual[3] - 48);

			h1 = h1 + h2;
			m1 = m1 + m2;

			setTime(h1, m1, 01, 01, 01, 2018);
			
			pantalla10 = true;
			return;
		}
	}
	if (x >= 265 && x <= 465 && y >= 270 && y <= 310 && (z > MINPRESSURE && z < MAXPRESSURE))  // ATRAS
	{		
		//pantalla2 = false;
		posicion = 0;
		leerPulsacionConfigHora = false;
		delay(300);
		pantalla10 = true;
		return;
	}
}

// ***************************************** MENU CONF HORA ACTUAL **********************************************************
void menuConfigHora()
{
	leerPulsacionConfigHora = true;

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);
	myGLCD.setBackColor(NEGRO);
	myGLCD.setColor(ROJO);
	myGLCD.setTextSize(3);
	myGLCD.print("INGRESAR HORA ACTUAL", 35, 40);

	myGLCD.setColor(BLANCO);
	myGLCD.fillRect(160, 95, 230, 145);
	myGLCD.fillRect(250, 95, 320, 145);
	myGLCD.fillCircle(240, 115, 5);
	myGLCD.fillCircle(240, 130, 5);
	llenarNumeros();

	myGLCD.setColor(210, 206, 53);
	myGLCD.fillRoundRect(15, 270, 215, 310);	
	myGLCD.fillRoundRect(265, 270, 465, 310);
	// myGLCD.setBackColor(NEGRO);
	myGLCD.setColor(255, 0, 255);
	myGLCD.setTextSize(3);
	myGLCD.print("OK", 100, 280);
	myGLCD.print("ATRAS", 310, 280);
	


}



// ***************************************** LEER PULSACION MENU CONFIGURACION *****************************************************
void leerPulsacionMenuConfiguracion()
{
	lecturaPanel();
	if (x >= 40 && x <= 440 && y >= 270 && y <= 300 && (z > MINPRESSURE && z < MAXPRESSURE))  // MENU PRINCIPAL
	{
		leerPulsacionConfig= false;
		delay(300);
		pantalla1 = true;
		return;
	}
	if (x >= 40 && x <= 440 && y >= 50 && y <= 80 && (z > MINPRESSURE && z < MAXPRESSURE))  //CONFIG HORA
	{
		leerPulsacionConfig = false;
		delay(300);
		pantalla11 = true;
		return;
		
	}
	if (x >= 40 && x <= 440 && y >= 130 && y <= 160 && (z > MINPRESSURE && z < MAXPRESSURE))  //CAMBIAR CLAVE
	{
		leerPulsacionConfig = false;
		delay(300);
		pantalla5 = true;
		return;
	}
	
}

// ***************************************** MENU CONFIGURACION ***** *****************************************************
void menuConfiguracion()
{	
	leerPulsacionConfig = true;	

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);
	
	myGLCD.setColor(BLANCO);
	myGLCD.fillRoundRect(40, 50, 440, 80);
	myGLCD.fillRoundRect(40, 130, 440, 160);	

	myGLCD.setColor(VERDE);
	myGLCD.fillRoundRect(40, 270, 440, 300);
	myGLCD.setTextSize(3);	
	myGLCD.setColor(NEGRO);
	myGLCD.print("MENU PRINCIPAL", 120, 275);

	myGLCD.setBackColor(BLANCO);
	myGLCD.print("CONFIG. HORA", 80, 55);
	myGLCD.print("CAMBIAR CLAVE", 80, 135);
		
	/*Serial1.print("AT+CIPSTA?");
	delay(1);		
	char temp = Serial1.read();*/
	myGLCD.print("IP: ", 80, 215);

}



//***************************************** LEER PULSACION MENU LUCES *****************************************************
void leerPulsacionMenuLuces() {
	lecturaPanel();
	if (x >= 40 && x <= 440 && y >= 270 && y <= 300 && (z > MINPRESSURE && z < MAXPRESSURE))  // MENU PRINCIPAL
	{
		
		leerPulsacionLuces = false;
		delay(300);
		pantalla1 = true;
		return;
	}
	if (x >= 50 && x <= 120 && y >= 20 && y <= 90 && (z > MINPRESSURE && z < MAXPRESSURE))  // PLANTA ALTA
	{
		if (!digitalRead(53))
		{
			myGLCD.setColor(AMARILLO);
			myGLCD.fillCircle(85, 55, 35);
			digitalWrite(53, 1);
			delay(300);
		}
		else
		{
			myGLCD.setColor(ROJO);
			myGLCD.fillCircle(85, 55, 35);
			digitalWrite(53, 0);
			delay(300);
		}
	}
	if (x >= 200 && x <= 270 && y >= 20 && y <= 90 && (z > MINPRESSURE && z < MAXPRESSURE))  //PLANTA BAJA
	{
		if (!digitalRead(51))
		{
			myGLCD.setColor(AMARILLO);
			myGLCD.fillCircle(235, 55, 35);
			digitalWrite(51, 1);
			delay(300);
		}
		else
		{
			myGLCD.setColor(ROJO);
			myGLCD.fillCircle(235, 55, 35);
			digitalWrite(51, 0);
			delay(300);
		}
	}
	if (x >= 350 && x <= 420 && y >= 20 && y <= 90 && (z > MINPRESSURE && z < MAXPRESSURE))  // PATIO
	{
		if (!digitalRead(49))
		{
			myGLCD.setColor(AMARILLO);
			myGLCD.fillCircle(385, 55, 35);
			digitalWrite(49, 1);
			delay(300);
		}
		else
		{
			myGLCD.setColor(ROJO);
			myGLCD.fillCircle(385, 55, 35);
			digitalWrite(49, 0);
			delay(300);
		}
	}
	/*if (x >= 50 && x <= 120 && y >= 135 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))  // DORMITORIO 1
	{
		if (!digitalRead(47))
		{
			myGLCD.setColor(AMARILLO);
			myGLCD.fillCircle(85, 170, 35);
			digitalWrite(47, 1);
			delay(300);
		}
		else
		{
			myGLCD.setColor(ROJO);
			myGLCD.fillCircle(85, 170, 35);
			digitalWrite(47, 0);
			delay(300);
		}
	}
	if (x >= 200 && x <= 270 && y >= 135 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))  // DORMITORIO 2
	{
		if (!digitalRead(45))
		{
			myGLCD.setColor(AMARILLO);
			myGLCD.fillCircle(235, 170, 35);
			digitalWrite(45, 1);
			delay(300);
		}
		else
		{
			myGLCD.setColor(ROJO);
			myGLCD.fillCircle(235, 170, 35);
			digitalWrite(45, 0);
			delay(300);
		}
	}
	if (x >= 350 && x <= 420 && y >= 135 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))  // TOCADOR
	{
		if (!digitalRead(43))
		{
			myGLCD.setColor(AMARILLO);
			myGLCD.fillCircle(385, 170, 35);
			digitalWrite(43, 1);
			delay(300);
		}
		else
		{
			myGLCD.setColor(ROJO);
			myGLCD.fillCircle(385, 170, 35);
			digitalWrite(43, 0);
			delay(300);
		}
	}*/
}


// ***************************************** MANEJO DE LUCES *****************************************************
void menuLuces() {
	leerPulsacionLuces = true;

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);
	myGLCD.setColor(VERDE);
	myGLCD.fillRoundRect(40, 270, 440, 300);
	myGLCD.setTextSize(3);
	myGLCD.setColor(NEGRO);
	myGLCD.print("MENU PRINCIPAL", 120, 275);

	//  CIRCULOS ROJOS SI ESTA APAGADO
	myGLCD.setColor(ROJO);
	if (!digitalRead(53))  myGLCD.fillCircle(85, 55, 35); // COMEDOR
	if (!digitalRead(51))  myGLCD.fillCircle(235, 55, 35); // LIVING
	if (!digitalRead(49))  myGLCD.fillCircle(385, 55, 35); // PASILLO
	/*if (!digitalRead(47))  myGLCD.fillCircle(85, 170, 35); // DORMIT 1
	if (!digitalRead(45))  myGLCD.fillCircle(235, 170, 35); // DORMIT 2
	if (!digitalRead(43))  myGLCD.fillCircle(385, 170, 35); // TOCADOR*/

	 //  CIRCULOS AMARILLOS SI ESTA PRENDIDO
	myGLCD.setColor(AMARILLO);
	if (digitalRead(53))  myGLCD.fillCircle(85, 55, 35);
	if (digitalRead(51))  myGLCD.fillCircle(235, 55, 35);
	if (digitalRead(49))  myGLCD.fillCircle(385, 55, 35);
	/*if (digitalRead(47))  myGLCD.fillCircle(85, 170, 35);
	if (digitalRead(45))  myGLCD.fillCircle(235, 170, 35);
	if (digitalRead(43))  myGLCD.fillCircle(385, 170, 35);*/

	myGLCD.setColor(255, 255, 0);
	myGLCD.drawRect(50, 20, 120, 90);
	myGLCD.drawRect(200, 20, 270, 90);
	myGLCD.drawRect(350, 20, 420, 90);
	/*myGLCD.drawRect(50, 135, 120, 205);
	myGLCD.drawRect(200, 135, 270, 205);
	myGLCD.drawRect(350, 135, 420, 205);*/

	myGLCD.setTextSize(2);
	myGLCD.setColor(255, 0, 255);
	myGLCD.setBackColor(NEGRO);
	myGLCD.print("P. BAJA", 50, 100);
	myGLCD.print("1er PISO", 200, 100);
	myGLCD.print("PATIO", 350, 100);
	/*myGLCD.print("Dormit 1", 50, 215);
	myGLCD.print("Dormit 2", 200, 215);
	myGLCD.print("Tocador", 350, 215);*/
		
}



//***************************************** LEER PULSACION MENU VENTILADORES ***********************************************
void leerPulsacionMenuVentiladores() {
	lecturaPanel();
	if (x >= 40 && x <= 440 && y >= 270 && y <= 300 && (z > MINPRESSURE && z < MAXPRESSURE))  // MENU PRINCIPAL
	{		
		leerPulsacionVentilador = false;
		delay(300);
		pantalla1 = true;
		return;
	}
	/*if (x >= 200 && x <= 270 && y >= 135 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))  // COMEDOR
	{
		if (!digitalRead(52))
		{
			myGLCD.setColor(AMARILLO);
			myGLCD.fillCircle(235, 170, 35);
			digitalWrite(52, 1);
			delay(300);
		}
		else
		{
			myGLCD.setColor(ROJO);
			myGLCD.fillCircle(235, 170, 35);
			digitalWrite(52, 0);
			delay(300);
		}
	}*/
	if (x >= 200 && x <= 270 && y >= 20 && y <= 90 && (z > MINPRESSURE && z < MAXPRESSURE))  //LIVING
	{
		if (!digitalRead(47))
		{
			myGLCD.setColor(AMARILLO);
			myGLCD.fillCircle(235, 55, 35);
			digitalWrite(47, 1);
			delay(300);
		}
		else
		{
			myGLCD.setColor(ROJO);
			myGLCD.fillCircle(235, 55, 35);
			digitalWrite(47, 0);
			delay(300);
		}
	}

}

// ***************************************** MANEJO DE VENTILADORES *****************************************************
void menuVentiladores() {
	leerPulsacionVentilador = true;

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);
	myGLCD.setColor(VERDE);
	myGLCD.fillRoundRect(40, 270, 440, 300);
	myGLCD.setTextSize(3);
	myGLCD.setColor(NEGRO);
	myGLCD.print("MENU PRINCIPAL", 120, 275);

	//  CIRCULOS ROJOS SI ESTA APAGADO
	myGLCD.setColor(ROJO);
	if (!digitalRead(50))  myGLCD.fillCircle(235, 55, 35); // LIVING
	//if (!digitalRead(52))  myGLCD.fillCircle(235, 170, 35); // COMEDOR


	 //  CIRCULOS AMARILLOS SI ESTA PRENDIDO
	myGLCD.setColor(AMARILLO);
	if (digitalRead(50))  myGLCD.fillCircle(235, 55, 35);
	//if (digitalRead(52))  myGLCD.fillCircle(235, 170, 35);

	myGLCD.setColor(255, 255, 0);
	myGLCD.drawRect(200, 20, 270, 90);
	//myGLCD.drawRect(200, 135, 270, 205);

	myGLCD.setTextSize(2);
	myGLCD.setColor(255, 0, 255);
	myGLCD.setBackColor(NEGRO);
	myGLCD.print("Ventilador 1er Piso", 130, 100);
	//myGLCD.print("Ventilador Comedor", 130, 215);	

}



// ***************************************** PANTALLA CAMBIO DE CLAVE *****************************************************
void pantallaCambioDeClave() {
	leerPulsacionCambioClave = true;

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);
	myGLCD.setBackColor(NEGRO);
	myGLCD.setColor(ROJO);
	myGLCD.setTextSize(3);
	myGLCD.print("CAMBIAR CLAVE", 130, 40);

	myGLCD.setColor(BLANCO);
	myGLCD.fillRect(80, 95, 220, 145);
	myGLCD.fillRect(240, 95, 380, 145);
	llenarNumeros();

	myGLCD.setColor(210, 206, 53);
	myGLCD.fillRoundRect(30, 270, 220, 310);
	myGLCD.fillRoundRect(230, 270, 420, 310);

	myGLCD.setColor(255, 0, 255);
	myGLCD.setTextSize(3);
	myGLCD.print("CAMBIAR", 60, 280);
	myGLCD.print("CANCELAR", 250, 280);

}

// ***************************************** CAMBIAR CLAVE *****************************************************
void cambiarClave() {
	if (posiClave == 0) posicionX = 100;
	if (posiClave == 1) posicionX = 125;
	if (posiClave == 2) posicionX = 150;
	if (posiClave == 3) posicionX = 175;
	if (posiClave == 4) posicionX = 260;
	if (posiClave == 5) posicionX = 285;
	if (posiClave == 6) posicionX = 310;
	if (posiClave == 7) posicionX = 335;


	lecturaPanel();

	if (posiClave < 8) {
		if (x >= 30 && x <= 100 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(30, 170, 100, 205, 0, 60, 180);
			nuevaClave[posiClave] = '0';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("0", posicionX, 105);
			posiClave++;

			delay(350);
		}
		if (x >= 110 && x <= 180 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(110, 170, 180, 205, 1, 140, 180);
			nuevaClave[posiClave] = '1';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("1", posicionX, 105);
			posiClave++;
			delay(350);
		}
		if (x >= 190 && x <= 260 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(190, 170, 260, 205, 2, 220, 180);
			nuevaClave[posiClave] = '2';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("2", posicionX, 105);
			posiClave++;
			delay(350);
		}
		if (x >= 270 && x <= 340 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(270, 170, 340, 205, 3, 300, 180);
			nuevaClave[posiClave] = '3';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("3", posicionX, 105);
			posiClave++;
			delay(350);
		}
		if (x >= 350 && x <= 420 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(350, 170, 420, 205, 4, 380, 180);
			nuevaClave[posiClave] = '4';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("4", posicionX, 105);
			posiClave++;
			delay(350);
		}
		if (x >= 30 && x <= 100 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(30, 215, 100, 250, 5, 60, 225);
			nuevaClave[posiClave] = '5';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("5", posicionX, 105);
			posiClave++;
			delay(350);
		}
		if (x >= 110 && x <= 180 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(110, 215, 180, 250, 6, 140, 225);
			nuevaClave[posiClave] = '6';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("6", posicionX, 105);
			posiClave++;
			delay(350);
		}
		if (x >= 190 && x <= 260 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(190, 215, 260, 250, 7, 220, 225);
			nuevaClave[posiClave] = '7';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("7", posicionX, 105);
			posiClave++;
			delay(350);
		}
		if (x >= 270 && x <= 340 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(270, 215, 340, 250, 8, 300, 225);
			nuevaClave[posiClave] = '8';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("8", posicionX, 105);
			posiClave++;
			delay(350);
		}
		if (x >= 350 && x <= 420 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(350, 215, 420, 250, 9, 380, 225);
			nuevaClave[posiClave] = '9';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("9", posicionX, 105);
			posiClave++;
			delay(350);
		}
	}
	if (x >= 30 && x <= 220 && y >= 270 && y <= 310 && (z > MINPRESSURE && z < MAXPRESSURE))  // BOTON CAMBIAR CLAVE
	{
		if (posiClave == 8)
		{
			if (nuevaClave[0] == nuevaClave[4] && nuevaClave[1] == nuevaClave[5] && nuevaClave[2] == nuevaClave[6] && nuevaClave[3] == nuevaClave[7])
			{
				clave[0] = nuevaClave[0]; clave[1] = nuevaClave[1]; clave[2] = nuevaClave[2]; clave[3] = nuevaClave[3];
				// aca deberia guardar clabe en rom
				posiClave = 0;
				leerPulsacionCambioClave = false;
				pantalla1 = true;
				return;
			}
		}
	}
	if (x >= 230 && x <= 420 && y >= 270 && y <= 310 && (z > MINPRESSURE && z < MAXPRESSURE))  // BOTON CANCELAR
	{		
		posiClave = 0;
		leerPulsacionCambioClave = false;
		pantalla1 = true;
		return;
	}


}




// ***************************************** COMPARA HORA CON LA DEL DESPERTADOR *****************************************************
void comparaHora() {
	int h1, h2, m1, m2;

	h1 = (hora[0] - 48) * 10;
	h2 = (hora[1] - 48);
	m1 = (hora[2] - 48) * 10;
	m2 = (hora[3] - 48);

	h1 = h1 + h2;
	m1 = m1 + m2;

	if (h1 == hour(t))
		if (m1 == minute(t))
		{
			estadoAlarma = false;
			digitalWrite(41, 1); 
			pantalla3 = true;			
			return;
		}
}

// ***************************************** LEER PULSACIONES DE PROGRAMAR DESPERTADOR *****************************************************
void leerPulsacionProgramarDespertador() {
	if (posicion == 0) posicionX = 175;
	if (posicion == 1) posicionX = 200;
	if (posicion == 2) posicionX = 265;
	if (posicion == 3) posicionX = 290;

	lecturaPanel();

	if (posicion < 4) {
		if (x >= 30 && x <= 100 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(30, 170, 100, 205, 0, 60, 180);
			hora[posicion] = '0';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("0", posicionX, 105);
			posicion++;

			delay(350);
		}
		if (x >= 110 && x <= 180 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(110, 170, 180, 205, 1, 140, 180);
			hora[posicion] = '1';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("1", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 190 && x <= 260 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(190, 170, 260, 205, 2, 220, 180);
			hora[posicion] = '2';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("2", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 270 && x <= 340 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(270, 170, 340, 205, 3, 300, 180);
			hora[posicion] = '3';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("3", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 350 && x <= 420 && y >= 170 && y <= 205 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(350, 170, 420, 205, 4, 380, 180);
			hora[posicion] = '4';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("4", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 30 && x <= 100 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(30, 215, 100, 250, 5, 60, 225);
			hora[posicion] = '5';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("5", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 110 && x <= 180 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(110, 215, 180, 250, 6, 140, 225);
			hora[posicion] = '6';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("6", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 190 && x <= 260 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(190, 215, 260, 250, 7, 220, 225);
			hora[posicion] = '7';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("7", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 270 && x <= 340 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(270, 215, 340, 250, 8, 300, 225);
			hora[posicion] = '8';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("8", posicionX, 105);
			posicion++;
			delay(350);
		}
		if (x >= 350 && x <= 420 && y >= 215 && y <= 250 && (z > MINPRESSURE && z < MAXPRESSURE))
		{
			agregarEfecto(350, 215, 420, 250, 9, 380, 225);
			hora[posicion] = '9';
			myGLCD.setColor(NEGRO);
			myGLCD.setBackColor(BLANCO);
			myGLCD.setTextSize(4);
			myGLCD.print("9", posicionX, 105);
			posicion++;
			delay(350);
		}
	}
	if (x >= 15 && x <= 155 && y >= 270 && y <= 310 && (z > MINPRESSURE && z < MAXPRESSURE))  // MENU PRINCIPAL
	{
		posicion = 0;
		leerPulsacionDespertador = false;
		delay(300);
		pantalla1 = true;
		return;
	}
	if (x >= 170 && x <= 310 && y >= 270 && y <= 310 && (z > MINPRESSURE && z < MAXPRESSURE))  // ACTIVAR ALARMA
	{
		if (posicion == 4)
		{
			estadoAlarma = true;			
			posicion = 0;
			leerPulsacionDespertador = false;
			delay(300);
			pantalla1 = true;
			
			return;
		}
	}
	if (x >= 325 && x <= 465 && y >= 270 && y <= 310 && (z > MINPRESSURE && z < MAXPRESSURE))  // CANCELAR ALARMA
	{
		estadoAlarma = false;
		pantalla2 = false;
		posicion = 0;
		leerPulsacionDespertador = false;
		delay(300);
		pantalla1 = true;
		return;
	}
}

// ***************************************** PROGRAMAR DESPERTADOR *****************************************************
void programarDespertador() {
	leerPulsacionDespertador = true;

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);
	myGLCD.setBackColor(NEGRO);
	myGLCD.setColor(ROJO);
	myGLCD.setTextSize(3);
	myGLCD.print("PROGRAMAR DESPERTADOR", 45, 40);

	myGLCD.setColor(BLANCO);
	myGLCD.fillRect(160, 95, 230, 145);
	myGLCD.fillRect(250, 95, 320, 145);
	myGLCD.fillCircle(240, 115, 5);
	myGLCD.fillCircle(240, 130, 5);
	llenarNumeros();

	myGLCD.setColor(210, 206, 53);
	myGLCD.fillRoundRect(15, 270, 155, 310);
	myGLCD.fillRoundRect(170, 270, 310, 310);
	myGLCD.fillRoundRect(325, 270, 465, 310);
	// myGLCD.setBackColor(NEGRO);
	myGLCD.setColor(255, 0, 255);
	myGLCD.setTextSize(3);
	myGLCD.print("ATRAS", 40, 280);
	myGLCD.print("OK", 220, 280);
	myGLCD.print("CANCEL", 340, 280);

}



// ***************************************** DESACTIVAR DESPERTADOR *****************************************************
void desactivarDespertador() {
	leerPulsacionDesactivarD = true;

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);

	myGLCD.setColor(CIAN);
	myGLCD.fillRoundRect(80, 100, 410, 220);
	myGLCD.setBackColor(CIAN);
	myGLCD.setTextSize(4);
	myGLCD.setColor(NEGRO);
	myGLCD.print("DESACTIVAR", 120, 120);
	myGLCD.print("DESPERTADOR", 105, 170);
	   
}

// ***************************************** VERIFICAR SI PULSO DESACTIVARDESPERTADOR *****************************************************
void pulsoDesactivarD() {
	lecturaPanel();
	if (x >= 80 && x <= 410 && y >= 100 && y <= 220 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		leerPulsacionDesactivarD = false;		
		digitalWrite(41, 0); // apago la cancion mp3
		delay(300);
		pantalla1 = true;
		estadoAlarma = false;
		return;
	}
}

// ***************************************** VERIFICAR SI PULSO DESACTIVAR SIRENA *****************************************************
void pulsoDesactivarS() {
	lecturaPanel();
	if (x >= 80 && x <= 410 && y >= 100 && y <= 220 && (z > MINPRESSURE && z < MAXPRESSURE))
	{
		digitalWrite(41, 0); // apago la alarma
		leerPulsacionSirena = false;	
		leerPulsacionAlarma = false;
		delay(300);
		pantalla1 = true;		
		return;
	}
}

// ***************************************** PANTALLA DESACTIVAR ALARMA *****************************************************
void desactivarAlarma() {
	estadoSirena = false;
	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);

	myGLCD.setColor(CIAN);
	myGLCD.fillRoundRect(80, 100, 410, 220);
	myGLCD.setBackColor(CIAN);
	myGLCD.setTextSize(4);
	myGLCD.setColor(NEGRO);
	myGLCD.print("DESACTIVAR", 120, 120);
	myGLCD.print("ALARMA", 135, 170);

	leerPulsacionSirena = true;
}



// *****************************************  LECTURA DE PULSACIONES DEL MENU PRINCIPAL *****************************************************
void lecturaMenuPrincipal() {
	lecturaPanel();
	if (x >= 70 && x <= 170 && y >= 20 && y <= 120 && (z > MINPRESSURE && z < MAXPRESSURE))  // MENU LUCES
	{		
		leerMenu = false;
		pantalla6 = true;
		return;
	}
	if (x >= 190 && x <= 290 && y >= 20 && y <= 120 && (z > MINPRESSURE && z < MAXPRESSURE))  //MENU VENTILADORES
	{		
		leerMenu = false;
		pantalla7 = true;
		return;
	}
	if (x >= 310 && x <= 410 && y >= 20 && y <= 120 && (z > MINPRESSURE && z < MAXPRESSURE))  //DESPERTADOR
	{		
		leerMenu = false;
		pantalla2 = true;
		return;
	}
	if (x >= 70 && x <= 170 && y >= 140 && y <= 240 && (z > MINPRESSURE && z < MAXPRESSURE))  //CONFIGURAR
	{
		leerMenu = false;		
		pantalla10 = true;
		return;
	}
	if (x >= 190 && x <= 290 && y >= 140 && y <= 240 && (z > MINPRESSURE && z < MAXPRESSURE))  //PROGRAMAR ALARMA
	{
		leerMenu = false;
		pantalla9 = true;		
		return;
	}
	if (x >= 310 && x <= 410 && y >= 140 && y <= 240 && (z > MINPRESSURE && z < MAXPRESSURE))  //INFORMACION
	{	
		leerMenu = false;
		pantalla13 = true;
		return;
	}
	/*if (x >= 70 && x <= 410 && y >= 280 && y <= 310 && (z > MINPRESSURE && z < MAXPRESSURE))  //PORTON
	{				
		leerMenu = false;
		pantalla8 = true;
		return;
	}*/
	if (x >= 460 && x <= 480 && y >= 5 && y <= 25 && (z > MINPRESSURE && z < MAXPRESSURE))  //BLOQUEAR PANTALLA
	{
		leerMenu = false;
		pantalla0 = true;
		return;
	}
}
// ***************************************** MENU PRINCIPAL *****************************************************
void menuPrincipal() {
	leerMenu = true;
	myGLCD.setTextSize(3);

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);

	myGLCD.setColor(AZUL);
	myGLCD.fillRoundRect(460, 5, 480, 25);

	//Rectangulos para los elementos del menu principal
	myGLCD.setColor(VERDE);

	//myGLCD.drawRoundRect(70, 20, 170, 120);
	myGLCD.drawBitmap(70, 20, 100, 100, luz); // luces

	//myGLCD.drawRoundRect(190, 20, 290, 120);
	myGLCD.drawBitmap(190, 20, 100, 100, V2); // vent1
	
	//myGLCD.drawRoundRect(310, 20, 410, 120);
	myGLCD.drawBitmap(310, 20, 100, 100, despertador); // alarma

	
	
	myGLCD.setColor(VERDE);
	myGLCD.drawRoundRect(70, 140, 170, 240);
	myGLCD.drawRoundRect(190, 140, 290, 240);
	myGLCD.drawRoundRect(310, 140, 410, 240);

	myGLCD.setBackColor(NEGRO);
	myGLCD.setColor(AMARILLO);
	myGLCD.setTextSize(2);

	myGLCD.print("CONFIG", 88, 180);
	myGLCD.print("ALARMA", 205, 180);
	myGLCD.print("INFORM", 322, 180);
	
}



// ***************************************** LLENAR PANTALLA CLAVE  *****************************************************
void llenarPantallaClave() {
	leerClave = true;

	myGLCD.setColor(NEGRO);
	myGLCD.fillRect(0, 0, 480, 320);
	myGLCD.setBackColor(NEGRO);
	myGLCD.setColor(ROJO);
	myGLCD.setTextSize(3);
	myGLCD.print("INGRESE CLAVE", 130, 40);
	llenarNumeros();
}

void verificarClave() {
	bool igual = true;
	char clave2[4];

	clave2[0] = numeroPulsado();
	delay(350);

	clave2[1] = numeroPulsado();
	delay(350);

	clave2[2] = numeroPulsado();
	delay(350);

	clave2[3] = numeroPulsado();
	delay(350);

	if (clave2[0] == clave[0] && clave2[1] == clave[1] && clave2[2] == clave[2] && clave2[3] == clave[3])
	{
		leerClave = false;
		pantalla1 = true;
	}
	else
	{
		clave2[0] = 'a'; clave2[1] = 'a'; clave2[2] = 'a'; clave2[3] = 'a';
		myGLCD.setBackColor(NEGRO);
		myGLCD.setColor(ROJO);
		myGLCD.setTextSize(3);
		myGLCD.print("CLAVE INCORRECTA", 128, 40);
		delay(200);
		llenarPantallaClave();
	}
}



// ***************************************** PANTALLA BLOQUEADA *****************************************************
void pantallaBloqueada() {
	llenarPantallaClave();
}

// ***************************************** SIMULADOR DE PRES *****************************************************

void simuladorPres()
{
	state = !state;
	if (state) 
		digitalWrite(51, 1);
	if (!state) 
		digitalWrite(51, 0);
}
void simuladorPres2()
{
	state2 = !state2;
	if (state2)
		digitalWrite(53, 1);
	if (!state2)
		digitalWrite(53, 0);
}
void simuladorPres3()
{
	state3 = !state3;
	if (state3)
		digitalWrite(49, 1);
	if (!state3)
		digitalWrite(49, 0);
}


void loop() {
	t = now();

	unsigned long currentMillis = millis();
	unsigned long currentMillis2 = millis();
	unsigned long currentMillis3 = millis();
	if (simulador)
	{
		if ((unsigned long)(currentMillis - previousMillis) >= interval)
		{
			simuladorPres();
			previousMillis = millis();
		}
		if ((unsigned long)(currentMillis2 - previousMillis2) >= interval2)
		{
			simuladorPres2();
			previousMillis2 = millis();
		}
		if ((unsigned long)(currentMillis3 - previousMillis3) >= interval3)
		{
			simuladorPres3();
			previousMillis3 = millis();
		}
	}
	 // *************************** MODULO WIFI ***************************
 
 if (Serial1.available() > 0) //Esto basicamente revisa si hay datos recibidos o que ya estaban almacenados en el mÃ³dulo wifi. Si hay datos (>0), entonces que proceda con el cÃ³digo siguiente.
    {
        char c = Serial1.read(); // Se declara una variable de tipo caracter ("c") que va a almacenar el dato leÃ­do por el mÃ³dulo.
		Serial.print(c);
        if (c == 71) // Esto quiere decir, que si el dato que lee es igual a 71 (es decir, la letra G en lenguaje ASCII) significa que busca detectar un "GET" que envia el navegador cuando intentamos abrirlo
        {
           // Serial.println("Se ha enviado la peticiÃ³n"); // Que se imprima en el serial que se ha enviado la peticiÃ³n correctamente
            delay(3000); // Se esperan 3000 milisegundos, o sea, 3 segundos.
            webserver(); // Que ejecute la funciÃ³n llamada "webserver"
        }
    }  

    if(x1==1)
    {
        if(Serial1.find("+IPD,")) //Se revisa si el servidor recibiÃ³ datos
        {
            delay(1000); // Espera 1 segundo para recibir esos datos
            connectionId = Serial1.read()-48; //esto es porque la funciÃ³n "read" regresa un valor decimal ASCII y el 0 empieza en 48.
            
            Serial1.find("pin="); //Detecta el pin que se presionarÃ¡ con el botÃ³n en el cÃ³digo en HTML 'http://192.168.15.151/pin=13'
            int pinNumber = (Serial1.read()-48)*10; //Este obtiene el primer numero del pin. Por ejemplo, si es el pin 13, entonces el primer numero es 1 y se multiplica por 10, para obtener 10.
            pinNumber += (Serial1.read()-48); // Aqui se obtiene el segundo numero, si fuera el  pin 13, entonces el segundo nÃºmero es 3 y se aÃ±ade al primer numero ^

           // if (pinNumber==41)
          //  {
                digitalWrite(pinNumber, !digitalRead(pinNumber)); //Mandar el comando al pin designado
                //webserver();
                Serial1.print("inviertiendo pin ");
                Serial1.println(pinNumber);                               
          //  }
            //cleanWiFiBuffer();
            if (pinNumber==50)
            {
              Serial1.end();
            }
        }       
    }
      
	// *************************** ****************************************

	if (estadoAlarma == true)		//verifica si es la hora del despertador 
		comparaHora();

	if (estadoSirena == true)
	{		
		int value = digitalRead(46);
		if (value == HIGH)
		{
			digitalWrite(41, 1);
			desactivarAlarma();			
		}		
	}

	// ************************** VERIFICAR LAS PULSACIONES DE LA PANTALLA TACTIL ******************

	if (leerMenu == true)			// lectura de pulsaciones del menu principal
		lecturaMenuPrincipal();

	if (leerPulsacionLuces == true)			// lectura de pulsaciones del menu luces
		leerPulsacionMenuLuces();

	if (leerPulsacionVentilador == true)			// lectura de pulsaciones del menu ventiladores
		leerPulsacionMenuVentiladores();

	/*if (leerPulsacionPorton == true)			// lectura de pulsaciones del menu porton
		leerPulsacionMenuPorton();*/

	if (leerPulsacionDespertador == true)		// lectura de pulsaciones de programar despertador
		leerPulsacionProgramarDespertador();
	
	if (leerPulsacionDesactivarD == true)		// lectura de pulsaciones de programar despertador
		pulsoDesactivarD();

	if (leerPulsacionCambioClave == true)		// lectura de pulsaciones de cambiar clave
		cambiarClave();
	
	if (leerClave == true)						// lectura de pulsaciones de leer clave ingresada
		verificarClave();

	if (leerPulsacionAlarma == true)			// 
		leerPulsacionMenuAlarma();

	if (leerPulsacionSirena == true)			// lectura de pulsacion de desactivar sirena
		pulsoDesactivarS();

	if (leerPulsacionConfig == true)			// lectura de pulsaciones de menu configuracion
		leerPulsacionMenuConfiguracion();
	
	if (leerPulsacionConfigHora == true)			// lectura de pulsaciones de menu config hora actual
		leerPulsacionMenuConfigHora();

	/*if (leerPulsacionConfigRed == true)			// lectura de pulsaciones de menu config red
		leerPulsacionMenuConfigRed();*/

	if (leerPulsacionConfigCelular == true)			// lectura de pulsaciones de menu config celular
		leerPulsacionMenuConfigCelular();


	
	// ********************* INGRESO A LAS PANTALLAS DE MENU *******************************

	if (pantalla0 == true)			// verifica clave ingresada
	{
    
		pantalla0 = false;
		pantallaBloqueada();		
	}

	if (pantalla1 == true)			// ingresa al menu principal
	{
		pantalla1 = false;
		menuPrincipal();
	}

	if (pantalla2 == true)			// establecer hora de alarma
	{
		pantalla2 = false;
		programarDespertador();
	}

	if (pantalla3 == true)			// ingresa pantalla desactivar despertador
	{
		pantalla3 = false;
		desactivarDespertador();
	}

	if (pantalla5 == true)			// ingresa a pantalla de cambiar clave
	{
		pantalla5 = false;
		pantallaCambioDeClave();
	}
	if (pantalla6 == true)			// ingresa a pantalla de menu luces
	{
		pantalla6 = false;
		menuLuces();
	}
	if (pantalla7 == true)			// ingresa a pantalla de menu ventiladores
	{
		pantalla7 = false;
		menuVentiladores();
	}
	/*if (pantalla8 == true)			// ingresa a pantalla de menu porton
	{
		pantalla8 = false;
		menuPorton();
	}*/

	if (pantalla9 == true)			// ingresa a pantalla de MENU ALARMA
	{
		pantalla9 = false;
		menuAlarma();
	}

	if (pantalla10 == true)			// ingresa a pantalla de CONFGURACION
	{
		pantalla10 = false;
		menuConfiguracion();
	}

	if (pantalla11 == true)			// ingresa a pantalla de CONFG HORA ACTUAL
	{
		pantalla11 = false;
		menuConfigHora();
	}

	/*if (pantalla12 == true)			// ingresa a pantalla de CONFG RED
	{
		pantalla12 = false;
		menuConfigRed();
	}*/

	if (pantalla13 == true)			// ingresa a pantalla de INFORMACION
	{
		pantalla13 = false;
		menuConfigCelular();
	}
    
}
