#include <stdio.h>
#include <math.h>

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

static double dt=0.001;
static unsigned int N=3;

///////////////////////////////////////////////////////////////
/*Funcion que saca la distancia al cuadrado entre dos cuerpos*/
///////////////////////////////////////////////////////////////
double dist2(CUERPO cuerpo1, CUERPO cuerpo2){
  return pow(cuerpo2.x-cuerpo1.x,2)+pow(cuerpo2.y-cuerpo1.y,2)+pow(cuerpo2.z-cuerpo1.z,2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/*Funcion que calcula el siguiente paso del cuerpo n en funcion de todas las posiciones anteriores*/
/*para la particula i.                                                                            */
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

void main(){
  CUERPO arreglo[3];
  CUERPO uno={1,2,3,4,5,6};
  CUERPO dos={6,5,3,2,4,5};
  CUERPO tres={0,1,32,2,3,4};
  CUERPO final;
  double vec_de_masas[3]={100,1,5};
  arreglo[0]=uno;
  arreglo[1]=dos;
  arreglo[2]=tres;
  trayectoria(arreglo, &final, vec_de_masas,1.1);
  printf("%f\t%f\t%f\t%f\t%f\t%f\n",uno.x,uno.y,uno.z,uno.vx,uno.vy,uno.vz);
  printf("%f\t%f\n",arreglo[0].x,arreglo[1].x);
}
