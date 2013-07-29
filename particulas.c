/*
particulas.c
               N numero de particulas
*/
#include <stdio.h>
#include <stdlib.h>
 #include <string.h>
#include <pthread.h>

long lTam = 1000;
int  iHilos;
 
/* Datos compartidos */
int *a;                   /* arreglo de numeros a sumar */
int indice_global = 0;     /* indice global */
unsigned long suma = 0;              /* resultado final, también utilizado por los esclavos */
pthread_mutex_t mutex1;    /* variable de bloqueo mutuamente exclusiva */
 

 typedef struct {
  char* nombre;
  double masa;
  double x;
  double y;
  double z;
  double vx;
  double vy;
  double vz;}CUERPO; 

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
  int i,tamano=0,N=0;
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
///////////////////////////////// AQUI SE LEE DEL ARCHIVO DE ENTRADA //////////////////////////////////////////
    if (farch_in = fopen(argv[1], "r"))
{
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



