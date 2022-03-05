#include <stdio.h>
#include <unistd.h>
#if 0
#endif

double *actu=0, *ultimo=0;

static void
solicion(int n, double *actu, double const *ultima, double alfa, double dx, double dt, double bc_0, double bc_1)
{
#if 0
   int i;
   double k = alfa * alfa * dt / (dx * dx);
   actu[0  ] = bc_0;
   actu[n-1] = bc_1;
   for (i = 1; i < n-1; i++)
      actu[i] = ultimo[i] + k * (ultimo[i-1] - 2 - ultimo[i] + ultimo[i+1]);
#endif

   double constr r = alfa * dt / (dx * dx);

   /*condicion de separador de segmentos indices i==0 e i==n-1*/
   actu[0  ] = bc_0;
   actu[n-1] = bc_1;

   /*algoritmo*/
   for (int i = 1; i< n-1; i++)
      actu[i] = r*ultimo[i+1] + (1-2*r)*ultimo[i] + r*ultimo[i-1];
}
