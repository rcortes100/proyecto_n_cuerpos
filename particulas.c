/*
particulas.c
               N numero de particulas
*/
#include <stdio.h>
#include <stdlib.h>
 #include <string.h>
#include <pthread.h>
#include <math.h>
#include <fcntl.h>


/*Constante gravitacional (depende de las unidades a usar)*/
#define const_G 1

 typedef struct {
  char* nombre;
  double masa;
  double x;
  double y;
  double z;
  double vx;
  double vy;
  double vz;}CUERPO; 

typedef struct 
 {
   CUERPO      **C_inicial;
   CUERPO      **C_final;
   double     sum; 
   int     veclen; 
 } DATOS;


int  iHilos;
/* Datos compartidos */
//pthread_mutex_t mutex1;    /* variable de bloqueo mutuamente exclusiva */
DATOS DATCICLO;
double delta_t; /* Declaracion de delta t */
off_t offset; //ofset para el archivo de salida

///////////////////////////////////////////////////////////
/*Funciones que sirven para hacer el calculo de los pasos*/
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
/*Funcion que saca la distancia al cuadrado entre dos cuerpos
///////////////////////////////////////////////////////////////
double dist2(CUERPO *cuerpo1, CUERPO *cuerpo2){
  return pow(DATCICLO.C_inicial[j]->x-DATCICLO.C_inicial[my_id]->x,2)+pow(DATCICLO.C_inicial[j]->y-DATCICLO.C_inicial[my_id]->y,2)+pow(DATCICLO.C_inicial[j]->z-DATCICLO.C_inicial[my_id]->z,2);
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////
/*Funcion que calcula el siguiente paso del cuerpo n en funcion de todas las posiciones anteriores*/
/*para la particula i. Requiere implicitamente del numero N de particulas y del incremento delta_t */
////////////////////////////////////////////////////////////////////////////////////////////////////
//void trayectoria(CUERPO pt0[0], CUERPO *DATCICLO, int i){
void *trayectoria(void *particula){
  int j;
  double m_r3,dist2;
  long my_id= (long)particula;

  DATCICLO.C_final[my_id]->vx=0; //Inicializa en ceros para poder hacer la suma de las fuerzas
  DATCICLO.C_final[my_id]->vy=0; //una por una.
  DATCICLO.C_final[my_id]->vz=0; //

  //Suma de fuerzas particula por particula, componente a componente ( iHilios= N por las condiciones del proyecto)
  for( j=0 ; j<iHilos ; j++){
    if(j!=my_id){
      dist2=pow(DATCICLO.C_inicial[j]->x-DATCICLO.C_inicial[my_id]->x,2)+pow(DATCICLO.C_inicial[j]->y-DATCICLO.C_inicial[my_id]->y,2)+pow(DATCICLO.C_inicial[j]->z-DATCICLO.C_inicial[my_id]->z,2);
      m_r3=DATCICLO.C_inicial[j]->masa/pow(dist2,1.5);
      DATCICLO.C_final[my_id]->vx+=m_r3*(DATCICLO.C_inicial[j]->x-DATCICLO.C_inicial[my_id]->x);
      DATCICLO.C_final[my_id]->vy+=m_r3*(DATCICLO.C_inicial[j]->y-DATCICLO.C_inicial[my_id]->y);
      DATCICLO.C_final[my_id]->vz+=m_r3*(DATCICLO.C_inicial[j]->z-DATCICLO.C_inicial[my_id]->z);
    }
  }

  //Multiplicacion por G y delta_t
  DATCICLO.C_final[my_id]->vx*=const_G*delta_t;
  DATCICLO.C_final[my_id]->vy*=const_G*delta_t;
  DATCICLO.C_final[my_id]->vz*=const_G*delta_t;
  //Se le agregan las velocidades del paso temporal anterior
  DATCICLO.C_final[my_id]->vx+=DATCICLO.C_inicial[my_id]->vx;
  DATCICLO.C_final[my_id]->vy+=DATCICLO.C_inicial[my_id]->vy;
  DATCICLO.C_final[my_id]->vz+=DATCICLO.C_inicial[my_id]->vz;
  //Se modifican las posiciones
  DATCICLO.C_final[my_id]->x=DATCICLO.C_inicial[my_id]->x+DATCICLO.C_inicial[my_id]->vx*delta_t;
  DATCICLO.C_final[my_id]->y=DATCICLO.C_inicial[my_id]->y+DATCICLO.C_inicial[my_id]->vy*delta_t;
  DATCICLO.C_final[my_id]->z=DATCICLO.C_inicial[my_id]->z+DATCICLO.C_inicial[my_id]->vz*delta_t;

DATCICLO.C_final[my_id]->masa=DATCICLO.C_inicial[my_id]->masa;
DATCICLO.C_final[my_id]->nombre=DATCICLO.C_inicial[my_id]->nombre;
}

///////////////////////////////////////////TERMINAN FUNCIONES DE CALCULO///////////////////////////////


int leer_campo (FILE *archivo, char *campo)
{
    fscanf (archivo, "%s[^\t\n\r]", campo);
    if (feof (archivo))
        return 0;
    fgetc (archivo);
    return 1;
}

void** arreglo_insertar (void **arreglo, int *tamano, void *dato)
{
    arreglo = (void**) realloc (arreglo, sizeof (void*)*(*tamano+1));
    arreglo[*tamano] = dato;
    (*tamano)++;
    return arreglo;
}

void imprimir (FILE *farch_out,double tiempo){
int i;
fprintf(farch_out,"tiempo %f\n", tiempo);
fseek(farch_out,0L,SEEK_CUR);
offset=lseek(fileno(farch_out), 0L, SEEK_CUR);
//printf("ofimpresion:%i\n",offset);
///Recordamos que iHilos es igual a N////
for(i=0;i<iHilos;i++){
	fprintf(farch_out,"%s\t%g\t%g\t%g\t%g\t%g\t%g\t%g\n",DATCICLO.C_final[i]->nombre,DATCICLO.C_final[i]->masa,DATCICLO.C_final[i]->x,DATCICLO.C_final[i]->y,DATCICLO.C_final[i]->z,DATCICLO.C_final[i]->vx,DATCICLO.C_final[i]->vy,DATCICLO.C_final[i]->vz);
}
}

void imprimir_jmol(FILE *jFile){
int i;
fprintf(jFile,"%i\n", iHilos);
fprintf(jFile,"%i\n", iHilos);
for(i=0;i<iHilos;i++){
fprintf(jFile,"H \t%g\t%g\t%g\n",DATCICLO.C_final[i]->x,DATCICLO.C_final[i]->y,DATCICLO.C_final[i]->z);
}
}


void leer(FILE *farch_out){
int i;
double z;
char campo[200];
//printf("of2:%i\n",offset);
fseek(farch_out,offset,SEEK_SET);
//printf("of3:%i\n",offset);
for(i=0;i<iHilos;i++){
leer_campo (farch_out, campo);
DATCICLO.C_inicial[i]->nombre=strdup (campo);
leer_campo (farch_out, campo);
DATCICLO.C_inicial[i]->masa=atof(campo);
leer_campo (farch_out, campo);
DATCICLO.C_inicial[i]->x=atof(campo);
leer_campo (farch_out, campo);
DATCICLO.C_inicial[i]->y=atof(campo);
leer_campo (farch_out, campo);
DATCICLO.C_inicial[i]->z=atof(campo);
leer_campo (farch_out, campo);
DATCICLO.C_inicial[i]->vx=atof(campo);
leer_campo (farch_out, campo);
DATCICLO.C_inicial[i]->vy=atof(campo);
leer_campo (farch_out, campo);
DATCICLO.C_inicial[i]->vz=atof(campo);
}
}

main (int argc, char* argv[])
{
  int t,iteraciones,tamano=0,N=0;
char campo[200];
char *salida;
double tiempo=0;
long i;

CUERPO **arreglo_inicial=NULL, **arreglo_final=NULL;
CUERPO *particula,*particula2;
  pthread_t *thread;                 /* hilos */
  pthread_attr_t attr;
//  pthread_mutex_init (&mutex1, NULL);   /* inicializa el mutex */
  FILE *farch_in,*farch_out, *jFile;
salida=argv[2];
   if (argc != 5){
     fprintf(stderr,"Uso %s arch_in arch_out iteraciones delta_t\n", argv[0]);
    exit(1);
	}

iteraciones=atoi(argv[3]);
delta_t=atof(argv[4]);
  /*Aqui se obtiene delta_t de los argumentos de entrada*/
  if (delta_t <= 0){
      fprintf(stderr,"delta_t debe ser un numero mayor a cero, usted puso %s\n",argv[4]);
      exit(1);
    } 

///////////////////////////////// AQUI SE LEE DEL ARCHIVO DE ENTRADA //////////////////////////////////////////
    if (farch_in = fopen(argv[1], "r"))
{
//        datos_in(farch_in,**particulas);

while (leer_campo (farch_in, campo))
        {
            particula = (CUERPO*) malloc (sizeof (CUERPO));
            particula2 = (CUERPO*) malloc (sizeof (CUERPO));
            particula->nombre = strdup (campo);
            leer_campo (farch_in, campo);
            particula->masa = atof (campo);
            leer_campo (farch_in, campo);
            particula->x = atof (campo);
            leer_campo (farch_in, campo);
            particula->y = atof (campo);
            leer_campo (farch_in, campo);
            particula->z = atof (campo);
            leer_campo (farch_in, campo);
            particula->vx = atof (campo);
            leer_campo (farch_in, campo);
            particula->vy = atof (campo);
            leer_campo (farch_in, campo);
            particula->vz = atof (campo);
            arreglo_inicial = (CUERPO**) arreglo_insertar ((void**)arreglo_inicial, &tamano, particula);
	    particula2->nombre = particula->nombre;
	    particula2->masa = particula->masa;
	    particula2->x = particula->x;
	    particula2->y = particula->y;
	    particula2->z = particula->z;
	    particula2->vx = particula->vx;
	    particula2->vy = particula->vy;
	    particula2->vz = particula->vz;
	    tamano--;
            arreglo_final = (CUERPO**) arreglo_insertar ((void**)arreglo_final, &tamano, particula2);
//	    printf("%i %s\n",N,arreglo_inicial[N]->nombre);
	    N++;
        }
        fclose(farch_in);
	printf("Numero de particulas: %i\n",N);
	iHilos=N;
	DATCICLO.C_inicial=arreglo_inicial;
	DATCICLO.C_final=arreglo_final;
//	    printf("%i %s\n",N-1,arreglo_inicial[N-2]->nombre);
//	    printf("%i %f\n",N-1,arreglo_inicial[N-2]->vz);

}
    else{
      printf("No se puede abrir el archivo\n");
}
//Se crean aloja el espacio para los N hilos
  thread = malloc(iHilos*sizeof(pthread_t));

/// SE ALOJA LA MEMORIA PARA LA CONFIGURACION FINAL



   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
/////////////////////////////////////////////////// AQUI VA EL CALCULO //////////////////////////////////// 
///////////////////// SALIDA/////////////////////////
farch_out = fopen(argv[2], "w+");
jFile = fopen("EVOLUCION.xyz", "w");
/*
if ((offset = fseek(farch_out, 0L, SEEK_END)) == -1)
      perror("lseek() error");
printf("of0:%i %i\n",offset,temporal);
*/
///imprimimos para t=0//
t=0;
imprimir(farch_out,tiempo);
imprimir_jmol(jFile);
 ////////////////CICLOS EN EL TIEMPO///////////////////
for(t=1;t<=iteraciones;t++){
leer(farch_out);
tiempo=t*delta_t;
  for (i=0;i<iHilos;i++)   /* crea los hilos */  ///ESCLAVO DEBE SER LA FUNCION DE RICARDO///
          if (pthread_create(&thread[i], &attr, trayectoria, (void *)i) != 0)     
                  perror ("pthread_create");
 
  for (i=0;i<iHilos;i++)             /* Une los hilos */
          if (pthread_join(thread[i],NULL)!=0)
                  perror ("pthread_join");
////////IMPRESION A ARCHIVO////////
imprimir(farch_out,tiempo);
imprimir_jmol(jFile);
////////////////////////////////////////////////
//printf("TIEMPO: %f\n",tiempo);
}

printf("Tiempo final: %f\n",tiempo);
//////////////////////////////////////////////////////////////////////
fclose(farch_out);
fclose(jFile);

 free(particula);
 free(particula2);
 free(arreglo_inicial);
 free(arreglo_final);
}



