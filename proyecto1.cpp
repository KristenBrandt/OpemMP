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


    // Calculo de N es la cantidad de intervalos discretos,  el numerador es el largo de la barra y el denominador es el tamaño de las distancias discretas
    int N = 10000/dX;

    cout<<"Tamanio de array: "<<N;
    cout<<endl;

    //Variables de iteracion
    int j;
    int i;
    int o;
    int p;
    int q;

    // Constantes de la ecuacion

    float dXpow = pow( static_cast< float >(dX),2);
    float k = pow(10,-5);

    omp_set_num_threads(T);

  //Calcular dt
    float dt = 0.2*dXpow/k;

  //Inicializar Array 1, que representa la vara de metal <- PARALELIZABLE (NO se paralelizo porque incrementa el runtime)

    float * Ar1 = new float[N];

    Ar1[0] = static_cast< float >(Tl);
    Ar1[N-1] = static_cast< float >(Tr); // Las temperaturas en los extremos de la vara los introducimos directamente


    for(i = 1; i < N-1; i++)
    {
      Ar1[i] = static_cast< float > (T0); // La temperatura en el resto de la vara lo hacemos por medio de este for
    }

  //Inicializar Array 2, que se usa para guardar temporalmente valores de temperatura nuevos
    float * Ar2 = new float[N];

  //Inicializar Array de dT, para actualizar criterio de salida
  float * diferencias = new float[N];

  // Print array 1 inicializado por control
    printf("Vara inicializada:\n");
    for (p = 0; p < N; p++)
      cout << Ar1[p]<<" ";
    printf("\n");


  //While que evalua criterio de salida
     while (maximo >= e) {
       maximo = 0.0; // Cada vuelta del while se resetea la variable de control

       //for para actualizar la temperatura <- PARALELIZABLE
       #pragma omp parallel for schedule(static)

         for (j = 1; j < N-1; j++) {

           Ar2[j] = Ar1[j] + (k*dt/dXpow)*(Ar1[j-1]-2*Ar1[j]+Ar1[j+1]); // calculamos nueva temperatura
           diferencias[j] = abs(Ar1[j] - Ar2[j]); // guardamos dT

         }
         // for para actualizar arreglo de respuesta <- PARALELIZABLE
         #pragma omp parallel for schedule(static)
         for (o = 1; o < N-1; o++) {

           Ar1[o] = Ar2[o];

         }

         // for para calculo de maximo que nos indica si el while debe terminar <- NO PARALELIZABLE
        #pragma critical
        for (q = 1; q < N-1; q++){

          if (diferencias[q] > maximo){
            maximo = diferencias[q]; // buscamos max(dT) para determinar si convergio
          }

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
