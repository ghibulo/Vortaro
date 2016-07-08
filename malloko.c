/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
** kompililu: kiu ajn ANSI-C kompatibila
**
** korpo:     malloko.c
** Cxi tio korpo estas komuna por cxioj ceteraj korpoj. 
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/




#include <stdlib.h>
#include <malloc.h>
#include "malloko.h"

/*-----------------------------------------------------------------------------------
** Terminu aplikajxo
**-----------------------------------------------------------------------------------*/
void eraro(char n, char *novajxo) {
   switch (n) {
      case 1:  printf("Eraro en skribi/legi dosieron\n");break; 
      case 2:  printf("Malbona CSV dosiero\n");break;
      case 3:  printf("Memoro estas elcxerpita!");break;
      case 4:  printf("Eraro - mi ne povas malfermi dosiero:\n");break;
      case 5:  printf("Eraro - mi ne povas fermi dosiero!\n");break;
      default: printf("Eraro!");break;
   }
   if (novajxo!=NULL) printf(novajxo);
   printf("\n");
   exit(n);
}

#ifdef  DEBUG
   extern FILE *memErarsercxilo;
   extern long int sumo;

/*-----------------------------------------------------------------------------------
** Inicialo de memora kontrolado
**-----------------------------------------------------------------------------------*/
   void inicMemArangxo() {
      memErarsercxilo = fopen("erarsercxilo.txt","w");
      sumo=0;
   }

/*-----------------------------------------------------------------------------------
** Bloku sekvan memoron kaj registri kvanto de gxi
**-----------------------------------------------------------------------------------*/
   void* miaMalloc(char *noto, size_t dimensio) {
      register void *resulto;
      struct mallinfo mi1,mi2;
      int i;

      mi1 = mallinfo();
      resulto = malloc(dimensio);
      mi2 = mallinfo();
      i=mi2.uordblks-mi1.uordblks;
      sumo+=i;

      if (resulto==0) {
         fclose(memErarsercxilo);
         eraro(3,NULL);
      }

      fprintf(memErarsercxilo,"Malloc: adreso: %#lX, dimensio: %d, Uzita memoro: %ld\n",(long int)resulto,i,sumo);


      fprintf(memErarsercxilo,"%s\n",noto);
      return resulto;
   }

/*-----------------------------------------------------------------------------------
** Malbloku memoron kaj registri kvanto de gxi
**-----------------------------------------------------------------------------------*/
   void miaFree(char *noto, void *montrilo) {
      struct mallinfo mi1, mi2;
      int i;
      mi1 = mallinfo();
      free(montrilo);
      mi2 = mallinfo();
      i=mi1.uordblks-mi2.uordblks;
      sumo-=i;
      fprintf(memErarsercxilo,"Free: adreso: %#lX, dimensio: %d, Uzita memoro: %ld\n",(long int)montrilo,i,sumo);
      fprintf(memErarsercxilo,"%s\n",noto);
   }

/*-----------------------------------------------------------------------------------
** Skribu la finkalkulo en dosiero - memErarsercxilo, malfermu gxin.
**-----------------------------------------------------------------------------------*/
   void finMemArangxo() {
      fprintf(memErarsercxilo,"-----------------------\n");
      if (sumo==0) {
         fprintf(memErarsercxilo,"Gratulas al vi, cxiu memoro estas malblokita!\n");
      } else {
         fprintf(memErarsercxilo,"Bedauxras, sed io ne estas malblokita: %ld\n",sumo);
      }

      miaFclose(memErarsercxilo);
   }

#endif


