/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
**
** cxapo:     kodado.h
** kompililu: kiu ajn ANS-C kompatibila
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/


#ifndef H_KODADO_H
#define H_KODADO_H



/*-----------------------------------------------------------------------------------
** Strukturo KONVKODADO (por konverti UTF-8 teksto -> teksto cxehxa  
** kodita (Windows 1250/CP852/ISO-8859-2))
**-----------------------------------------------------------------------------------*/
typedef struct  {
   char *de; /*kiuj sercxas*/
   char *en; /*al kiuj sxangxi*/
} KONVKODADO;



/*-----------------------------------------------------------------------------------
** Inicialo de strukturo KONVKODADO 
**         - okupu memoron kaj agordu konstantojn.
**         - konverto == 'u' ... konvertu nenion 
**         - konverto == 'w' ... konvertu UTF-8 -> Windows-1250
**         - konverto == 'i' ... konvertu UTF-8 -> ISO-8859-2 (ISO Latin 2)
**         - konverto == 'c' ... konvertu UTF-8 -> CP852 (PC Latin 2)
**-----------------------------------------------------------------------------------*/
KONVKODADO* inicKodado(char konverto);

/*-----------------------------------------------------------------------------------
** Donu konverton - teksto 'kio' estos konvertita laux 'kiel', poste teksto 'kio' 
** estos ruinigita, kaj konvertita teksto estos eligo
**-----------------------------------------------------------------------------------*/
char* konvertuKodado(KONVKODADO *kiel, char *kio);

/*-----------------------------------------------------------------------------------
** Malbloki memoron, strukturo KONVKODADO estos ruinigita
**-----------------------------------------------------------------------------------*/
void neniiguKodado (KONVKODADO **kiu);



#endif
