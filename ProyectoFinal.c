/**************************************************/
/* Algoritmo registrosMeteorologicos              */
/*                                                */ 
/* Descripción: Algoritmo que simula una estación */
/* meteorológica, permite ingresar datos por      */
/* fechas, posee busquedas, borrado, ordenamientos*/
/* como también guardar los datos en un archivo   */ 
/* .dat y hacer un backup del mismo.              */ 
/*                                                */ 
/* Autores: Palacio Cristian, Quiroga Agustín     */
/*                                                */
/* Fecha: 16/11/2022                              */
/**************************************************/
//El código se ejecuto y compilo con Visual Studio Code en Windows 10/11, es funcional con el archivo ".dat" pasado en EVELIA.
//Si se ejecuta en el compilador online(onlinegdb.com), el archivo ".dat" proveido no funciona, se corrompe, hay que ingresar los datos a mano del mismo(Es solamente en el caso de querer utilizar un archivo pre cargado).

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#define max 1000
    typedef struct{
        long int ddmmyyyy; //fecha del registro, dd es el día, mm el mes, yyyy el año
        int tmax;   //temperatura máxima ocurrida ese día
        int tmin;   //temperatura mínima ocurrida ese día
        int HUM;    //humedad promedio del día registrado
        int PNM;    //valor es la presión atmosférica
        int DV;     //dirección del viento que se toma en grados y en valores enteros de 0 a 360
        int FF;     //velocidad máxima ocurrida en el dia
        int PP;     //precipitación pluvial acumulado en 24 hs
        int borrado;// baja lógica del registro
    } Tregdiario;
    
    struct TNodo {
        Tregdiario info;
        struct TNodo *next;
    };
    
    struct TNodo* CrearNodo();

    FILE* f;    //variable interna del archivo
    char nomArch[max]; //nombre del archivo
    char buffer2[max];
    int respMenu;
    int cantFechas = 0;
    long int searchDate;

    //Funciones
    int Vacia(Tregdiario fecha[max]);
    int Llena(Tregdiario fecha[max]);
    int buscarFecha(Tregdiario fecha[max], long int fechaBuscar, int n);
    struct TNodo* CrearNodo(Tregdiario fecha[max], int index);

    //Acciones
    void Menu (int *rp);
    void Insertar(Tregdiario fecha[max]);
    void revisoPreCargado(Tregdiario fecha[max], char nameArchivo[max]);
    void Mostrar(Tregdiario fecha[max]);
    void guardoLista(Tregdiario fecha[max], char nameArchivo[max]);
    void muestroText(Tregdiario fecha[max], int numActual);
    void ingresoDatos(Tregdiario fecha[max], int index);
    void modificoFecha(Tregdiario fecha[max], long int fechaBuscar);
    void modificoDatosFecha(Tregdiario fecha[max], int index);
    void borroFecha(Tregdiario fecha[max], long int fechaBuscar);
    void backupFile(Tregdiario fecha[max], char nameFile[max]);
    void InsertarC(struct TNodo **primero, Tregdiario fecha[max], int index);
    void maxTemperature(Tregdiario fecha[max]);
    void MostrarLista(struct TNodo *puntero);
    void orderPrecipitation(Tregdiario fecha[max]);
    void speedWind(Tregdiario fecha[max]);
    void ingresoDatosfecha(long int *fechaBuscar);

//Algoritmo principal
int main(){
    Tregdiario fechaNew[max];
    setlocale(LC_ALL,"");

    printf("Ingrese el nombre del archivo SIN la extension '.dat': ");
    scanf("%s", nomArch);

    //Concatenamos el nombre del archivo.
    strcpy(buffer2, ".dat");
    strcat(nomArch, buffer2);

    revisoPreCargado(fechaNew,nomArch); //Revisamos si el archivo dado tiene registros cargados.
    while(respMenu != 10){ //Selecciono una opcion.
        Menu(&respMenu);
        switch(respMenu){
            case 1: printf("\nUsted a seleccionado la opcion de dar de alta un registro\n");
                    if(Llena(fechaNew)==1){
                        printf("\n¡La lista esta llena, no puedes hacer esto!\n");
                    }else{
                        Insertar(fechaNew);
                        guardoLista(fechaNew, nomArch);
                    }
                    break;
            case 2: printf("Usted a seleccionado la opcion de dar de baja un registro\n");
                    if(Vacia(fechaNew)==1){
                        printf("\n¡La lista esta vacia, no puedes hacer esto!\n");
                    }else{
                        printf("\nPor favor ingrese su fecha\n");
                        ingresoDatosfecha(&searchDate);
                        borroFecha(fechaNew, searchDate);
                        guardoLista(fechaNew, nomArch);
                    }
                    break;
            case 3: printf("\nUsted a seleccionado la opcion de modificar un registro\n");
                    if(Vacia(fechaNew)==1){
                        printf("\n¡La lista esta vacia, no puedes hacer esto!\n");
                    }else{
                        printf("\nPor favor ingrese su fecha\n");
                        ingresoDatosfecha(&searchDate);
                        modificoFecha(fechaNew, searchDate);
                        guardoLista(fechaNew, nomArch);
                    }
                    break;
            case 4: printf("\nUsted a seleccionado la opcion para ver todos los registros activos\n");
                    if(Vacia(fechaNew)==1){
                        printf("\n¡La lista esta vacia!\n");
                    }else{
                        Mostrar(fechaNew);
                    }
                    break;
            case 5: printf("\nUsted a seleccionado la opcion para buscar un registro de un día dado\n");
                    if(Vacia(fechaNew)==1){
                        printf("\n¡La lista esta vacia, no puedes hacer esto!\n");
                    }else{
                        printf("\nPor favor ingrese su fecha\n");
                        ingresoDatosfecha(&searchDate);
                        if(buscarFecha(fechaNew, searchDate, 1) == -1){
                            printf("\n¡Esa fecha no existe!\n");
                        }else{
                            printf("\n[RESULTADO]\n");
                            muestroText(fechaNew,buscarFecha(fechaNew, searchDate, 1));
                        }
                    }
                    break;
            case 6: printf("\nUsted a seleccionado la opcion listar el día o días de máxima temperatura en lo que va del año\n");
                    if(Vacia(fechaNew)==1){
                        printf("\n¡La lista esta vacia!\n");
                    }else{
                        maxTemperature(fechaNew);
                    }
                    break;
            case 7: printf("\nUsted a seleccionado al opcion listar el día o días de máxima precipitación en lo que va del año\n");
                    if(Vacia(fechaNew)==1){
                        printf("\n¡La lista esta vacia!\n");
                    }else{
                        orderPrecipitation(fechaNew);
                    }
                    break;
            case 8: printf("\nUsted a seleccionado dar la opcion de listar las fechas de mayor a menor velocidad de viento\n");
                    if(Vacia(fechaNew)==1){
                        printf("\n¡La lista esta vacia!\n");
                    }else{
                        speedWind(fechaNew);
                    }
                    break;
            case 9: printf("\nUsted a seleccionado la opcion para realizar una copia de seguridad del archivo del año en curso\n"); //Se debe finalizar el programa para que se cree el nuevo archivo.
                    if(Vacia(fechaNew)==1){
                        printf("\n¡La lista esta vacia!\n");
                    }else{
                        backupFile(fechaNew, nomArch);
                    }
                    break;
            case 10: printf("\nGracias...\n\n");
        }
    }
}

//Accion que revisa si el archivo tiene previamente elementos cargados y los agrega al arreglo.
void revisoPreCargado(Tregdiario fecha[max], char nameArchivo[max]){
    FILE *arch;
    int i = 0;
    arch=fopen(nameArchivo,"a+b");
        while(!feof(arch)){ //Si tiene elementos cargados los cuento.
            i++;
            if(fecha[i].borrado != 1) fread(&fecha[i],sizeof(fecha[i]),1,arch);
        };
    fclose(arch);
    cantFechas = i;
}

//Accion que realiza un backup del archivo, le "-backup.dat" a su nombre.
void backupFile(Tregdiario fecha[max], char nameFile[max]){
    char nameFileBackup[max]; 
    int longNameFile = strlen(nameFile); 

    for(int i=0; i < longNameFile - 4; i++){
        nameFileBackup[i] = nameFile[i];
    }

    strcat(nameFileBackup, "-backup.dat");

    FILE *arch;
        printf("\nGenerando backup...");
        arch=fopen(nameFileBackup,"wb");
            for(int i=1; i < cantFechas; i++){
                if(fecha[i].borrado != 1) fwrite(&fecha[i],sizeof(fecha[i]),1,arch);
            }
    fclose(arch);
        printf("\n¡Backup generado con exito!\n");
}

//Accion que inserta una fecha y aumenta la cantidad de fechas cargadas.
void Insertar(Tregdiario fecha[max]){
    ingresoDatos(fecha, cantFechas);
    cantFechas = cantFechas + 1;
}

//Accion que muestra el menu.
void Menu (int *rp){
        do{
            printf("\n1- Alta de un registro diario\n");
            printf("2- Suprimir un registro diario\n");
            printf("3- Modificar un registro, se busca por la fecha\n");
            printf("4- Mostrar todos los registros activos\n");
            printf("5- Buscar registro de un día dado y mostrar todos los parámetros\n");
            printf("6- Listar el día o días de máxima temperatura en lo que va del año\n");
            printf("7- Listar el día o días de máxima precipitación en lo que va del año\n");
            printf("8- Listar las fechas de mayor a menor velocidad de viento\n");
            printf("9- Realizar una copia de seguridad del archivo del año en curso\n");
            printf("10- Salir\n");
            printf("Ingrese una opcion numerica: ");
            scanf("%d", &(*rp));
            
        }while(*rp < 1 || *rp > 10);
}

//Accion que muestra los elementos del archivo.
void Mostrar(Tregdiario fecha[max]){
    int contador = 0;
    printf("\n[FECHAS]\n");
    for(int i=1; i < cantFechas; i++){
        if(fecha[i].borrado != 1){
            muestroText(fecha, i);
            contador++;
        } 
    }
    printf("\nFECHAS TOTALES: %d\n", contador);
}

//Accion que muestra los datos del registro.
void muestroText(Tregdiario fecha[max], int numActual){
    printf("\nFecha: %ld", fecha[numActual].ddmmyyyy);
    printf("\nTemperatura Maxima: %d", fecha[numActual].tmax);
    printf("\nTemperatura Minima: %d", fecha[numActual].tmin);
    printf("\nHum: %d", fecha[numActual].HUM);
    printf("\nPNM: %d", fecha[numActual].PNM);
    printf("\nDV: %d", fecha[numActual].DV);
    printf("\nFF: %d", fecha[numActual].FF);
    printf("\nPP: %d\n", fecha[numActual].PP);
}

//Accion que guarda el achivo.
void guardoLista(Tregdiario fecha[max], char nameArchivo[max]){
    FILE *arch;
        arch=fopen(nameArchivo,"wb");
            for(int i=1; i < cantFechas; i++){
                fwrite(&fecha[i],sizeof(fecha[i]),1,arch);
            }
    fclose(arch);
}

//Funcion que revisa si el archivo esta lleno.
int Llena(Tregdiario fecha[max]){
    int m = 0;
        while(fecha[m].ddmmyyyy != 0){
            m++;
        }
        
        if(m == max){
            return 1;
        }else{
            return 0;
        }
}

//Funcion que revisa si el archivo esta vacio.
int Vacia(Tregdiario fecha[max]){
    int m = 1, contador = 0;
    
        while(fecha[m].ddmmyyyy != 0){
            contador++;
            m++;
        }
        
        if(contador == 0){
            return 1;
        }else{
            return 0;
        }
}

//Accion que permite modificar una fecha dada.
void modificoFecha(Tregdiario fecha[max], long int fechaBuscar){
    int n=1;
    
    while(fecha[n].ddmmyyyy != fechaBuscar && n <= cantFechas){
        n++;
    }
    
    if(fecha[n].ddmmyyyy != fechaBuscar || fecha[n].borrado == 1){
        printf("\n¡Esa fecha no existe!\n");
    }else{
        printf("\n[MODIFICAR FECHA]\n");
        modificoDatosFecha(fecha, n); 
    }
}

//Funcion recurisva que permite buscar una fecha.
int buscarFecha(Tregdiario fecha[max], long int fechaBuscar, int n){
    if(fecha[n].ddmmyyyy != fechaBuscar && n == cantFechas){
        return -1;
    }else if(fecha[n].ddmmyyyy == fechaBuscar && fecha[n].borrado == 0){
        return n;
    }else{
        return buscarFecha(fecha, fechaBuscar, n + 1); 
    }
}

//Accion que permite ingresar los datos de los registros
void ingresoDatos(Tregdiario fecha[max], int index){
    int dia;
    int mes;
    int anio;
    char textDia[8];
    char textMes[8];
    char textAnio[8];

    char auxMes[8];

    printf("Ingrese el año: ");fflush(stdout);
    scanf("%d", &anio);
    while(anio < 1000 || anio > 9999){
        printf("Ingrese un año valido: ");fflush(stdout);
        scanf("%d", &anio);
	}

    if(anio % 4 == 0 && anio % 100 != 0 || anio % 400 == 0  ){
        printf("Ingrese el mes: ");fflush(stdout);
        scanf("%d", &mes);

        while(mes < 1 || mes > 12){
            printf("Ingrese un mes valido: ");fflush(stdout);
            scanf("%d", &mes);
        }

        if(mes== 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12){
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 31){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }else if(mes == 2){
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 29){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }else{
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 30){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }
    }else{
        printf("Ingrese el mes: ");fflush(stdout);
        scanf("%d", &mes);

        while(mes < 1 || mes > 12){
            printf("Ingrese un mes valido: ");fflush(stdout);
            scanf("%d", &mes);
        }

        if(mes== 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12){
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 31){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }else if(mes == 2){
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 28){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }else{
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 30){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }
    }

    //////////////////Uso de librerias para concatenar los datos de la fecha ingresada y dejarla en formato ddmmyyyy

    strcpy(auxMes, "0");

    sprintf(textDia, "%d", dia);
    sprintf(textMes, "%d", mes);   
    sprintf(textAnio, "%d", anio);

    if(mes < 10){   //Si el mes menor a 10, le concatenamos un 0, para que no rompa el formato ddmmyyyy
        strcat(auxMes, textMes);
        strcpy(textMes, auxMes);
    }

    strcat(textDia, textMes);
    strcat(textDia, textAnio);

    long int entero = (long int) strtol(textDia, NULL, 10);
    ////////////////////////////////////////////////////

    if(buscarFecha(fecha, entero, 1) == -1){
        fecha[index].ddmmyyyy = entero;

        printf("Ingrese la temperatura maxima ocurrida en el dia: ");fflush(stdout);
        scanf("%d", &fecha[index].tmax);
        
        printf("Ingrese la temperatura minima ocurrida en el dia: ");fflush(stdout);
        scanf("%d", &fecha[index].tmin);
        
        while(fecha[index].tmin > fecha[index].tmax){
            printf("La temperatura minima no puede ser mayor que la temperatura maxima\n");
            printf("Ingrese una temperatura minima valida: ");fflush(stdout);
            scanf("%d", &fecha[index].tmin);
        }
        
        printf("Ingrese la humedad promedio del dia registrado(0 a 100): ");fflush(stdout);
        scanf("%d", &fecha[index].HUM);
        while(fecha[index].HUM < 0 || fecha[index].HUM > 100){
            printf("Ingrese la humedad promedio del dia registrado(0 a 100): ");fflush(stdout);
            scanf("%d", &fecha[index].HUM);
        }
        
        printf("Ingrese el valor de la presion atmosferica (900 a 3500): ");fflush(stdout);
        scanf("%d", &fecha[index].PNM);
        while(fecha[index].PNM < 900 || fecha[index].PNM > 3500){
            printf("Ingrese el valor de la presion atmosferica (900 a 3500): ");fflush(stdout);
            scanf("%d", &fecha[index].PNM);
        }
            
        printf("Ingrese la dirección del viento, se toma en grados y en valores enteros de 0 a 360: ");fflush(stdout);
        scanf("%d", &fecha[index].DV);
        while(fecha[index].DV < 0 || fecha[index].DV > 360){
            printf("Ingrese la dirección del viento, se toma en grados y en valores enteros de 0 a 360: ");fflush(stdout);
            scanf("%d", &fecha[index].DV);
        }
        
        printf("Ingrese la velocidad máxima del viento ocurrida en el dia: ");fflush(stdout);
        scanf("%d", &fecha[index].FF);
        
        printf("Ingese la precipitación pluvial acumulado en 24 hs: ");fflush(stdout);
        scanf("%d", &fecha[index].PP);
        while(fecha[index].PP < 0 || fecha[index].PP > 1000){
            printf("Ingese la precipitación pluvial acumulado en 24 hs (0 a 1000): ");fflush(stdout);
            scanf("%d", &fecha[index].PP);
        }
    }else{
        printf("\n¡Esa fecha ya existe!\n");
        cantFechas = cantFechas - 1;
    }
}

void ingresoDatosfecha(long int *fechaBuscar){
    int dia;
    int mes;
    int anio;
    char textDia[8];
    char textMes[8];
    char textAnio[8];

    char auxMes[8];

    printf("Ingrese el año: ");fflush(stdout);
    scanf("%d", &anio);
    while(anio < 1000 || anio > 9999){
        printf("Ingrese un año valido: ");fflush(stdout);
        scanf("%d", &anio);
	}

    if(anio % 4 == 0 && anio % 100 != 0 || anio % 400 == 0  ){
        printf("Ingrese el mes: ");fflush(stdout);
        scanf("%d", &mes);

        while(mes < 1 || mes > 12){
            printf("Ingrese un mes valido: ");fflush(stdout);
            scanf("%d", &mes);
        }

        if(mes== 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12){
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 31){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }else if(mes == 2){
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 29){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }else{
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 30){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }
    }else{
        printf("Ingrese el mes: ");fflush(stdout);
        scanf("%d", &mes);

        while(mes < 1 || mes > 12){
            printf("Ingrese un mes valido: ");fflush(stdout);
            scanf("%d", &mes);
        }

        if(mes== 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12){
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 31){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }else if(mes == 2){
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 28){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }else{
            printf("Ingrese el día:  ");fflush(stdout);
            scanf("%d", &dia);
            while(dia < 1 || dia > 30){
                printf("Ingrese un día valido:  ");fflush(stdout);
                scanf("%d", &dia);
            }
        }
    }

    //////////////////Uso de librerias para concatenar los datos de la fecha ingresada y dejarla en formato ddmmyyyy

    strcpy(auxMes, "0");

    sprintf(textDia, "%d", dia);
    sprintf(textMes, "%d", mes);   
    sprintf(textAnio, "%d", anio);

    if(mes < 10){   //Si el mes menor a 10, le concatenamos un 0, para que no rompa el formato ddmmyyyy
        strcat(auxMes, textMes);
        strcpy(textMes, auxMes);
    }

    strcat(textDia, textMes);
    strcat(textDia, textAnio);

    *fechaBuscar = (long int) strtol(textDia, NULL, 10);

    ////////////////////////////////////////////////////
}

void modificoDatosFecha(Tregdiario fecha[max], int index){
        printf("Ingrese la temperatura maxima ocurrida en el dia: ");fflush(stdout);
        scanf("%d", &fecha[index].tmax);
        
        
        printf("Ingrese una temperatura minima valida: ");fflush(stdout);
        scanf("%d", &fecha[index].tmin);
        while(fecha[index].tmin > fecha[index].tmax){
            printf("La temperatura minima no puede ser mayor que la temperatura maxima\n");
            printf("Ingrese una temperatura minima valida: ");fflush(stdout);
            scanf("%d", &fecha[index].tmin);
        };
        
        printf("Ingrese la humedad promedio del dia registrado(0 a 100): ");fflush(stdout);
        scanf("%d", &fecha[index].HUM);
        while(fecha[index].HUM < 0 || fecha[index].HUM > 100){
            printf("Ingrese la humedad promedio del dia registrado(0 a 100): ");fflush(stdout);
            scanf("%d", &fecha[index].HUM);
        }
        
        printf("Ingrese el valor de la presion atmosferica (900 a 3500): ");fflush(stdout);
        scanf("%d", &fecha[index].PNM);
        while(fecha[index].PNM < 900 || fecha[index].PNM > 3500){
            printf("Ingrese el valor de la presion atmosferica (900 a 3500): ");fflush(stdout);
            scanf("%d", &fecha[index].PNM);
        }
            
        printf("Ingrese la dirección del viento, se toma en grados y en valores enteros de 0 a 360: ");fflush(stdout);
        scanf("%d", &fecha[index].DV);
        while(fecha[index].DV < 0 || fecha[index].DV > 360){
            printf("Ingrese la dirección del viento, se toma en grados y en valores enteros de 0 a 360: ");fflush(stdout);
            scanf("%d", &fecha[index].DV);
        }
        
        printf("Ingrese la velocidad máxima del viento ocurrida en el dia: ");fflush(stdout);
        scanf("%d", &fecha[index].FF);
        
        printf("Ingese la precipitación pluvial acumulado en 24 hs: ");fflush(stdout);
        scanf("%d", &fecha[index].PP);
        while(fecha[index].PP < 0 || fecha[index].PP > 1000){
            printf("Ingese la precipitación pluvial acumulado en 24 hs (0 a 1000): ");fflush(stdout);
            scanf("%d", &fecha[index].PP);
        }
};

//Accion que borra una fecha dada.
void borroFecha(Tregdiario fecha[max], long int fechaBuscar){
        int n=1;
        
        while(fecha[n].ddmmyyyy != fechaBuscar && n <= cantFechas){
            n++;
        }   
        
        if(fecha[n].ddmmyyyy != fechaBuscar){
            printf("\n¡Esa fecha no existe!\n");
        }else{
            fecha[n].borrado = 1;
            printf("\n¡Fecha borrada con exito!\n");
        }
}

//Funcion que crea un nodo
struct TNodo* CrearNodo(Tregdiario fecha[max], int index) {
	struct TNodo * nuevo = NULL;
	nuevo = (struct TNodo *) malloc (sizeof(struct TNodo));
	if (nuevo==NULL){
	 	exit(EXIT_FAILURE); 
	} else {
	  nuevo->info.ddmmyyyy = fecha[index].ddmmyyyy;
      nuevo->info.tmax = fecha[index].tmax;
	  nuevo->next = NULL;
	  return nuevo;
	}
}

//Accion que permite insertar un elemento al inicio de una lista enlazada
void InsertarC(struct TNodo **primero, Tregdiario fecha[max], int index) {
    struct TNodo *nuevo; //Creamos un nuevo nodo
    nuevo = CrearNodo(fecha, index);
	if (*primero==NULL) {
	 	*primero = nuevo;
	} else {
		nuevo->next = *primero;
		*primero = nuevo;
	}
}


//Accion que muestra el día con la máxima temperatura,(en caso de haber 2 o más fechas con los mismos datos de temperatura las muestra).
void maxTemperature(Tregdiario fecha[max]){
        int n=1;
        struct TNodo *primero, *aux;
        int mayor = fecha[0].tmax;
        int index = 0;
        primero = NULL;
        int cantTempMax = 0;
        
        while(n <= cantFechas){
            if (fecha[n].tmax >= mayor) {
				mayor = fecha[n].tmax;
				index = n;
			}
            n++;
        }

        n=1;
        
        while(n <= cantFechas){
            if(fecha[n].tmax == mayor){
                InsertarC(&primero, fecha, n);
            }
            n++;
        }

        while (primero != NULL) {
            printf( "Fecha: %ld \n", primero->info.ddmmyyyy);
            printf( "Temperatura: %d \n", primero->info.tmax);
            primero = primero->next;
        }
}

//Accion que muestra las primeras 10 fechas de mayor a menor por velocidad del viento, se utilizo Bubble Sort.
void speedWind(Tregdiario fecha[max]){
    int i, j;            
    Tregdiario temp[max];
    Tregdiario aux[max];
    int topten = 0;
    int limit = 10;
    for(i=1; i < cantFechas; i++){
        aux[i] = fecha[i];
    }
    
    for (i=1; i < cantFechas; i++){
       for (j=1; j < cantFechas - 1; j++){
          if (aux[j].FF < aux[j + 1].FF) {
            temp[0]=aux[j];
            aux[j]=aux[j + 1];
            aux[j + 1]=temp[0];
          }
          
       }
    }

    if(cantFechas < 10) limit = cantFechas; //Si hay menos de 10 registros cargados

    printf("\n[FECHAS]\n");
    for(i=1; i < cantFechas; i++){
        if(aux[i].borrado != 1 && topten < 10){
            printf("\nFecha: %ld", aux[i].ddmmyyyy);
            printf("\nVelocidad del viento: %d\n", aux[i].FF);
            topten++;
        } 
    }
}

//Accion que muestra las primeras 10 fechas de mayor a menor por precipitacion total del dia, se utilizo Insertion Sort, para ordenarlas de menor a mayor.
void orderPrecipitation(Tregdiario fecha[max]) {
    int i, j;
    Tregdiario aux[max];
    Tregdiario temp[max];
    int topten = 0;
    int limit = 10;
    for(i=1; i < cantFechas; i++){
        aux[i] = fecha[i];
    }
    
    for (i=1; i < cantFechas; i++) {
        temp[i] = aux[i];
        j = i - 1;
        
        while (j >= 0 && aux[j].PP > temp[i].PP) {
            aux[j + 1] = aux[j];
            j--;
        }
        
        aux[j + 1] = temp[i];
    }

    if(cantFechas < 10) limit = cantFechas; //Si hay menos de 10 registros cargados

    printf("\n[FECHAS]\n");
    for(i=cantFechas - 1; i > 0; i--){
        if(aux[i].borrado != 1 && topten < 10){
            printf("\nFecha: %ld", aux[i].ddmmyyyy);
            printf("\nPrecipitacion total del dia: %d\n", aux[i].PP);
            topten++;
        }
    }
}

