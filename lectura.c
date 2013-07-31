/************************************************
*  codigo para leer datos desde archivo	*
************************************************/
//Esto es la cabecera, que seguro ya pusieron ustedes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define BUFSIZE 512

//agregué dos estructuras, una para datos fijos y otra para variables

typedef struct {
        double x;
        double y;
        double z;
        double vx;
        double vy;
        double vz;}CUERPO;

typedef struct {
	char *nomb_part;
	double masa;
	}DATOS;

CUERPO *tiempo_cero;   // arreglo de estructuras con los datos iniciales sin las masas

DATOS *fijos;	// arrgeglo de los datos fijos, tengo duda en el nombre, pero la masa si se almacena bien

FILE *entrada, *salida;

int main (int argc, char *argv[])
{

int i=0, j=0, cont=0;
char campo[20], aux[20], linea[512];      //Arreglo para almacenar el bufer de entrada

if (argc!=2) {  //cambiar a 5 cuando el programa este listo
     fprintf (stderr,"Use: %s archivo_entrada archivo_salida _numero_iteraciones delta_t\n",argv[0]);
     exit (0);
  }


if (entrada = fopen(argv[1], "r")) {

// Contador de lineas (particulas)
while (fgets(linea,512,entrada)!=NULL)
	cont++;
printf("\"%s\": %d lineas\n", argv[1], cont);

fseek(entrada, 0L, SEEK_SET); //nos regresa al inicio del archivo

	fijos=(DATOS *)malloc(cont*sizeof(DATOS));
	tiempo_cero=(CUERPO *)malloc(cont*sizeof(CUERPO));

for(j=0; j<cont; j++){
	for(i=0; i<8; i++){ // son 8 campos los que debe tener cada linea del archivo de entrada
		fscanf(entrada, "%s[\t\n]", campo);
		switch(i){
			case 0 : //printf("campo= %s \n", campo);
				 fijos[j].nomb_part= strdup(campo); // si no se susa strdup no se conserva el nombre en la cadena y al final será igual que el ultimo dato "campo" que se haya escaneado
				 //printf("la entrada es %s \n", fijos[j].nomb_part ); //probando el buffer
				 break;
				 //fgetc(entrada);
				//fscanf(entrada, "%s[\t\n]", campo);
			case 1 : //printf("campo= %s \n", campo);
				 fijos[j].masa=atof(campo);
				 //printf("la entrada es %f \n", fijos[j].masa ); //probando el buffer
				 break;
				 //fgetc(entrada);
				 //fscanf(entrada, "%s[\t\n]", campo);
			case 2 : //printf("campo= %s \n", campo);
				 tiempo_cero[j].x=atof(campo);
				 //printf("la entrada es %f \n", tiempo_cero[j].x ); //probando el buffer
				 break;
				 //fgetc(entrada);
			case 3 : tiempo_cero[j].y=atof(campo);
				 break;
			case 4 : tiempo_cero[j].z=atof(campo);
				 break;
			case 5 : tiempo_cero[j].vx=atof(campo);
				 break;
			case 6 : tiempo_cero[j].vy=atof(campo);
				 break;
			case 7 : tiempo_cero[j].vz=atof(campo);
				 break;
			} // Fin del switch
		} // Fin del for para cada linea del archivo de entrada
	
	} // Fin del for para todas las entradas


fclose(entrada);

} // Fin del if

else {

error("N_CUERPOS: no se puede abrir %s", argv[1]);
exit(1);
}

/**************************************************************************
esta rutina es para probar que los datos se hayan insertado bien en los arrglos y es donde no imprime el nombre, en los print que hay en el switch si lo imprime bien

printf("j= %d\n", j);

for(i=0; i<j; i++)
printf("la particula es %s con masa %f y coordx %f\n", fijos[i].nomb_part, fijos[i].masa, tiempo_cero[i].x ); //probando el buffer
****************************************************************************/

}//Fin de la función main

