/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
**
** cxapo:     sxlosiloj.h
** kompililu: kiu ajn ANS-C kompatibila
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/



#ifndef H_SXLOSILOJ_H
#define H_SXLOSILOJ_H

/* kiom memoro estos blokata de objekto SXLOSILOJ - la unuan fojon*/
#define tdimensio1 4194304 
#define ndimensio1 262144

/* kiom memoro estos blokata de objekto SXLOSILOJ - poste*/
#define tdimensio2 65536
#define ndimensio2 65536



/*-----------------------------------------------------------------------------------
** Strukturo SXLOSILOJ por rangxi, surekranigi kaj konservi sxlosilojn
** de faktoj
**-----------------------------------------------------------------------------------*/
typedef struct  {
   char *sxlosteksto; /*zono de teksto*/
   long int tekstdimensio; /*dimensio de tekstzono*/
   long int *sxlosnombro; /*zono de nombroj*/
   long int nombrdimensio; /*dimensio de nombrzono*/
   long int tekstmntr; /*teksta montrilo*/
   long int nombrmntr; /*nombra montrilo*/
} SXLOSILOJ;

/*-----------------------------------------------------------------------------------
** Inicialo de strukturo SXLOSILOJ 
**         - okupu memoron kaj agordu konstantojn.
**-----------------------------------------------------------------------------------*/
SXLOSILOJ *inicSxlosiloj(void);

/*-----------------------------------------------------------------------------------
** Aldoni cetera sxlosilo:
**   sxlosilo     - teksto de grava sercxota teksto
**   eksterDateno - situo de eksteraj tekstoj en la fakto
**-----------------------------------------------------------------------------------*/
void aldonuSxlosilo(SXLOSILOJ *sxlosiloj, char *sxlosilo, long int eksterDateno);

/*-----------------------------------------------------------------------------------
** 3 funkcioj: rangxiordigi, rangxu, rangxuSxlosiloj, kiuj modelas 'merge sorto'
** nombroj en zono 'sxlosnombro' estas rangxata laux tekstoj en 'sxlosteksto'
**-----------------------------------------------------------------------------------*/
void rangxuSxlosiloj(SXLOSILOJ *sxlosiloj);

/*-----------------------------------------------------------------------------------
** Konservi strukturon SXLOSILOJ en dosiero f
** 
**-----------------------------------------------------------------------------------*/
void konservuSxlosiloj(SXLOSILOJ *sxlosiloj, FILE *f);


/*-----------------------------------------------------------------------------------
** Surekranigi strukturon SXLOSILOJ de dosiero f
** 
**-----------------------------------------------------------------------------------*/
SXLOSILOJ* surekranigiSxlosiloj(FILE *f);

/*-----------------------------------------------------------------------------------
** Malbloki memoron, strukturo SXLOSILOJ estos ruinigita.
** 
**-----------------------------------------------------------------------------------*/
void fermuSxlosiloj(SXLOSILOJ **sxl);



#endif
