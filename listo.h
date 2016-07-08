/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
**
** cxapo:     listo.h
** kompililu: kiu ajn ANS-C kompatibila
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/



#ifndef H_LISTO_H
#define H_LISTO_H

/*-----------------------------------------------------------------------------------
** strukturo por unu elemento en la listo
-----------------------------------------------------------------------------------*/
typedef struct s_elementlisto {
   void * data;
   struct s_elementlisto *sekva;
   struct s_elementlisto *antauxa;
} ELEMENT_LISTO;

/*-----------------------------------------------------------------------------------
** strukturo por surstakigo de datoj
-----------------------------------------------------------------------------------*/
typedef struct listo {
   ELEMENT_LISTO *unua;
   ELEMENT_LISTO *lasta;
   ELEMENT_LISTO *montrilo;
} LISTO;

/*-----------------------------------------------------------------------------------
** Inicialo de strukturo LISTO - 
**         - okupu memoron kaj agordu konstantojn
**-----------------------------------------------------------------------------------*/
LISTO* inicListo (void);

/*-----------------------------------------------------------------------------------
** Sxovu la montrilo al la sekva elemento
**-----------------------------------------------------------------------------------*/
int sekvaListo(LISTO *l);

/*-----------------------------------------------------------------------------------
** Sxovu la montrilo al la sekva elemento. Se montrilo estas cxe la lasta,
** sxovu gxin al la unua (kiel ringo).
**-----------------------------------------------------------------------------------*/
void sekvaRingoListo(LISTO *l);


/*-----------------------------------------------------------------------------------
** Sxovu la montrilo al la antauxa elemento
**-----------------------------------------------------------------------------------*/
int antauxaListo(LISTO *l);



/*-----------------------------------------------------------------------------------
** Sxovu la montrilo al la antauxa elemento. Se montrilo estas cxe la unua,
** sxovu gxin al la lasta (kiel ringo).
**-----------------------------------------------------------------------------------*/
void antauxaRingoListo(LISTO *l);


/*-----------------------------------------------------------------------------------
** Sxovu la montrilo absolute:
** setListo(L,1) - al la unua elemento
** setListo(L,4) - al la kvara elemento
** setListo(L,-1) - al la lasta elemento
** setListo(L,-2) - al la antauxa lasta elemento
**-----------------------------------------------------------------------------------*/
int setListo(LISTO *l,long int wh);

/*-----------------------------------------------------------------------------------
** Legu datoj tie, kie estas la montrilo. Nenio estas neniita,
** cxio en la listo restas sama.
**-----------------------------------------------------------------------------------*/
void* leguMontriloListo(LISTO *l);



/*-----------------------------------------------------------------------------------
** Aldonu sekvan daton post la elemento 'where'. Kiam where==NULL, aldonu gxin
** post la lasta elemento
**-----------------------------------------------------------------------------------*/
void aldonuPostListo(LISTO *l, void * data, ELEMENT_LISTO *where);



/*-----------------------------------------------------------------------------------
** nulListo(L,L->montrilo): 
**   Legu datoj tie, kie estas la montrilo kaj nuligu gxin de la listo.
** nulListo(L,L->unua): 
**   Legu datoj de la unua elemento, kaj nuligu gxin de la listo.
** nulListo(L,L->lasta): 
**   Legu datoj de la lasta elemento, kaj nuligu gxin de la listo.
**-----------------------------------------------------------------------------------*/
void* nulListo(LISTO *l, ELEMENT_LISTO *kiu);

/*-----------------------------------------------------------------------------------
** Neniigu cxiujn datojn en la listo
**-----------------------------------------------------------------------------------*/
void* nulTutaListo(LISTO *l);

/*-----------------------------------------------------------------------------------
** Realigu 'nulTutaListo' kaj neniigu plenan liston.
** Al la parametro *l agordu NULL.
**-----------------------------------------------------------------------------------*/
void neniiguListo(LISTO **l);

/*-----------------------------------------------------------------------------------
** Listo estas plena    -> 1 (vero)
** Listo ne estas plena -> 0 (malvero)
**-----------------------------------------------------------------------------------*/
#define cxuMalplenaListo(l) ((l)->unua==NULL)

/*-----------------------------------------------------------------------------------
** Montrilo estas post la lasta elemento        -> 0 (malvero)
** Montrilo ne estas post la lasta elemento     -> 1 (vero)
**-----------------------------------------------------------------------------------*/
#define cxuNePostLastaListo(l) ((l)->montrilo != NULL)


#if 0
/* ---------------------portempe neestas utila por la vortaro------------------------*/

#define cxuLastaListo(l) ((l)->montrilo->sekva == NULL)
#define cxuUnuaListo(l) ((l)->montrilo->antauxa == NULL)


/*-----------------------------------------------------------------------------------
** Aldonu sekvan daton antaux la elemento 'where'. Kiam where==NULL, aldonu gxin
** antaux la unua elemento
**-----------------------------------------------------------------------------------*/
void aldonuAntauxListo(LISTO *l, void * data, ELEMENT_LISTO *where);

/* ---------------------portempe neestas utila por la vortaro------------------------*/
#endif




#endif
