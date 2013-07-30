/*
particulas.c
               N numero de particulas
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

/*Constante gravitacional (depende de las unidades a usar)*/
#define const_G 1

long lTam = 1000;
int  iHilos;
 
/* Datos compartidos */
int *a;                    /* arreglo de numeros a sumar */
int indice_global = 0;     /* indice global */
unsigned long suma = 0;    /* resultado final, también utilizado por los esclavos */
pthread_mutex_t mutex1;    /* variable de bloqueo mutuamente exclusiva */
static double dt;          /* Declaracion de delta t */
 

 typedef struct {
  char* nombre;
  double masa;
  double x;
  double y;
  double z;
  double vx;
  double vy;
  double vz;}CUERPO; 

///////////////////////////////////////////////////////////
/*Funciones que sirven para hacer el calculo de los pasos*/
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
/*Funcion que saca la distancia al cuadrado entre dos cuerpos*/
///////////////////////////////////////////////////////////////
double dist2(CUERPO cuerpo1, CUERPO cuerpo2){
  return pow(cuerpo2.x-cuerpo1.x,2)+pow(cuerpo2.y-cuerpo1.y,2)+pow(cuerpo2.z-cuerpo1.z,2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/*Funcion que calcula el siguiente paso del cuerpo n en funcion de todas las posiciones anteriores*/
/*para la particula i. Requiere implicitamente del numero N de particulas y del incremento dt     */
////////////////////////////////////////////////////////////////////////////////////////////////////
void trayectoria(CUERPO pt0[0], CUERPO *pt1, int i){
  int j;
  double m_r3;

  pt1->vx*=0;  //Inicializa en ceros para poder hacer la suma de las fuerzas
  pt1->vy*=0;  //una por una.
  pt1->vz*=0;  //

  //Suma de fuerzas particula por particula, componente a componente
  for( j=0 ; j<N ; j++){
    if(j!=i){
      m_r3=pt0[j].masa/pow(dist2(pt0[i],pt0[j]),1.5);
      pt1->vx+=m_r3*(pt0[j].x-pt0[i].x);
      pt1->vy+=m_r3*(pt0[j].y-pt0[i].y);
      pt1->vz+=m_r3*(pt0[j].z-pt0[i].z);
    }
  }
  
  //Multiplicacion por G y dt
  pt1->vx*=const_G*dt;
  pt1->vy*=const_G*dt;
  pt1->vz*=const_G*dt;

  //Se le agregan las velocidades del paso temporal anterior
  pt1->vx+=pt0[i].vx;
  pt1->vy+=pt0[i].vy;
  pt1->vz+=pt0[i].vz;

  //Se modifican las posiciones 
  pt1->x=pt0[i].x+pt0[i].vx*dt;
  pt1->y=pt0[i].y+pt0[i].vy*dt;
  pt1->z=pt0[i].z+pt0[i].vz*dt;
}



void *esclavo (void *ignorado)  /* Hilos esclavos */
{
  int indice_local;
  unsigned long suma_parcial = 0;
 
  do {
          pthread_mutex_lock (&mutex1);  /* obtiene el siguiente indice del arreglo */
          indice_local =  indice_global; /* Lee el indice actual y lo guarda */
          indice_global++;
          pthread_mutex_unlock (&mutex1);
 
          if (indice_local < lTam)
                  suma_parcial += *(a+indice_local);
  }
  while (indice_local < lTam);
 
 
  pthread_mutex_lock (&mutex1);   /* Agrega la suma parcial a la suma global */
     suma += suma_parcial;
  pthread_mutex_unlock(&mutex1);
 
  return (NULL);                      /* El hilo termina */
}

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


main (int argc, char* argv[])
{
  int i,tamano=0,N=0; //N es el numero de cuerpos
  char campo[20];
  double tiempo=0;
  CUERPO *particula,**arreglo_particulas=NULL;
  pthread_t *thread;                 /* hilos */
  pthread_mutex_init (&mutex1, NULL);   /* inicializa el mutex */
  FILE *farch_in,*farch_out;

  if (argc != 5){
    fprintf(stderr,"Uso %s arch_in arch_out iteraciones delta_t\n", argv[0]);
    exit(1);
  }

  /*Aqui se obtiene dt de los argumentos de entrada*/
  if (dt=atof(argv[4] <= 0){
      fprintf(stderr,"delta_t debe ser un numero mayor a cero, usted puso %s\n",argv[4]);
      exit(1);
    }

///////////////////////////////// AQUI SE LEE DEL ARCHIVO DE ENTRADA //////////////////////////////////////////
  if (farch_in = fopen(argv[1], "r")){
//        datos_in(farch_in,**particulas);

while (leer_campo (farch_in, campo))
        {
            particula = (CUERPO*) malloc (sizeof (CUERPO));
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
            arreglo_particulas = (CUERPO**) arreglo_insertar ((void**)arreglo_particulas, &tamano, particula);
//	    printf("%i %s\n",N,arreglo_particulas[N]->nombre);
	    N++;
        }
        fclose(farch_in);
	printf("Numero de particulas: %i\n",N);
	iHilos=N;
//	    printf("%i %s\n",N-1,arreglo_particulas[N-2]->nombre);
//	    printf("%i %f\n",N-1,arreglo_particulas[N-2]->vz);

}
    else{
      printf("No se puede abrir el archivo\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  if (argc>2) {
//     lTam = atol(argv[2]);
//  }
 
  a = malloc(lTam*sizeof(int));
//Se crean aloja el espacio para los N hilos
  thread = malloc(iHilos*sizeof(pthread_t));
/////////////////////////////////////////////////// AQUI VA EL CALCULO //////////////////////////////////// 
  for (i=0;i<lTam;i++)                /* inicializa a[] */
          a[i]=i+1;
 
  for (i=0;i<iHilos;i++)             /* crea los hilos */
          if (pthread_create(&thread[i],NULL,esclavo,NULL) != 0)
                  perror ("pthread_create");
 
  for (i=0;i<iHilos;i++)             /* Une los hilos */
          if (pthread_join(thread[i],NULL)!=0)
                  perror ("pthread_join");
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  printf ("La suma de 1 hasta %lu es %lu\n", (unsigned long) lTam, suma);
///////////////////// SALIDA/////////////////////////
   if (farch_out = fopen(argv[2], "w"))
{
//        datos_in(farch_in,**particulas);
	fprintf(farch_out,"tiempo %f\n", tiempo);
for(i=0;i<N;i++){
	fprintf(farch_out,"%s %f %f %f %f %f %f %f\n",arreglo_particulas[i]->nombre,arreglo_particulas[i]->masa,arreglo_particulas[i]->x,arreglo_particulas[i]->y,arreglo_particulas[i]->z,arreglo_particulas[i]->vx,arreglo_particulas[i]->vy,arreglo_particulas[i]->vz);
//printf("%i\n",i);
//printf("%s\n",arreglo_particulas[5]->nombre);
}
        fclose(farch_out);
}
    else{
      printf("No se puede abrir el archivo\n");
}
////////////////////////////////////////////////////////////
 free(particula);
 free(arreglo_particulas);
}
