/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
** kompililu: kiu ajn ANSI-C kompatibila
**
** korpo:     listo.c
** Cxi tio korpo enhavas ilojn por memori objektojn en listo. Estas bezonata por
** memori resultatojn de sercxojn.
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/




#include <stdlib.h>
#include "malloko.h"
#include "listo.h"

#ifdef  DEBUG
   extern FILE *memErarsercxilo;
   extern long int sumo;
#endif


/*-----------------------------------------------------------------------------------
** Inicialo de strukturo LISTO - 
**         - okupu memoron kaj agordu konstantojn
**-----------------------------------------------------------------------------------*/
LISTO*  inicListo (void) {
   LISTO *nova;
   nova = (LISTO *) miaMalloc("inicListo;nova",sizeof(LISTO));
   nova->unua = NULL;
   nova->lasta  = NULL;
   nova->montrilo = NULL;
   return nova;
}

/*-----------------------------------------------------------------------------------
** Sxovu la montrilo al la sekva elemento
**-----------------------------------------------------------------------------------*/
int sekvaListo(LISTO *l) {
   if (l->montrilo!=NULL) {
      l->montrilo = l->montrilo->sekva;
      return 1;
   }
   return 0;
}


/*-----------------------------------------------------------------------------------
** Sxovu la montrilo al la sekva elemento. Se montrilo estas cxe la lasta,
** sxovu gxin al la unua (kiel ringo).
**-----------------------------------------------------------------------------------*/
void sekvaRingoListo(LISTO *l) {
     sekvaListo(l);
     if (l->montrilo==NULL)
         l->montrilo = l->unua;
}

/*-----------------------------------------------------------------------------------
** Sxovu la montrilo al la antauxa elemento
**-----------------------------------------------------------------------------------*/
int antauxaListo(LISTO *l) {
   if (l->montrilo!=NULL) {
      l->montrilo = l->montrilo->antauxa;
      return 1;
   }
   return 0;
}

/*-----------------------------------------------------------------------------------
** Sxovu la montrilo al la antauxa elemento. Se montrilo estas cxe la unua,
** sxovu gxin al la lasta (kiel ringo).
**-----------------------------------------------------------------------------------*/
void antauxaRingoListo(LISTO *l) {
     antauxaListo(l);
     if (l->montrilo==NULL)
         l->montrilo= l->lasta;
}


/*-----------------------------------------------------------------------------------
** Sxovu la montrilo absolute:
** setListo(L,1) - al la unua elemento
** setListo(L,4) - al la kvara elemento
** setListo(L,-1) - al la lasta elemento
** setListo(L,-2) - al la antauxa lasta elemento
**-----------------------------------------------------------------------------------*/
int setListo(LISTO *l,long int wh) {
   if (wh>=1) {
      long int j=1;
      for(l->montrilo = l->unua;(l->montrilo!=NULL)&&(j<wh);j++) {
         l->montrilo=l->montrilo->sekva;
      }
   }
   if (wh<=-1) {
      long int j=1;
      for(l->montrilo = l->lasta;(l->montrilo!=NULL)&&(j<-wh);j++) {
         l->montrilo=l->montrilo->antauxa;
      }
   }
   if (l->montrilo==NULL)
      return 0;
   else
      return 1;
}

/*-----------------------------------------------------------------------------------
** Legu datoj tie, kie estas la montrilo. Nenio estas neniita,
** cxio en la listo restas sama.
**-----------------------------------------------------------------------------------*/
void* leguMontriloListo(LISTO *l) {
   if (l->montrilo!=NULL)
      return l->montrilo->data;
   else
      return NULL;
}

/*-----------------------------------------------------------------------------------
** Aldonu sekvan daton post la elemento 'where'. Kiam where==NULL, aldonu gxin
** post la lasta elemento
**-----------------------------------------------------------------------------------*/
void aldonuPostListo(LISTO *l, void * data, ELEMENT_LISTO *where) {
   ELEMENT_LISTO *nova;
   nova= (ELEMENT_LISTO *) miaMalloc("aldonuPostListo;nova",sizeof(ELEMENT_LISTO));
   nova->data = data;
   if (where==NULL) { /*aldonu post la lasta*/
      if (l->lasta==NULL) { 
         l->lasta = l->unua = nova;
         nova->antauxa = nova->sekva = NULL;
      } else {
         l->lasta->sekva=nova;
         nova->sekva=NULL;
         nova->antauxa=l->lasta;
         l->lasta = nova;
      }
   } else {
      ELEMENT_LISTO *tmp;
      tmp = where->sekva;
      where->sekva=nova;
      nova->antauxa=where;
      nova->sekva=tmp;
      if (tmp!=NULL) { 
         tmp->antauxa=nova;
      } else {
         l->lasta=nova;
      }
   }
}


/*-----------------------------------------------------------------------------------
** nulListo(L,L->montrilo): 
**   Legu datoj tie, kie estas la montrilo kaj nuligu gxin de la listo.
** nulListo(L,L->unua): 
**   Legu datoj de la unua elemento, kaj nuligu gxin de la listo.
** nulListo(L,L->lasta): 
**   Legu datoj de la lasta elemento, kaj nuligu gxin de la listo.
**-----------------------------------------------------------------------------------*/
void* nulListo(LISTO *l, ELEMENT_LISTO *kiu) {
   void *tmp=kiu->data;
   if (l->montrilo==kiu) { /*kdyz na nej zrovna ukazuje, tak dopredu, kdyz je posledni, tak dozadu*/
         if (l->montrilo->sekva!=NULL) 
            l->montrilo=l->montrilo->sekva;
         else
            l->montrilo=l->montrilo->antauxa;
   }
   if (l->unua==kiu) {
      l->unua=l->unua->sekva;
      if (l->unua!=NULL) 
         l->unua->antauxa=NULL;
      else
         l->lasta=NULL;

      miaFree("nulListo;kiu(unua)",kiu);
      return tmp;
   }
   if (l->lasta==kiu) {
      l->lasta=l->lasta->antauxa;
      l->lasta->sekva=NULL;
      miaFree("nulListo;kiu(dua)",kiu);
      return tmp;
   }
   kiu->antauxa->sekva=kiu->sekva;
   kiu->sekva->antauxa=kiu->antauxa;
   miaFree("nulListo;kiu(tria)",kiu);
   return tmp;
}


/*-----------------------------------------------------------------------------------
** Neniigu cxiujn datojn en la listo
**-----------------------------------------------------------------------------------*/
void* nulTutaListo(LISTO *l) {
   if (l!=NULL) {
      while (l->unua!=NULL) {
         void *pt = nulListo(l,l->unua);
         miaFree("nulTutaListo;pt",pt);
      }
   }
   return l;
}

/*-----------------------------------------------------------------------------------
** Realigu 'nulTutaListo' kaj neniigu plenan liston.
** Al la parametro *l agordu NULL.
**-----------------------------------------------------------------------------------*/
void neniiguListo(LISTO **l) {
   if (*l!=NULL) {
      nulTutaListo(*l);
      miaFree("neniiguListo;*l",*l);
      *l=NULL;
   }
}

#if 0
/* ---------------------portempe neestas utila por la vortaro------------------------*/






/*-----------------------------------------------------------------------------------
** Aldonu sekvan daton antaux la elemento 'where'. Kiam where==NULL, aldonu gxin
** antaux la unua elemento
**-----------------------------------------------------------------------------------*/
void aldonuAntauxListo(LISTO *l, void * data, ELEMENT_LISTO *where) {

   ELEMENT_LISTO *nova;
   nova= (ELEMENT_LISTO *) miaMalloc("aldonuAntauxListo;nova",sizeof(ELEMENT_LISTO));
   nova->data = data;
   if (where==NULL) { /*antaux la unua*/
      if (l->unua==NULL) { 
         l->lasta = l->unua = nova;
         nova->antauxa = nova->sekva = NULL;
      } else {
         l->unua->antauxa=nova;
         nova->antauxa=NULL;
         nova->sekva=l->unua;
         l->unua = nova;
      }
   } else {
      ELEMENT_LISTO *tmp;
      tmp = where->antauxa;
      where->antauxa=nova;
      nova->sekva=where;
      nova->antauxa=tmp;
      if (tmp!=NULL) { 
         tmp->sekva=nova;
      } else {
         l->unua=nova;
      }

   }
}
/* ---------------------portempe neestas utila por la vortaro------------------------*/
#endif


