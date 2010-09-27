using namespace std;
#include <string>
#include <stdlib.h>
#include <SolarSockets/SolarSockets++.h>
#include <ctype.h>
#include "Cliente.h"
#include <unistd.h>
#include "Pantalla.h"
#include "Pixel.h"
#include "math.h"

int main(){
	Pantalla* pantalla=new Pantalla();
	Pixel* pixel=new Pixel(100,200,255);
    for(float t=0;t<6.29;t=t+0.001){
    	int x= 400+280*cos(t);
    	int y= 300+280*sin(t);
    	pantalla->dibujarPixel(x,y,pixel);
    }
	pantalla->actualizarPantalla(0,0,0,0);
	sleep(4);
	delete pixel;
	delete pantalla;
	Cliente* cliente= new Cliente();
	cliente->conectar();
	bool seguir=true;
	//empieza la interacción usuario - cliente - servidor
	while(seguir){
		system("clear");
		cout<<"Elija la opción deseada:"<<endl;
		cout<<"1 - Ingresar operacion"<<endl;
		cout<<"2 - Levantar archivo de operaciones"<<endl;
		cout<<"3 - Pruebas de pantalla"<<endl;
		cout<<"0 - Salir"<<endl;
		string respuesta;
		cin>>respuesta;
		while((respuesta!="1")and(respuesta!="2")and(respuesta!="0")and(respuesta!="3")){
				cout<<"Ingrese una opción valida"<<endl;
				cin>>respuesta;
		}
		switch(respuesta[0]){
			case '1':cliente->interactuarConUsuarioYservidor();break;
			case '2':{
				string nombreArchivo;
				cout<<"Ingrese el nombre del archivo"<<endl;
				cin>>nombreArchivo;
				cliente->enviarArchivoOperaciones(nombreArchivo);
			}
				break;
			case '3':{
				Pantalla* pantalla = new Pantalla();
				Pixel* pixel = new Pixel((char)100,(char)255,(char)255);
				pantalla->dibujarPixel(10,10,pixel);
				pantalla->actualizarPantalla(10,10,10,10);
				sleep(2);
				delete pantalla;
				break;
			}
			case '0': seguir=false; break;
		}
	}
	delete cliente;
	return 0;
}
