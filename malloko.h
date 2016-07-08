#include <stdio.h>
/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
**
** cxapo:     malloko.h
** kompililu: kiu ajn ANS-C kompatibila
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/


/*-----------------------------------------------------------------------------------
**sxangxu al DEBUG, se vi volas sxalti MemArangxo 
-----------------------------------------------------------------------------------*/
#define NDEBUG

/*-----------------------------------------------------------------------------------
** longo de linio en csv-dosiero
** uzita en kodado.c(konvertuKodado); vortaro.c(ellaboruCSV, ceteraLinio) 
**-----------------------------------------------------------------------------------*/
#define LINLONGO 1000

/*-----------------------------------------------------------------------------------
** Bloku memoro por 't' kaj kopu teksto de 'f' al 't'
**-----------------------------------------------------------------------------------*/
#define cpString(t,f) {int tmp=strlen(f);(t)=((char *)miaMalloc("cpString;t",tmp+1));strcpy(t,f);t[tmp]='\0';}

/*-----------------------------------------------------------------------------------
** miaFopen/miaFclose estas anstataux fopen/fclose
** kiam oni ne povas labori kun dosiero, tiam impresu informon kaj terminu aplikajxon.
**-----------------------------------------------------------------------------------*/
#define miaFopen(variablo,nomo,parametro) if ((variablo=fopen(nomo,parametro)) == NULL) eraro(4,nomo);
#define miaFclose(variablo) if (fclose(variablo)==EOF) eraro(5,NULL);

/*-----------------------------------------------------------------------------------
** Terminu aplikajxo
**-----------------------------------------------------------------------------------*/
void eraro(char n, char *novajxo);




#ifdef  DEBUG
   FILE *memErarsercxilo;
   long int sumo;

/*-----------------------------------------------------------------------------------
** Inicialo de memora kontrolado
**-----------------------------------------------------------------------------------*/
   void inicMemArangxo();

/*-----------------------------------------------------------------------------------
** Bloku sekvan memoron kaj registri kvanto de gxi
**-----------------------------------------------------------------------------------*/
   void* miaMalloc(char *noto, size_t dimensio);

/*-----------------------------------------------------------------------------------
** Malbloku memoron kaj registri kvanto de gxi
**-----------------------------------------------------------------------------------*/
   void miaFree(char *noto, void *montrilo);

   /*-----------------------------------------------------------------------------------
** Skribu la finkalkulo en dosiero - memErarsercxilo, malfermu gxin.
**-----------------------------------------------------------------------------------*/
   void finMemArangxo();
#else

   #define miaMalloc(t,s) malloc(s)
   #define miaFree(t,s) free(s)

   #define inicMemArangxo() ;
   #define finMemArangxo() ;
#endif
