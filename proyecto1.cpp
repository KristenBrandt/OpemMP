#include <unistd.h>     //std lib
#include <iostream>
#include <cmath>
#include <fstream>      //fstream, ofstream, ifstream
#include <string>       //string
#include <omp.h>

using namespace std;






int main(int argc, char * argv[]) {

  //Captura de datos de la terminal
    if (argc == 1)
      {
        printf("Usage: proyecto dX Tl T0 Tr #tasks\n\n");
        printf("Donde: \n\tdX = Tamaño de intervalos en cm enteros (la vara mide 10000 cm)\n\tTl = Temperatura inicial de la izquierda\n\tT0 = Temperatura inicial de la vara\n\tTr = Temperatura inicial de la derecha\n\ttasks = Cantidad de threads\n");
        exit (1);
      }
    int dX = atoi(argv[1]); // Tamaño de intervalos de la barra
    int Tl = atoi(argv[2]); // Temperatura izquierda
    int T0 = atoi(argv[3]); // Temperatura inicial de vara
    int Tr = atoi(argv[4]); // Temperatura derecha
    int T = atoi(argv[5]); // Tasks

    float e = 0.05; // Variable de criterio de salida
    float maximo = 1; // Variable de control de salida
    float antes;
    float dif;

    //Size of arrays
    int N = 10000/dX;

    cout<< "Tamanio de array: "<<N;
    cout<<endl;

    //Variables de iteracion
    int j;
    int i;
    int o;
    int p;

    // Constantes de la ecuacion

    float dXpow = pow( static_cast< float >(dX),2);
    float k = pow(10,-5);

    //omp_set_num_threads(T);

  //Calcular dt
    float dt = 0.4*dXpow/k;

  //Inicializar Array 1, que representa la vara de metal <- PARALELIZABLE

    float * Ar1 = new float[N];

    Ar1[0] = static_cast< float >(Tl);
    Ar1[N-1] = static_cast< float >(Tr);

    for(i = 1; i < N-1; i++)
    {
      Ar1[i] = static_cast< float > (T0);
    }

  //Inicializar Array 2, que se usa para guardar temporalmente valores de temperatura nuevos
    float * Ar2 = new float[N];


  // Print array 1 inicializado por control
  printf("Vara inicializada:\n");
  for (p = 0; p < N; p++)
    cout << Ar1[p]<<" ";
  printf("\n");


  //While que evalua criterio de salida
   while (maximo >= e) {
     maximo = 0.0;

     //for para actualizar la temperatura <- PARALELIZABLE
     for (j = 1; j < N-1; j++) {

       antes = Ar1[j];
       Ar2[j] = Ar1[j] + (k*dt/dXpow)*(Ar1[j-1]-2*Ar1[j]+Ar1[j+1]);
       dif = abs(antes - Ar2[j]);
       if(dif > maximo) {
         maximo = dif;
       }
     }

     // for para actualizar arreglo de respuesta <- PARALELIZABLE
     for (o = 1; o < N-1; o++) {

       Ar1[o] = Ar2[o];

     }
   }



  //Print Array 1 convergido

  printf("Vara convergida:\n");
  for (p = 0; p < N; p++){
    cout << Ar1[p] << " ";
  }
  printf("\n");


    return 0;

  }
