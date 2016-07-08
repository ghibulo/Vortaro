/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
** kompililu: kiu ajn ANSI-C kompatibila
**
** korpo:     sxlosiloj.c
** Cxi tio korpo enhavas objekton 'sxlosiloj', kiu estas uzata por memori, ordigi
** kaj konservi gravajn vortojn (bezonata por sercxo)
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/



#include <string.h>
#include <malloc.h>
#include "malloko.h"
#include "sxlosiloj.h"

char *rangxitaZono; /*pro la 'merge sorto' - kie estos sercxotaj objektoj*/

#ifdef  DEBUG
   extern FILE *memErarsercxilo;
   extern long int sumo;
#endif

/*-----------------------------------------------------------------------------------
** Inicialo de strukturo SXLOSILOJ 
**         - okupu memoron kaj agordu konstantojn.
**-----------------------------------------------------------------------------------*/
SXLOSILOJ* inicSxlosiloj(void) {
   SXLOSILOJ *sxl = miaMalloc("inicSxlosiloj;sxl",sizeof(SXLOSILOJ));
   sxl->sxlosteksto = miaMalloc("inicSxlosiloj;sxl->sxlosteksto",tdimensio1*(sizeof(char)));
   sxl->sxlosnombro = miaMalloc("inicSxlosiloj;sxl->sxlosnombro",ndimensio1*(sizeof(long int)));
   sxl->tekstdimensio = tdimensio1;
   sxl->nombrdimensio = ndimensio1;
   sxl->tekstmntr = 0; /*teksta montrilo*/
   sxl->nombrmntr = 0; /*nombra montrilo*/
   return(sxl);
}

/*-----------------------------------------------------------------------------------
** Aldoni cetera sxlosilo:
**   sxlosilo     - teksto de grava sercxota teksto
**   eksterDateno - situo de eksteraj tekstoj en la fakto
**-----------------------------------------------------------------------------------*/
void aldonuSxlosilo(SXLOSILOJ *sxl, char *sxlosilo, long int eksterDateno) {
	char dmt;
   if (sxlosilo==NULL) return;
   dmt = strlen(sxlosilo)+1+sizeof(long int);
   if ((sxl->tekstmntr +dmt) >sxl->tekstdimensio) {
      char *nova;
      sxl->tekstdimensio+=tdimensio2;
      nova = miaMalloc("aldonuSxlosilo;nova",sxl->tekstdimensio*sizeof(char));
      memcpy(nova,sxl->sxlosteksto,sxl->tekstmntr);
      miaFree("aldonuSxlosilo;sxl->sxlosteksto",sxl->sxlosteksto);
      sxl->sxlosteksto=nova;
   }
   if ((sxl->nombrmntr+1)>sxl->nombrdimensio) {
      long int *nova;
      sxl->nombrdimensio+=ndimensio2;
      nova = miaMalloc("aldonuSxlosilo;nova",sxl->nombrdimensio*sizeof(long int));
      memcpy(nova,sxl->sxlosnombro,sxl->nombrmntr*sizeof(long int));
      miaFree("aldonuSxlosilo;sxl->sxlosnombro",sxl->sxlosnombro);
      sxl->sxlosnombro=nova;
   }
   strcpy(sxl->sxlosteksto + sxl->tekstmntr,sxlosilo);
   sxl->sxlosnombro[sxl->nombrmntr++] = sxl->tekstmntr;
   sxl->tekstmntr+=dmt;
   *((long int*)(sxl->sxlosteksto+sxl->tekstmntr-sizeof(long int))) = eksterDateno;

}


/*-----------------------------------------------------------------------------------
** 3 funkcioj: rangxiordigi, rangxu, rangxuSxlosiloj, kiuj modelas 'merge sorto'
** nombroj en zono 'sxlosnombro' estas rangxata laux tekstoj en 'sxlosteksto'
**-----------------------------------------------------------------------------------*/
void rangxiordigi(long int *A,long int p, long int q, long int r) {

   long int n1=q-p+1;
   long int n2=r-q;
   long int *L = miaMalloc("rangxiordigi;L",n1*sizeof(long int));
   long int *R = miaMalloc("rangxiordigi;R",n2*sizeof(long int));
   long int k;
   long int i=0;
   long int j=0;

   memcpy(L,A+p,n1*sizeof(long int));
   memcpy(R,A+q+1,n2*sizeof(long int));
   for (k=p;k<=r;k++) {
      if ((i==n1)||(j==n2)) {
         if (j==n2) {
            A[k]=L[i++];
         } else {
            A[k]=R[j++];
         }
      } else {
         if  (strcmp( rangxitaZono + L[i], rangxitaZono + R[j])<0) {
            A[k]=L[i++];
         } else {
            A[k]=R[j++];
         }
      }
   }/*for*/
   miaFree("rangxiordigi;L",L);
   miaFree("rangxiordigi;R",R);
}/*rangxiordigi(long int *A,long int p, long int q, long int r)*/


void rangxu(long int *A,long int p, long int r) {
   if (p<r) {
      long int q= (p+r)/2;
      rangxu(A,p,q);
      rangxu(A,q+1,r);
      rangxiordigi(A,p,q,r);
   }
}

void rangxuSxlosiloj(SXLOSILOJ *sxl) {
   
   rangxitaZono = sxl->sxlosteksto;
   rangxu(sxl->sxlosnombro,0, sxl->nombrmntr-1);
   /*qsort((void*)sxl->sxlosnombro,sxl->nombrmntr-1, sizeof(long int), rangxi);*/
}

/*-----------------------------------------------------------------------------------
** Konservi strukturon SXLOSILOJ en dosiero f
** 
**-----------------------------------------------------------------------------------*/
void konservuSxlosiloj(SXLOSILOJ *sxl, FILE *f) {
   fwrite(&(sxl->nombrmntr),sizeof(long int),1,f);  /*kvanto de nombroj*/
   fwrite(sxl->sxlosnombro, sizeof(long int), sxl->nombrmntr,f);
   fwrite(&(sxl->tekstmntr),sizeof(long int),1,f); /*kvanto de karakteroj*/
   fwrite(sxl->sxlosteksto, sizeof(char), sxl->tekstmntr,f);
}

/*-----------------------------------------------------------------------------------
** Surekranigi strukturon SXLOSILOJ de dosiero f
** 
**-----------------------------------------------------------------------------------*/
SXLOSILOJ* surekranigiSxlosiloj(FILE *f) {
   SXLOSILOJ *sxl = miaMalloc("surekranigiSxlosiloj;sxl",sizeof(SXLOSILOJ));
   fread(&(sxl->nombrdimensio),sizeof(long int),1,f);
   sxl->sxlosnombro = miaMalloc("surekranigiSxlosiloj;sxl->sxlosnombro",sxl->nombrdimensio*(sizeof(long int)));
   fread(sxl->sxlosnombro,sizeof(long int),sxl->nombrdimensio,f);

   fread(&(sxl->tekstdimensio),sizeof(long int),1,f);
   sxl->sxlosteksto = miaMalloc("surekranigiSxlosiloj;sxl->sxlosteksto",sxl->tekstdimensio*(sizeof(char)));
   fread(sxl->sxlosteksto,sizeof(char),sxl->tekstdimensio,f);

   sxl->tekstmntr = sxl->tekstdimensio; /*teksta montrilo*/
   sxl->nombrmntr = sxl->nombrdimensio; /*nombra montrilo*/
   return sxl;
}

/*-----------------------------------------------------------------------------------
** Malbloki memoron, strukturo SXLOSILOJ estos ruinigita.
** 
**-----------------------------------------------------------------------------------*/
void fermuSxlosiloj(SXLOSILOJ **sxl) {
   miaFree("fermuSxlosiloj;(*sxl)->sxlosnombro",(*sxl)->sxlosnombro);
   miaFree("fermuSxlosiloj;(*sxl)->sxlosteksto",(*sxl)->sxlosteksto);
   miaFree("fermuSxlosiloj;*sxl",*sxl);
   *sxl = NULL;
}

