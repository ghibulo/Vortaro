/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
** kompililu: kiu ajn ANSI-C kompatibila
**
** korpo:     vortaro.c
** Cxi tio korpo enhavas main kaj objektojn: KONVERTO (por konverti CSV dosiero
** al DAT dosiero), TRADUKADO (por traduki vortojn helpe de DAT dosiero)
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sxlosiloj.h"
#include "malloko.h"
#include "vortaro.h"
#include "kodado.h"

extern char *defaultDATsituo;

#ifdef  DEBUG
   extern FILE *memErarsercxilo;
   extern long int sumo;
#endif


/*-----------------------------------------------------------------------------------
** Inicialo de strukturo KONVERTO (pro konverti CSV-dosiero -> DAT-dosiero 
**         - okupu memoron, agordu konstantojn, malfermu dosieroj, inicu sxlosiloj
**-----------------------------------------------------------------------------------*/
KONVERTO* inicKonverto (char *csvNomo, char *datNomo,char kodado) {
   KONVERTO *nova = (KONVERTO *) miaMalloc("inicKonverto;nova",sizeof(KONVERTO));
   long int fino=0;
   miaFopen(nova->CSVdosiero,csvNomo,"r");
   miaFopen(nova->DATdosiero,datNomo,"wb");
   nova->sercxsxlosiloj = inicSxlosiloj();
   nova->reasercxsxlosiloj = inicSxlosiloj(); /*reigita sxlosiloj, rengxita de laux vortfinajxo*/
   nova->kodado = kodado;

   fwrite(&fino,sizeof(long int),1,nova->DATdosiero);
   return nova;
}


/*-----------------------------------------------------------------------------------
** Donu cxion pro la estigo dat-dosiero (ellaboru csv-dosiero, konservu sxlosiloj kaj
** fermu dat-dosiero
**-----------------------------------------------------------------------------------*/
void donuKonverto(KONVERTO *k) {
   long int montrilo;
   ellaboruCSV(k);
   miaFclose(k->CSVdosiero);
   rangxuSxlosiloj(k->sercxsxlosiloj);
   rangxuSxlosiloj(k->reasercxsxlosiloj);
   montrilo = ftell(k->DATdosiero);
   konservuSxlosiloj(k->sercxsxlosiloj, k->DATdosiero);
   konservuSxlosiloj(k->reasercxsxlosiloj,k->DATdosiero);
   if (fseek(k->DATdosiero,0,SEEK_SET)) {
      fclose(k->DATdosiero);
      eraro(1,"Problemo pro la konverto CSV->DAT");
   }
   fwrite(&montrilo,sizeof(long int),1,k->DATdosiero);
   miaFclose(k->DATdosiero);
}

/*-----------------------------------------------------------------------------------
** Malbloki memoron, strukturo KONVERTO estos ruinigita.
** 
**-----------------------------------------------------------------------------------*/
void fermuKonverto(KONVERTO **k) {
   fermuSxlosiloj(&((*k)->sercxsxlosiloj));
   fermuSxlosiloj(&((*k)->reasercxsxlosiloj));
   miaFree("fermuKonverto;k",*k);
   *k=NULL;
}


/*-----------------------------------------------------------------------------------
** Kiam unu fakto ne estas en unu linio, tiu cxi funkcio longigas gxin
**-----------------------------------------------------------------------------------*/
char* ceteraLinio(FILE *f, char **linio, char **s, char *cet, KONVKODADO *kod) {
   char *cetlin;
   char *tmp;
   if (++*cet>4) eraro(2,*linio); /*unu fakto pli longa ol 4 linio -> eraro*/
   cetlin = (char*)miaMalloc("ceteraLinio;cetlin",(*cet)*LINLONGO);
   tmp = cetlin;
   while (**s) *tmp++ = *(*s)++;
   miaFree("ceteraLinio;*linio",*linio);
   *linio=cetlin;
   *s=cetlin;
   if (kod!=NULL) {
      char *tmpln=(char*)miaMalloc("ceteraLinio;tmpln",LINLONGO*sizeof(char));
      char *j, *i;
      j=fgets(tmpln,LINLONGO,f);
      i= tmpln =konvertuKodado(kod, tmpln);
      while (*i) *tmp++ = *i++;
      *tmp='\0';
      miaFree("ceteraLinio;tmpln",tmpln);
      return j;
   }
   return fgets(tmp,LINLONGO,f);
}

/*-----------------------------------------------------------------------------------
** Sercxu plia parto (eligo de funkcio) kaj terminu gxin de la '\0'
**-----------------------------------------------------------------------------------*/
char* partigiSxlosiloj(char **teksto) {
   char *s = *teksto;
   char *f;
   char limo;
   if (s==NULL) {
      return NULL;
   }
   while ((*s!='.')&&(*s!=',')&&(*s)) {
      s++;
   }
   if (!*s) {
      *teksto=NULL;
      return NULL;
   }
   limo=*s;
   f=s+1;
   while ((*f!=limo)&&(*f)) {
                           f++;
   }
   if (!*f) {
      *teksto=NULL; 
      return NULL;
   }
   if (!*(f+1)) 
            *teksto=NULL;
   *(f+1)='\0';
   if (*teksto!=NULL) 
            *teksto=f+2;
   return s;
}

/*-----------------------------------------------------------------------------------
** Ellaboru csv-dosieron, cxiu linio estas partigita al partoj de fakton,
** kaj kusxigita al dat-dosiero. Unua kaj dua parto estas kusxigita en 
** strukturo SXLOSILOJ. Cxi tiuj partoj estas ankaux kusxigitaj inverse. 
**-----------------------------------------------------------------------------------*/

void ellaboruCSV(KONVERTO *konverto) {
   char *linio; /*unu linio de teksto (el DATdosiero)*/
   int j,cet, i=0;
   char *tmp;
   char* s; /*komenco de teksto*/
   char* f; /*fino de teksto*/
   long pozicio; /*pozicio en DATdosiero*/
   long debu=0, debpozicio;
   char *vorto, *revorto;
   KONVKODADO *kod;
   linio=(char*)miaMalloc("ellaboruCSV;linio",LINLONGO*sizeof(char));
   kod = (konverto->kodado=='u')?NULL:inicKodado(konverto->kodado);

   while (fgets(linio,LINLONGO,konverto->CSVdosiero)!=NULL) { 
      if (kod!=NULL) {
         linio = konvertuKodado(kod, linio);
      }
      s=strstr(linio,"\".");      /*sercxu komenco de fakto*/
      #ifdef DEBUG
         printf("%ld: %s\n", ++debu, s);
      #endif
      while (s!=NULL) { 
         pozicio = ftell(konverto->DATdosiero);
         #ifdef DEBUG
            printf("%ld\n",pozicio);
         #endif
         for(j=0;j<NPARTF;j++) {
            if ((*s)=='\"') {
               ++s;
               do { /* f ne devas esti NULL*/
                  f=strchr(s,'\"'); /*ne unua \" */
                  if (f==NULL) { cet=1; ceteraLinio(konverto->CSVdosiero,&linio,&s,(char*)&cet,kod);}
               } while (f==NULL); 
               if (*(f+1)=='\"') { /*salti \"\" */
                  do {
                        char *tmpch;
                        f+=2;
                        cet=1;
                        do { /* f ne devas esti NULL*/
                           tmpch=strchr(f,'\"');
                           if (tmpch==NULL) {  
                              long int diferenco = f-s;
                              ceteraLinio(konverto->CSVdosiero,&linio,&s,(char*)&cet,kod);
                              f=s+diferenco;
                           }
                        } while (tmpch==NULL); 
                        f=tmpch;
                     } while (*(f+1)=='\"');
               }
               *f='\0'; /*pro la strcpy*/
               tmp = s;
               /*strcpy(elfaktos[i][j],s);*/
               s=f+2;
            } else { /*vaka teksto*/
               tmp=NULL;
               s++;
            }
            switch(j) {
               case sxlosilo1:
               case sxlosilo2: 
                        while (tmp!=NULL) {
                           vorto=partigiSxlosiloj(&tmp);
                           if (vorto!=NULL) {
                              revorto = reiguVorton(vorto);
                              aldonuSxlosilo(konverto->sercxsxlosiloj, vorto, pozicio);
                              aldonuSxlosilo(konverto->reasercxsxlosiloj, revorto, pozicio);
                              miaFree("ellaboruCSV;revorto",revorto);
                           }
                        }
                        break;
               case vortradiko:
               case esperante:
               case fonto:
               case fako:
               case cxehxe:
               case noto:
               case komparu: kusxiguTeksto(tmp,konverto->DATdosiero);break;
               default: eraro(0,"donuFaktos, variablo j");
            }/*switch*/
            debpozicio = ftell(konverto->DATdosiero);
         }/*for*/
         /*i++;printf("%d\n",i);*/
         s=strstr(s,"\"."); /*sercxu komenco de fakto*/
      }/*while (s!=NULL)*/
   } /*while (fgets(linio,LINLONGO,konverto->CSVdosiero)!=NULL) */
   miaFree("ellaboruCSV;linio",linio);
   neniiguKodado(&kod);
}/*ellaboruCSV*/


/*-----------------------------------------------------------------------------------
** Estigi inversan vorton
**-----------------------------------------------------------------------------------*/
char* reiguVorton(const char* vorto) {
   char *n=(char*)vorto;
   char* rezulto;
   char i;
   while (*n++)
   ;
   rezulto=miaMalloc("reiguVorton;rezulto",sizeof(char)*(n-vorto));
   i = 0;
   n--;
   while (n--!=vorto) {
      rezulto[i++]=*n;
   }
   rezulto[i]='\0';
   return rezulto;
}


void trovuDATsituon(char *argv0) {
   char *mntrfino, *mntr;
   char i;
   char lnomo = strlen(defaultDATnomo);
   if ((mntrfino = strrchr(argv0,'\\'))==NULL) {
      mntrfino = strrchr(argv0,'/');
   }
   defaultDATsituo = miaMalloc("trovuDATsituon;defaultDATsituo",lnomo+(mntrfino-argv0)+3);
   mntr=argv0;i=0;
   while (mntr<=mntrfino) {
      defaultDATsituo[i++]=*mntr++;
   }
   strcpy(defaultDATsituo+i,defaultDATnomo);
}


/*-----------------------------------------------------------------------------------
** Malfermu csv/dat - dosierojn, donu konverto, fermu cxion
** 
**-----------------------------------------------------------------------------------*/
void konvertu(char *tCSVnomo, char *tDATnomo, char kodado) {
   /*
   char *CSVnomo = (char*) miaMalloc("konvertu;CSVnomo",(strlen(tCSVnomo)+1)*sizeof(char));
   */
   char *DATnomo;
   /*= (tDATnomo==NULL)? (char*) miaMalloc("konvertu;DATnomo;tDATnomo==NULL",15*sizeof(char)): (char*) miaMalloc("konvertu;DATnomo;tDATnomo!=NULL",strlen(tDATnomo)+1);*/
   KONVERTO *miaKonv;
   /*strcpy(CSVnomo,tCSVnomo);*/

   if (tDATnomo==NULL) {
      cpString(DATnomo,defaultDATsituo);
   } else {
      cpString(DATnomo,tDATnomo);
   }
   printf("Momenton! Mi devas konverti CSV dosiero - tio povas dauxri longe...\n");
   miaKonv =inicKonverto(tCSVnomo,DATnomo,kodado);
   donuKonverto(miaKonv);
   fermuKonverto(&miaKonv);
   /*miaFree("konvertu;CSVnomo",CSVnomo);*/
   miaFree("konvertu;DATnomo",DATnomo);
   printf("Konverto finigxis...\n");
}

/*-----------------------------------------------------------------------------------
** Inicialo de strukturo TRADUKADO (surekranigi dat-dosieron, inicu sxlosiloj,
** okupu memoron kaj agordu konstantojn.
**-----------------------------------------------------------------------------------*/

TRADUKADO* inicTradukado(char *aDATnomo) {
   TRADUKADO *nova = (TRADUKADO*)miaMalloc("inicTradukado;nova",sizeof(TRADUKADO));
   long int komencoSxl=0;
   if (aDATnomo==NULL) {
      miaFopen(nova->DATdosiero,defaultDATsituo,"rb");
   } else {
      miaFopen(nova->DATdosiero,aDATnomo,"rb");
   }
   if (fseek(nova->DATdosiero,komencoSxl,SEEK_SET)!=0) {
      fclose(nova->DATdosiero);
      eraro(1,"Mi ne povas labori kun DAT-dosiero");
   }
   fread(&(komencoSxl),sizeof(long int),1,nova->DATdosiero);
   if (fseek(nova->DATdosiero,komencoSxl,SEEK_SET)!=0) {
      fclose(nova->DATdosiero);
      eraro(1,"Mi ne povas labori kun DAT-dosiero");
   }
   nova->sercxsxlosiloj=surekranigiSxlosiloj(nova->DATdosiero);
   nova->reasercxsxlosiloj=surekranigiSxlosiloj(nova->DATdosiero);
   nova->lastasercxsxlosiloj=NULL;
   nova->montrilo=0;
   nova->sercxitaj=NULL;
   return nova;
}


/*-----------------------------------------------------------------------------------
** Traduku vorton, en kiu unua kaj lasta letero ne estas '.'/','. Tralegu cxiujn 
** lauxvice, sercxitaj memoru en listo 'sercxitaj'
**-----------------------------------------------------------------------------------*/
void lantaTraduko(TRADUKADO *t, char *vorto) {
   SXLOSILOJ *sxl = t->sercxsxlosiloj;
   char *vrt;
   long int *enlisto;
   long int i,maks;
   if (t->sercxitaj!=NULL) {
      nulTutaListo(t->sercxitaj);
   } 
   t->sercxitaj=inicListo();
   maks=sxl->nombrdimensio;
   for(i=0;i<maks;i++) {
      vrt = sxl->sxlosteksto+(sxl->sxlosnombro[i]);
      if (NULL!=strstr(vrt,vorto)) {
         enlisto=(long int*)miaMalloc("lantaTraduko;enlisto",sizeof(long int));
         leguLONGINTpostSTR(vrt,*enlisto)
         aldonuPostListo(t->sercxitaj, enlisto,NULL);
      }

   }
   setListo(t->sercxitaj,1);
}

/*-----------------------------------------------------------------------------------
** Traduku vorton, en kiu unua aux lasta letero estas '.'/','. Sercxu en rangxita zono 
** (sercxsxlosiloj/resercxsxlosiloj) pere de duonigi de intervaloj
**-----------------------------------------------------------------------------------*/
FAKTO* traduku(TRADUKADO *t, char *vorto) {
   SXLOSILOJ *sxl = NULL;
   char* deb;
   char *tmp;
   char *sercxataVorto=NULL;
   long int mntr,dl;

   
   tmp=vorto;
   while (*tmp++)
   ;
   tmp-=2;

   if ((*vorto==',')||(*vorto=='.')) {
      sxl=t->lastasercxsxlosiloj=t->sercxsxlosiloj;
      cpString(sercxataVorto,vorto);
   } else {
      if ((*tmp==',')||(*tmp=='.')) {
         sxl=t->lastasercxsxlosiloj=t->reasercxsxlosiloj;
         sercxataVorto = reiguVorton(vorto);
      }
   }
   
   if (sxl==NULL) {
      t->lastasercxsxlosiloj=NULL;
      lantaTraduko(t,vorto);
   }  else {
         long int l=0, d=sxl->nombrdimensio;
         int kompar;
         while ((dl=d-l)>1) {
            mntr = dl/2 + l;
            deb = sxl->sxlosteksto+(sxl->sxlosnombro[mntr]);
            kompar = strcmp(sxl->sxlosteksto+(sxl->sxlosnombro[mntr]),sercxataVorto);
            if (kompar<0) {
               l=mntr;
               continue;
            }
            if (kompar>0) {
               d=mntr;
               continue;
            }
            if (kompar==0) {
               break;
            }
         }
         /*if (dl<2) mntr=l;*/
         t->montrilo=mntr;
         miaFree("traduku;sercxataVorto",sercxataVorto);
   }/*else*/

   if (preparuDATdosiero(t)) {
      return leguFakto(t->DATdosiero);
   } else {
      return NULL;
   }
   
}

/*-----------------------------------------------------------------------------------
** Legu eksteraj tekstoj de fakton.  Montrilo de la dosiero f devas esti agordita.
**-----------------------------------------------------------------------------------*/
FAKTO* leguFakto(FILE *f) {
   FAKTO *fakto = (FAKTO*) miaMalloc("leguFakto;fakto",sizeof(FAKTO));
   unsigned char l;
   ENUMFAKTO i;
   (*fakto)[sxlosilo1]=(*fakto)[sxlosilo2]=NULL;
   for (i=vortradiko;i<=komparu;i++) {
      fread(&l,sizeof(char),1,f);
      (*fakto)[i]=(char*)miaMalloc("leguFakto;fakto",l*sizeof(char)+1);
      fread((*fakto)[i],sizeof(char),l,f);
      ((*fakto)[i])[l]='\0';
   }

   return fakto;

}

/*-----------------------------------------------------------------------------------
** Iru al la lefta apudulo de sercxita vorto
**-----------------------------------------------------------------------------------*/
FAKTO* irulefte(TRADUKADO *t) {
   if (t->lastasercxsxlosiloj==NULL) /*lantaTraduko*/ {
      if (cxuMalplenaListo(t->sercxitaj)) {
         return NULL;
      }
      antauxaRingoListo(t->sercxitaj);
   } else {
      long int tmp = t->montrilo-1;
      if (tmp>=0) {
         while ((tmp>=0)&&(konstatuPozicioEnDATdosiero(t,t->montrilo)==
                                                                  konstatuPozicioEnDATdosiero(t,--tmp)))
         ;
         t->montrilo=tmp+1;
      }
   }

   if (preparuDATdosiero(t)) {
      return leguFakto(t->DATdosiero);
   } else {
      return NULL;
   }
}

/*-----------------------------------------------------------------------------------
** Iru al la dekstra apudulo de sercxita vorto
**-----------------------------------------------------------------------------------*/
FAKTO* irudekstre(TRADUKADO *t) {
   if (t->lastasercxsxlosiloj==NULL) /*lantaTraduko*/ {
      if (cxuMalplenaListo(t->sercxitaj)) {
         return NULL;
      }
      sekvaRingoListo(t->sercxitaj);
   } else {
      long int tmp = t->montrilo+1;
      long int maks = t->sercxsxlosiloj->nombrmntr;
      if (tmp<maks) {
         while ((tmp<maks)&&(konstatuPozicioEnDATdosiero(t,t->montrilo)==
                                                                  konstatuPozicioEnDATdosiero(t,tmp++)))
         ;
         t->montrilo=tmp-1;
      }
   }

   if (preparuDATdosiero(t)) {
      return leguFakto(t->DATdosiero);
   } else {
      return NULL;
   }
}


/*-----------------------------------------------------------------------------------
** Impresu cxiujn apudulojn de la sercxita vorto. Por la ne interakta tradukado.
**-----------------------------------------------------------------------------------*/
void impresuApudulojn(TRADUKADO *t, char *vorto) {
   FAKTO *f;
   if (t->lastasercxsxlosiloj==NULL) /*lantaTraduko*/ {
      if (cxuMalplenaListo(t->sercxitaj)) {
         return;
      }
      setListo(t->sercxitaj,1);

      do {
         preparuDATdosiero(t);
         f=leguFakto(t->DATdosiero);
         impresuFakton(f);
         neniiguFakton(f);
         sekvaListo(t->sercxitaj);
      } while (cxuNePostLastaListo(t->sercxitaj));
   } else {
      long int tmp = t->montrilo+1;
      long int maks = t->sercxsxlosiloj->nombrmntr;
      SXLOSILOJ *sxl = t->lastasercxsxlosiloj;
      /*char *vorto = sxl->sxlosteksto+(sxl->sxlosnombro[t->montrilo]);*/
      char kondicxo;
      char *sercxataVorto=NULL;
      if (sxl==t->reasercxsxlosiloj) {
         sercxataVorto = reiguVorton(vorto);
      } else {
         cpString(sercxataVorto,vorto);
      }
      do {
         if (tmp<maks) {
            while ((tmp<maks)&&(konstatuPozicioEnDATdosiero(t,t->montrilo)==
                                                                     konstatuPozicioEnDATdosiero(t,tmp++)))
            ;
            t->montrilo=tmp-1;
         }
         if (kondicxo=(strstr(sxl->sxlosteksto+(sxl->sxlosnombro[t->montrilo]),sercxataVorto)!=NULL)) {
            preparuDATdosiero(t);
            f = leguFakto(t->DATdosiero);
            impresuFakton(f);
            neniiguFakton(f);
         }
      } while ((tmp<maks)&&(kondicxo));
      miaFree("impresuApudulojn;sercxataVorto",sercxataVorto);
   }/*else*/
   
}/*impresuApudulojn(TRADUKADO *t, char *vorto)*/


/*-----------------------------------------------------------------------------------
** Konstatu situo de la eksteraj partoj en dat dosiero, 
** agordu montrilo de la dat-dosiero
**-----------------------------------------------------------------------------------*/
char preparuDATdosiero(TRADUKADO *t) {
/*   SXLOSILOJ *sxl = t->sercxsxlosiloj;
   char *tmp = sxl->sxlosteksto+(sxl->sxlosnombro[t->montrilo]);
   while (*tmp++); */ /*al la fino de teksto*/
   long int pozicio;
   if (t->lastasercxsxlosiloj==NULL) /*lantaTraduko*/ {
      if (cxuMalplenaListo(t->sercxitaj)) {
         return 0;
      } else {
         pozicio = *(long int*)leguMontriloListo(t->sercxitaj);
      }
   } else {
         pozicio =konstatuPozicioEnDATdosiero(t,t->montrilo);
   }
   if (fseek(t->DATdosiero,pozicio,SEEK_SET)) {
      fclose(t->DATdosiero);
      eraro(1,"Mi ne povas labori kun DAT-dosiero");
   }     
   return 1;
}        

/*-----------------------------------------------------------------------------------
** Konstatu situo de la eksteraj partoj en dat dosiero, 
**-----------------------------------------------------------------------------------*/
long int konstatuPozicioEnDATdosiero(TRADUKADO *t, long int mntr) {
   char *tmp=t->lastasercxsxlosiloj->sxlosteksto+(t->lastasercxsxlosiloj->sxlosnombro[mntr]);
   long int resulto;
   #if 0 
   while (*tmp++); /*al la fino de teksto*/
   return *((long int *)tmp);
   #endif
   leguLONGINTpostSTR(tmp,resulto);
   return resulto;
}        

/*-----------------------------------------------------------------------------------
** Impresu unu parto de fakto
** 
**-----------------------------------------------------------------------------------*/
void impresunomoenumfakto(int i) {
   switch (i) {
      case 0: printf(nomoENUMFAKTO0);break;
      case 1: printf(nomoENUMFAKTO1);break;
      case 2: printf(nomoENUMFAKTO2);break;
      case 3: printf(nomoENUMFAKTO3);break;
      case 4: printf(nomoENUMFAKTO4);break;
      case 5: printf(nomoENUMFAKTO5);break;
      case 6: printf(nomoENUMFAKTO6);break;
      case 7: printf(nomoENUMFAKTO7);break;
      case 8: printf(nomoENUMFAKTO8);break;
      default: eraro(0,"Problemo en impresunomoenumfakto(int i)");break;
   }

}

/*-----------------------------------------------------------------------------------
** Impresu unu fakton al la monitoro
**-----------------------------------------------------------------------------------*/
void impresuFakton(FAKTO *f) {
   ENUMFAKTO i;
   if (f==NULL) {
      printf("Mi sercxas nenion!");
      return;
   }
   for(i=sxlosilo1;i<=komparu;i++) {
      if ((*f)[ordo(i)]!=NULL) {
         impresunomoenumfakto(ordo(i));
         printf(": \t %s  \n ",(*f)[ordo(i)]);
      }
   }
   printf("=======================\n");

}


/*-----------------------------------------------------------------------------------
** Malbloki memoron, strukturo FAKTO estos ruinigita.
**-----------------------------------------------------------------------------------*/
void neniiguFakton(FAKTO *f) {
   ENUMFAKTO i;
   if (f==NULL) return;
   for(i=sxlosilo1;i<=komparu;i++) {
      if ((*f)[i]!=NULL) 
                  miaFree("neniiguFakton;(*f)[i]",(*f)[i]);
   }
   miaFree("neniiguFakton;f",f);
}

/*-----------------------------------------------------------------------------------
** Malbloki memoron, strukturo TRADUKADO estos ruinigita.
** 
**-----------------------------------------------------------------------------------*/
void neniiguTradukadon(TRADUKADO **t) {
   miaFclose((*t)->DATdosiero);
   fermuSxlosiloj(&((*t)->sercxsxlosiloj));
   fermuSxlosiloj(&((*t)->reasercxsxlosiloj));
   neniiguListo(&((*t)->sercxitaj));
   miaFree("neniiguTradukadon;*t",*t);
   *t = NULL;
}


/*-----------------------------------------------------------------------------------
** Sercxu la plej proksiman sxlosilon, impresu fakton, atendu premi la klavon:
** 'n' - nova vorto, 'h' - lefta apudulo, 'l' - dekstra apudulo,
** CTRL+f aux CTRL+q - terminu aplikajxon
**-----------------------------------------------------------------------------------*/
void tradukadoInterakte (TRADUKADO *t) {
   
   char vorto[VORTOLONGO];
   char kr;
   FAKTO *f;
   while (1) {
      printf("Vorto por traduki (CTRL+q aux CTRL+f - fino de aplikajxo) : ");
      scanf("%s",vorto); while ('\n'!=getchar());
      if ((vorto[0]=='\021')||(vorto[0]=='\006')) break;
      kr='n';
      do {
         switch (kr) {
            case 'n': f=traduku(t,vorto);break;
            case 'h': f=irulefte(t);break;
            case 'l': f=irudekstre(t);break;
            default: f=NULL;
         }
         impresuFakton(f);
         printf("h-antauxa apudulo; l-sekva apudulo; n-nova vorto; CTRL+q aux CTRL+f - fino de aplikajxo\n");
         neniiguFakton(f);
         kr=getchar();
         while ('\n'!=getchar());
      } while ((kr!='n')&&(kr!='\021')&&(kr!='\006'));
      if ((kr=='\021')||(kr=='\006')) break;
   } /*while(1)*/
   
}


/*-----------------------------------------------------------------------------------
** Traduku vorton kaj cxiujn apudulojn, impresu cxion al monitoro.
**-----------------------------------------------------------------------------------*/
void tradukadoNoInterakte (TRADUKADO *t, char *vorto) {
   FAKTO *f;
   impresuTitolon(vorto);
   f=traduku(t,vorto);
   impresuFakton(f);
   neniiguFakton(f);
   impresuApudulojn(t,vorto);
   
}

/*-----------------------------------------------------------------------------------
** Impresu helpon - kiel uzadi cxi tiu aplikajxo.
**-----------------------------------------------------------------------------------*/
void impresuHelpon() {

printf("Aplikajxo 'vortaro' estis kreita de Jirka Hrbek en 2008 por esperantistoj sub licenco GNU GPL v.3. Vi povas uzadi gxin tiel:\n");
printf("\n");

printf("vortaro -k nomo.csv \n");
printf("   -(aux 'vortaro -ku nomo.csv) -konvertos vortojn en dosiero \"nomo.csv\" al cxi tiu aplikajxo. Konvertotaj vortoj estos en dosiero \"espvortaro.dat\". Dosiero \"nomo.csv\" vi povas ekhavi de \"http://esperanto.wz.cz/\", tra la konverto xls-dosiero->csv-dosiero. CSV-dosiero kaj sistemo devas uzadi UTF-8\n");
printf("\n");printf("\n");

printf("vortaro -ki nomo.csv \n");
printf("   -kiel elekto '-k', nur sistemo devas uzadi ISO-8859-2 (ISO Latin 2). Tiu elekto estas kutime por tiuj versioj de Linuks, kiuj ne uzadas UTF-8. Cxiuj esperantaj literoj estos konvertita al 'cx', 'gx' ...\n");
printf("\n");printf("\n");


printf("vortaro -kc nomo.csv \n");
printf("   -kiel elekto '-k', nur sistemo devas uzadi al CP852 (PC Latin 2). Tiu elekto estas kutime por Windows. Cxiuj esperantaj literoj estos konvertita al 'cx', 'gx' ...\n");
printf("\n");printf("\n");

printf("vortaro -kw nomo.csv \n");
printf("   -kiel elekto '-k', nur sistemo devas uzadi al Windows-1250. Bedauxrinde mi nescias, se tiu elekto estas utila por iu sistemo:). Cxiuj esperantaj literoj estos konvertita al 'cx', 'gx' ...\n");
printf("\n");printf("\n");

printf("vortaro -k nomo.csv nomo.dat\n");
printf("   -(eble '-ku', '-ki', '-kc' aux 'kw') konvertos vortojn en dosiero \"nomo.csv\" al cxi tiu aplikajxo. Konvertotaj vortoj estos en dosiero \"nomo.dat\". Dosiero \"nomo.csv\" vi povas ekhavi de \"http://esperanto.wz.cz/\", tra la konverto xls-dosiero->csv-dosiero.\n");
printf("\n");printf("\n");

printf("vortaro -t vorto [vorto vorto ...]\n");
printf("   -tradukos vorto/j helpe de \"espvortaro.dat\". Eligajxo iros al defauxlton.");
printf("\n");printf("\n");

printf("vortaro -td nomo.dat vorto [vorto vorto ...]\n");
printf("   -tradukos vorto/j helpe de \"nomo.dat\". Eligajxo iros al defauxlton.");
printf("\n");printf("\n");

printf("vortaro -ti\n");
printf("   -aplikajxo laboras interakte helpe de \"espvortaro.dat\" - demandos vorton, kaj tradukos gxin. Vi povas skani najbarajn vortojn kaj traduki sekvantajn vortojn aux fini laboron kun aplikajxo.");
printf("\n");printf("\n");


printf("vortaro -tid nomo.dat\n");
printf("   -aplikajxo laboras interakte helpe de \"nomo.dat\" - demandos vorton, kaj tradukos gxin. Vi povas skani najbarajn vortojn kaj traduki sekvantajn vortojn aux fini laboron kun aplikajxo.");
printf("\n");printf("\n");

printf("vortaro -h\n");
printf("   -cxi tiu helpo");

}
/*-----------------------------------------------------------------------------------
** Impresu nx letero 'kiu' - pro la impresuTitolon
** 
**-----------------------------------------------------------------------------------*/
void impresuKaraktrojn(char n, char kiu) {
   char i;
   for (i=0;i<n;i++) {
      printf("%c",kiu);
   }
}
/*-----------------------------------------------------------------------------------
** Impresu sercxita vorto frapante
**-----------------------------------------------------------------------------------*/
void impresuTitolon(char *titolo) {
   char l = 0;
   unsigned char *m=titolo;
   while (*m++) {
      l++;
   }
   impresuKaraktrojn(l+4,'*');
   printf("\n");
   printf("\n");
   printf("%s",titolo);
   printf("\n");
   printf("\n");
   impresuKaraktrojn(l+4,'*');
   printf("\n\n");
}

/*-----------------------------------------------------------------------------------
** Grandigu cxiun leteron en teksto 'param'  
** 
**-----------------------------------------------------------------------------------*/
char* ellaboruParametron(char *param) {
   char buf[VORTOLONGO];
   char *m;
   char i;
   char *res;
   if (*param != '-') return NULL;
   m=param;
   i=0;
   while (*m++) {
      if (*m<97) {
         buf[i++]=*m;
      } else {
         buf[i++]=*m - 32;
      }
   }
   res = miaMalloc("ellaboruParametron;res",i*sizeof(char));
   strcpy(res,buf);
   return res;
}

/*-----------------------------------------------------------------------------------
** Konstatu, se parametrojn 'unua' kaj 'dua' estas samaj
** 
**-----------------------------------------------------------------------------------*/
char komparuParametron(char *unua, char *dua) {
   char *m = unua;
   char i=0;
   while (*m) {
      if (strchr(dua,*m++)==NULL) {
         return 0;
      }
      i++;
   }
   if (strlen(dua)!=i) return 0;
   return 1;
}



/*-----------------------------------------------------------------------------------
** Terminu aplikajxo
** 
**-----------------------------------------------------------------------------------*/
#define exito miaFree("main;param",param);miaFree("main;defaultDATsituo",defaultDATsituo);finMemArangxo();exit(0);



/*-----------------------------------------------------------------------------------
** Laux parametroj donu cxiujn
** 
**-----------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{  


   char *param=NULL;
   char part;
   char partd;

   inicMemArangxo();
   trovuDATsituon(argv[0]); /*agordi defaultDATsituo*/
   if (argc>=2) {
      param = ellaboruParametron(argv[1]);
   } else {
      impresuHelpon();
      exito;
   }

   if (argc == 2) {
      if (komparuParametron(param,"TI")) {
         TRADUKADO *t = inicTradukado(NULL);
         tradukadoInterakte(t);
         neniiguTradukadon(&t);
         exito;
      }
      impresuHelpon();
      exito;
   }
   /* argc>2 */
   if (komparuParametron(param,"K") ||komparuParametron(param,"KU") ) {
      if (argc==3) {
         konvertu(argv[2],NULL,'u');
         exito;
      }
      if (argc==4) {
         konvertu(argv[2],argv[3],'u');
         exito;
      }
      impresuHelpon();
      exito;
   }

   if (komparuParametron(param,"KW") ) {
      if (argc==3) {
         konvertu(argv[2],NULL,'w');
         exito;
      }
      if (argc==4) {
         konvertu(argv[2],argv[3],'w');
         exito;
      }
      impresuHelpon();
      exito;
   }

   if (komparuParametron(param,"KI") ) {
      if (argc==3) {
         konvertu(argv[2],NULL,'i');
         exito;
      }
      if (argc==4) {
         konvertu(argv[2],argv[3],'i');
         exito;
      }
      impresuHelpon();
      exito;
   }

   if (komparuParametron(param,"KC") ) {
      if (argc==3) {
         konvertu(argv[2],NULL,'c');
         exito;
      }
      if (argc==4) {
         konvertu(argv[2],argv[3],'c');
         exito;
      }
      impresuHelpon();
      exito;
   }


   if (komparuParametron(param,"TID")) {
      if (argc==3) {
         TRADUKADO *t = inicTradukado(argv[2]); 
         tradukadoInterakte(t);   
         neniiguTradukadon(&t);   
         exito;
      } else {
         impresuHelpon();
      }
   }



/* no interakte tradukado */
   part = komparuParametron(param,"T");
   partd = komparuParametron(param,"TD");

   if (part || partd) {
      TRADUKADO *t;
      char i;
      if (part) {
         t = inicTradukado(NULL);
      } else {
         if (argc>3) {
            t = inicTradukado(argv[2]);
         } else {
            impresuHelpon();
            exito;
         }
      }
      for(i=(part)?2:3;i<argc;i++) {
         tradukadoNoInterakte(t, argv[i]);
      }
      neniiguTradukadon(&t);
      exito;
   } else {
      impresuHelpon();
      exito;
   }
   exito;
   return EXIT_SUCCESS;
}
