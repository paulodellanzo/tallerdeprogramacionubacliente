#include "Cliente.h"

Cliente::Cliente() {
	this->parserResultado=new ParserResultadoCliente();
	this->parser= new ParserCliente();

	//creamos el socket
	descriptorSocket=socket(AF_INET,SOCK_STREAM,0);
	if(descriptorSocket==-1){
		cout<<"Mal creado socket del cliente"<<endl;
	}

	//info del server
	estructuraDeDireccion.sin_family=AF_INET;//para lo de family QUE SEA LOCAL
	estructuraDeDireccion.sin_port=htons(PORT);
	estructuraDeDireccion.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY coloca nuestra direccion IP automaticamente

	for(int i=0;i<8;i++){
			estructuraDeDireccion.sin_zero[i]=0;}
}

void Cliente::conectar(){
	socklen_t length=sizeof(sockaddr);

	//Conectar
	int valorConectar=connect(descriptorSocket,(struct sockaddr*)&estructuraDeDireccion,length);
	if(valorConectar==-1){
		cout<<"Mal conectado"<<endl;
	}
	fcntl(descriptorSocket, F_SETFL, O_NONBLOCK);
}

void Cliente::enviar(char data[]){
	size_t leng=sizeof(char[MAXBYTES]);
	socklen_t length=sizeof(sockaddr);
	int valorSend=send(descriptorSocket,data,leng,0);
	if(valorSend==-1){
		cout<<"Mal enviado al servidor"<<endl;
	}
}

void Cliente::enviarOperacion(){

	system("clear");

	string idOperacion;
	list<string>* operandos=new list<string>();

	//Selección de Operacion
	cout<<"Ingrese el id de la operacion:"<<endl;
	cout<<"		S ----> Suma"<<endl;
	cout<<"		R ----> Resta"<<endl;
	cout<<"		M ----> Multiplicación"<<endl;
	cout<<"		D ----> División"<<endl;
	cin>>idOperacion;
	//Para pasar a mayuscula
	idOperacion=toupper(idOperacion[0]);

	while((idOperacion!="S")and(idOperacion!="R")and(idOperacion!="M")and(idOperacion!="D")){
		cout<<"Ingrese una operacion valida"<<endl;
		cin>>idOperacion;
		idOperacion=toupper(idOperacion[0]);
	}

	string nombreOperando;
	string valorOperando;
	string respuesta;

	//Ingreso de operandos
	switch(idOperacion[0]){
		case ('S'):nombreOperando="sum";respuesta="S";break;
		case ('R'):nombreOperando="res";respuesta="S";break;
		case ('M'):nombreOperando="mul";respuesta="S";break;
		case ('D'):respuesta="N";
					list<string>::iterator i;
					i=operandos->begin();
					cout<<"Ingrese dividendo:"<<endl;
					cin>>valorOperando;
					operandos->insert(i,"dividendo");
					i++;
					operandos->insert(i,valorOperando);
					cout<<"Ingrese divisor:"<<endl;
					cin>>valorOperando;
					operandos->insert(i,"divisor");
					i++;
					operandos->insert(i,valorOperando);
					break;
	}

	//Lectura de operandos en caso que no se haya elegido una division
	while ((respuesta=="S")){
		cout<<"Ingresar operando"<<endl;
		cin>>valorOperando;
		operandos->push_back(nombreOperando);
		operandos->push_back(valorOperando);
		cout<<"Desea ingresar otro operando? S/N"<<endl;
		cin>>respuesta;
		respuesta=toupper(respuesta[0]);
		while((respuesta!="S")and(respuesta!="N")){
			cout<<"Desea ingresar otro operando? S/N"<<endl;
			cin>>respuesta;
			respuesta=toupper(respuesta[0]);
		}
	}
	//Generacion y envio de xml
	const char* xml=this->parser->getXmlDeOperacion(idOperacion,operandos);
	enviar((char*)xml);
	cout<<"Mensaje enviado al Servidor"<<endl;
}

void Cliente::enviarArchivoOperaciones(string nombreArchivo){
	const char* nombreArchivoString = nombreArchivo.c_str();
	ParserCliente* parserArchivo= new ParserCliente(nombreArchivoString);
	const char* xml = parserArchivo->getSiguienteOperacion();
	while(strcmp(xml,"")!=0){
		enviar((char*)xml);
		xml = parserArchivo->getSiguienteOperacion();
	}
	delete parserArchivo;
}

void Cliente::recibir(){
	char data[MAXBYTES];
	socklen_t leng=sizeof(data);
	ssize_t valorRecive=recv(this->descriptorSocket,&data,leng,0);
	if(valorRecive==-1){
		cout<<"Mal recibido"<<endl;
	}else{
		data[valorRecive]='\0';
		cout<<data<<endl;
	}
}
Cliente::~Cliente() {
	delete this->parser;
	delete this->parserResultado;
}
