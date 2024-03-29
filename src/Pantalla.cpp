#include "Pantalla.h"

Pantalla::Pantalla(int alto, int ancho) {
	SDL_Init(SDL_INIT_VIDEO);
	pantalla=SDL_SetVideoMode(ancho,alto,24,SDL_SWSURFACE);
	//Cuando termina el programa se cierra el SDL
	atexit(SDL_Quit);
	if (TTF_Init() == 0) {
			atexit(TTF_Quit);
		}
	if (pantalla==0 or !(comprobarPantalla())) {
		cerr << "Error al crear ventana" << endl;
		cerr << SDL_GetError() << endl;
		exit(1);
	}
	SDL_EnableUNICODE(1);
}

bool Pantalla::comprobarPantalla(){
	// Bytes que ocupa un pixel
	if(pantalla->format->BytesPerPixel!=3){
		return false;
	}
	return true;
}

void Pantalla::dibujarPixel(int x, int y, SDL_Color color){
	if(x>(this->getAncho()) or y>(this->getAlto())) return;
	//Si hay que bloquear la pantalla la bloquea, al final de la función la desbloquea
	if ( SDL_MUSTLOCK(pantalla) ){
		if ( SDL_LockSurface(pantalla) < 0 ){
			return;
		}
	}

	// p es un puntero al pixel que vamos a pintar
	//pitch es la cantidad de bytes que ocupa una linea de la pantalla
	Uint8 *p = (Uint8*)pantalla->pixels + (y*pantalla->pitch) + (x*(pantalla->format->BytesPerPixel));
	if(!((color.r==0)and(color.g==255)and(color.b==0))){
		if (SDL_BYTEORDER==SDL_BIG_ENDIAN) {
			p[0]=color.r;
			p[1]=color.g;
			p[2]=color.b;
		} else {
			p[0]=color.b;
			p[1]=color.g;
			p[2]=color.r;
		}
	}
	if ( SDL_MUSTLOCK(pantalla) ){
		SDL_UnlockSurface(pantalla);
	}
}

void Pantalla::actualizarPantalla(int x, int y, int w, int h){
	SDL_UpdateRect(pantalla, x, y, w, h);
}

void Pantalla::dibujarBitMapDesdePos(BitMap* bitmap,int x, int y){
	SDL_Color** matrizDelBitmap=bitmap->getMatrizDeImagen();
	for(unsigned int i=0;i<bitmap->getAlto();i++){
		for(unsigned int j=0;j<bitmap->getAncho();j++){
			this->dibujarPixel(j + x,i + y,matrizDelBitmap[i][j]);
		}
	}
}

void Pantalla::dibujarBitMapDesdePosCircular(BitMap* bitmap,int x, int y){
	SDL_Color** matrizDelBitmap=bitmap->getMatrizDeImagen();
	double radioH=bitmap->getAlto()/2;
	double radioV=bitmap->getAncho()/2;
	double auxi;
	double auxj;
	for(unsigned int i=0;i<bitmap->getAlto();i++){
		for(unsigned int j=0;j<bitmap->getAncho();j++){
			auxi=i-radioH;
			auxj=j-radioV;
			if (((auxi*auxi)/(radioH*radioH)+(auxj*auxj)/(radioV*radioV))<=1){
					this->dibujarPixel(j + x,i + y,matrizDelBitmap[i][j]);
			}
		}
	}
}

void Pantalla::escribirTextoDesdePos(const char* texto, int x, int y, int tamaniofuente,SDL_Color color){
// Cargamos la fuente que vamos a utilizar
	TTF_Font *fuente;
	fuente = TTF_OpenFont("browa.ttf", tamaniofuente);
	SDL_Surface * rectangulo;
	rectangulo = TTF_RenderText_Blended(fuente, texto, color);
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.h = rectangulo->h;
	dest.w = rectangulo->w;
	// Mostramos el texto por pantalla
	SDL_BlitSurface(rectangulo, NULL, pantalla, &dest);
	TTF_CloseFont(fuente);
}

void Pantalla::escribirStringDesdePos(string texto, int x, int y, int tamaniofuente,int r, int g, int b){
	if(texto.size()!=0){
		SDL_Color color;
		color.r=r;
		color.g=g;
		color.b=b;
		escribirTextoDesdePos(texto.c_str(),x,y,tamaniofuente,color);
	}
}

void Pantalla::dibujarRectangulo(int x, int y, int ancho, int alto, int r, int g, int b){
	if(ancho==0){
		ancho=getAncho()-x;
	}
	if(alto==0){
			alto=getAlto()-y;
	}
	SDL_Color color;
	color.r=r;
	color.g=g;
	color.b=b;
	for(int i=0;i<alto;i++){
		for(int j=0;j<ancho;j++){
			this->dibujarPixel(j + x,i + y,color);
		}
	}
}

int Pantalla::getAncho(){
	return pantalla->w;
}

int Pantalla::getAlto(){
	return pantalla->h;
}

Pantalla::~Pantalla() {
}
