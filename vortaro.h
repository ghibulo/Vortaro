/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
**
** cxapo:     vortaro.h
** kompililu: kiu ajn ANS-C kompatibila
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/


#ifndef H_VORTARO_H
#define H_VORTARO_H

#include "malloko.h"
#include "listo.h"


/*-----------------------------------------------------------------------------------
** maksimumo de faktos en linio (de csv-dosiero)
-----------------------------------------------------------------------------------*/
#define MAKSLINFS 10

/*-----------------------------------------------------------------------------------
** nombro de partos en fakto
-----------------------------------------------------------------------------------*/
#define NPARTF 9

/*-----------------------------------------------------------------------------------
**maksimuma longitudo de vorto
-----------------------------------------------------------------------------------*/
#define VORTOLONGO 100

/*-----------------------------------------------------------------------------------
**defaulta nomo por la dat-dosiero
-----------------------------------------------------------------------------------*/
#define defaultDATnomo "espvortaro.dat"

/*-----------------------------------------------------------------------------------
** defaulta situo kaj nomo por la dat-dosiero
-----------------------------------------------------------------------------------*/
char *defaultDATsituo;

/*-----------------------------------------------------------------------------------
** partoj de la FAKTO 
-----------------------------------------------------------------------------------*/
typedef enum {
   sxlosilo1, sxlosilo2, vortradiko, esperante,
   fonto, fako, cxehxe, noto, komparu
} ENUMFAKTO;

/*-----------------------------------------------------------------------------------
**fakto estas NPARTF textoj (sxlosiloj, vortradiko, noto...)
-----------------------------------------------------------------------------------*/
typedef char* FAKTO[NPARTF];

/*-----------------------------------------------------------------------------------
**strukturo uzita por la konverto csv-dosiero -> dat-dosiero
-----------------------------------------------------------------------------------*/
typedef struct {
   FILE *CSVdosiero;
   FILE *DATdosiero;
   SXLOSILOJ *sercxsxlosiloj, *reasercxsxlosiloj;
   char kodado;
} KONVERTO;

/*-----------------------------------------------------------------------------------
** strukturo uzita por la traduko cxion, kion la uzulo volas 
-----------------------------------------------------------------------------------*/
typedef struct {
   FILE *DATdosiero;
   SXLOSILOJ *sercxsxlosiloj, *reasercxsxlosiloj;
   long int montrilo;
   SXLOSILOJ *lastasercxsxlosiloj;
   LISTO *sercxitaj;
} TRADUKADO;

/*-----------------------------------------------------------------------------------
** Inicialo de strukturo KONVERTO (pro konverti CSV-dosiero -> DAT-dosiero 
**         - okupu memoron, agordu konstantojn, malfermu dosieroj, inicu sxlosiloj
**-----------------------------------------------------------------------------------*/
KONVERTO* inicKonverto (char *csvNomo, char *datNomo, char kodado); 

/*-----------------------------------------------------------------------------------
** Donu cxion pro la estigo dat-dosiero (ellaboru csv-dosiero, konservu sxlosiloj kaj
** fermu dat-dosiero
**-----------------------------------------------------------------------------------*/
void donuKonverto(KONVERTO *konverto);

/*-----------------------------------------------------------------------------------
** Legu eksteraj tekstoj de fakton.  Montrilo de la dosiero f devas esti agordita.
**-----------------------------------------------------------------------------------*/
FAKTO* leguFakto(FILE *f);

/*-----------------------------------------------------------------------------------
** Malbloki memoron, strukturo FAKTO estos ruinigita.
**-----------------------------------------------------------------------------------*/
void neniiguFakto(FAKTO *f);

/*-----------------------------------------------------------------------------------
** Ellaboru csv-dosieron, cxiu linio estas partigita al partoj de fakton,
** kaj kusxigita al dat-dosiero. Unua kaj dua parto estas kusxigita en 
** strukturo SXLOSILOJ. Cxi tiuj partoj estas ankaux kusxigitaj inverse. 
**-----------------------------------------------------------------------------------*/
void ellaboruCSV(KONVERTO *konverto);


/*-----------------------------------------------------------------------------------
** Konstatu situo de la eksteraj partoj en dat dosiero, 
**-----------------------------------------------------------------------------------*/
long int konstatuPozicioEnDATdosiero(TRADUKADO *t, long int mntr);

/*-----------------------------------------------------------------------------------
** Estigi inversan vorton
**-----------------------------------------------------------------------------------*/
char* reiguVorton(const char* vorto);

/*-----------------------------------------------------------------------------------
** Konstatu situo de la eksteraj partoj en dat dosiero, 
** agordu montrilo de la dat-dosiero
**-----------------------------------------------------------------------------------*/
char preparuDATdosiero(TRADUKADO *t);

/*-----------------------------------------------------------------------------------
** Impresu unu fakton al la monitoro
**-----------------------------------------------------------------------------------*/
void impresuFakton(FAKTO *f);

/*-----------------------------------------------------------------------------------
** Malbloki memoron, strukturo FAKTO estos ruinigita.
**-----------------------------------------------------------------------------------*/
void neniiguFakton(FAKTO *f);


/*-----------------------------------------------------------------------------------
** Impresu sercxita vorto frapante
**-----------------------------------------------------------------------------------*/
void impresuTitolon(char *titolo);


/*-----------------------------------------------------------------------------------
** Kusxigu la tekston 't' en la dosiero 'd'
**-----------------------------------------------------------------------------------*/
#define kusxiguTeksto(t,d) {unsigned char TEMP_=(t==NULL)?0:strlen(t);fwrite((&(TEMP_)),sizeof(char),1,d);fwrite(t,sizeof(char),(TEMP_),(d));}

/*-----------------------------------------------------------------------------------
** Legu la tekston 't' en la dosiero 'd'
**-----------------------------------------------------------------------------------*/
#define leguTeksto(t,d) {unsigned char TEMP_;fread((&(TEMP_)),sizeof(char),1,d);t=(char*)miaMalloc("leguTekxto;t",TEMP_*sizeof(char)+1);fread(t,sizeof(char),TEMP_,d);t[TEMP_]='\0';}

/*-----------------------------------------------------------------------------------
** Trovu long - nombron, post la teksto 'str'
**-----------------------------------------------------------------------------------*/
#define leguLONGINTpostSTR(str,li) {while (*str++);li= (*((long int *)str));}

/*-----------------------------------------------------------------------------------
** Nomoj de partoj en FAKTO
**-----------------------------------------------------------------------------------*/
#define nomoENUMFAKTO0 "E-sxlosilo"
#define nomoENUMFAKTO1 "C-sxlosilo"
#define nomoENUMFAKTO2 " Vortradiko"
#define nomoENUMFAKTO3 "Esperante "
#define nomoENUMFAKTO4 "Fonto     "
#define nomoENUMFAKTO5 "Fako      "
#define nomoENUMFAKTO6 "Cxehxe    "
#define nomoENUMFAKTO7 "Noto      "
#define nomoENUMFAKTO8 "Komparu   "

/*-----------------------------------------------------------------------------------
** Por la impreso de fakto -> 'cxehxe' estos impresata antaux 'fonto'
** Uzita en 'impresuFakton'
**-----------------------------------------------------------------------------------*/
#define ordo(i) ((i==4)?6:(i==6)?4:i)



#endif
