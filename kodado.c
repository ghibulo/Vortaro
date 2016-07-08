/*
** aplikajxo: VORTARO 1.0
** licenco:   GNU
** kompililu: kiu ajn ANSI-C kompatibila
**
** korpo:     kodado.c
** Cxi tio korpo enhavas ilojn por konverti kodadon UTF-8 al malsamaj kodadoj 
** de la cxehxa lingvo - Windows-1250, ISO-8859-2 (ISO Latin 2),
** CP852 (PC Latin 2)
**
** Ghibulo 2008, solvita tasko por FAV ZCU Plzen
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloko.h"
#include "kodado.h"



/*-----------------------------------------------------------------------------------
** Inicialo de strukturo KONVKODADO 
**         - okupu memoron kaj agordu konstantojn.
**         - konverto == 'u' ... konvertu nenion 
**         - konverto == 'w' ... konvertu UTF-8 -> Windows-1250
**         - konverto == 'i' ... konvertu UTF-8 -> ISO-8859-2 (ISO Latin 2)
**         - konverto == 'c' ... konvertu UTF-8 -> CP852 (PC Latin 2)
**-----------------------------------------------------------------------------------*/
KONVKODADO* inicKodado(char konverto) {
   KONVKODADO *resulto;
   if (konverto=='u') return NULL;

   resulto=(KONVKODADO*)miaMalloc("inicKodado;resulto",sizeof(KONVKODADO));
   resulto->de = (char*)miaMalloc("inicKodado;resulto->de",sizeof(char)*83);
   resulto->en = (char*)miaMalloc("inicKodado;resulto->en",sizeof(char)*83);
   /*UTF-8*/
   resulto->de[0]  = '\305';resulto->de[1]  = '\255'; /*esp u*/
   resulto->de[2]  = '\305';resulto->de[3]  = '\235'; /*esp s*/
   resulto->de[4]  = '\304';resulto->de[5]  = '\235'; /*esp g*/
   resulto->de[6]  = '\304';resulto->de[7]  = '\245'; /*esp h*/
   resulto->de[8]  = '\304';resulto->de[9]  = '\265'; /*esp j*/
   resulto->de[10] = '\304';resulto->de[11] = '\211'; /*esp c*/
   resulto->de[12] = '\305';resulto->de[13] = '\254'; /*esp U*/
   resulto->de[14] = '\305';resulto->de[15] = '\234'; /*esp S*/
   resulto->de[16] = '\304';resulto->de[17] = '\234'; /*esp G*/
   resulto->de[18] = '\304';resulto->de[19] = '\244'; /*esp H*/
   resulto->de[20] = '\304';resulto->de[21] = '\264'; /*esp J*/
   resulto->de[22] = '\304';resulto->de[23] = '\210'; /*esp C*/
   resulto->de[24] = '\304';resulto->de[25] = '\233'; /*cz ev*/
   resulto->de[26] = '\305';resulto->de[27] = '\241'; /*cz sv*/
   resulto->de[28] = '\304';resulto->de[29] = '\215'; /*cz cv*/
   resulto->de[30] = '\305';resulto->de[31] = '\231'; /*cz rv*/
   resulto->de[32] = '\305';resulto->de[33] = '\276'; /*cz zv*/
   resulto->de[34] = '\303';resulto->de[35] = '\275'; /*cz yi*/
   resulto->de[36] = '\303';resulto->de[37] = '\241'; /*cz ai*/
   resulto->de[38] = '\303';resulto->de[39] = '\255'; /*cz ii*/
   resulto->de[40] = '\303';resulto->de[41] = '\251'; /*cz ei*/
   resulto->de[42] = '\305';resulto->de[43] = '\245'; /*cz tv*/
   resulto->de[44] = '\303';resulto->de[45] = '\272'; /*cz ui*/
   resulto->de[46] = '\305';resulto->de[47] = '\257'; /*cz uo*/
   resulto->de[48] = '\303';resulto->de[49] = '\263'; /*cz oi*/
   resulto->de[50] = '\304';resulto->de[51] = '\217'; /*cz dv*/
   resulto->de[52] = '\305';resulto->de[53] = '\210'; /*cz nv*/
   resulto->de[54] = '\304';resulto->de[55] = '\232'; /*cz Ev*/
   resulto->de[56] = '\305';resulto->de[57] = '\240'; /*cz Sv*/
   resulto->de[58] = '\304';resulto->de[59] = '\214'; /*cz Cv*/
   resulto->de[60] = '\305';resulto->de[61] = '\230'; /*cz Rv*/
   resulto->de[62] = '\305';resulto->de[63] = '\275'; /*cz Zv*/
   resulto->de[64] = '\303';resulto->de[65] = '\235'; /*cz Yi*/
   resulto->de[66] = '\303';resulto->de[67] = '\201'; /*cz Ai*/
   resulto->de[68] = '\303';resulto->de[69] = '\215'; /*cz Ii*/
   resulto->de[70] = '\303';resulto->de[71] = '\211'; /*cz Ei*/
   resulto->de[72] = '\305';resulto->de[73] = '\244'; /*cz Tv*/
   resulto->de[74] = '\303';resulto->de[75] = '\232'; /*cz Ui*/
   resulto->de[76] = '\303';resulto->de[77] = '\223'; /*cz Oi*/
   resulto->de[78] = '\304';resulto->de[79] = '\216'; /*cz Dv*/
   resulto->de[80] = '\305';resulto->de[81] = '\207'; /*cz Nv*/
   resulto->de[82] = '\000'; 

   if (konverto=='w') { /*al Windows-1250 */
      resulto->en[0]  = 'u';   resulto->en[1]  = 'x'; /*ux*/
      resulto->en[2]  = 's';   resulto->en[3]  = 'x'; /*sx*/
      resulto->en[4]  = 'g';   resulto->en[5]  = 'x'; /*gx*/
      resulto->en[6]  = 'h';   resulto->en[7]  = 'x'; /*hx*/
      resulto->en[8]  = 'j';   resulto->en[9]  = 'x'; /*jx*/
      resulto->en[10] = 'c';   resulto->en[11] = 'x'; /*cx*/
      resulto->en[12] = 'U';   resulto->en[13] = 'x'; /*UX*/
      resulto->en[14] = 'S';   resulto->en[15] = 'x'; /*SX*/
      resulto->en[16] = 'G';   resulto->en[17] = 'x'; /*GX*/
      resulto->en[18] = 'H';   resulto->en[19] = 'x'; /*HX*/
      resulto->en[20] = 'J';   resulto->en[21] = 'x'; /*JX*/
      resulto->en[22] = 'C';   resulto->en[23] = 'x'; /*CX*/
      resulto->en[24] = '\354';resulto->en[25] = '\000'; /*cz ev*/
      resulto->en[26] = '\232';resulto->en[27] = '\000'; /*cz sv*/
      resulto->en[28] = '\350';resulto->en[29] = '\000'; /*cz cv*/
      resulto->en[30] = '\370';resulto->en[31] = '\000'; /*cz rv*/
      resulto->en[32] = '\236';resulto->en[33] = '\000'; /*cz zv*/
      resulto->en[34] = '\375';resulto->en[35] = '\000'; /*cz yi*/
      resulto->en[36] = '\341';resulto->en[37] = '\000'; /*cz ai*/
      resulto->en[38] = '\355';resulto->en[39] = '\000'; /*cz ii*/
      resulto->en[40] = '\351';resulto->en[41] = '\000'; /*cz ei*/
      resulto->en[42] = '\235';resulto->en[43] = '\000'; /*cz tv*/
      resulto->en[44] = '\372';resulto->en[45] = '\000'; /*cz ui*/
      resulto->en[46] = '\371';resulto->en[47] = '\000'; /*cz uo*/
      resulto->en[48] = '\363';resulto->en[49] = '\000'; /*cz oi*/
      resulto->en[50] = '\357';resulto->en[51] = '\000'; /*cz dv*/
      resulto->en[52] = '\362';resulto->en[53] = '\000'; /*cz nv*/
      resulto->en[54] = '\314';resulto->en[55] = '\000'; /*cz Ev*/
      resulto->en[56] = '\212';resulto->en[57] = '\000'; /*cz Sv*/
      resulto->en[58] = '\310';resulto->en[59] = '\000'; /*cz Cv*/
      resulto->en[60] = '\330';resulto->en[61] = '\000'; /*cz Rv*/
      resulto->en[62] = '\216';resulto->en[63] = '\000'; /*cz Zv*/
      resulto->en[64] = '\335';resulto->en[65] = '\000'; /*cz Yi*/
      resulto->en[66] = '\301';resulto->en[67] = '\000'; /*cz Ai*/
      resulto->en[68] = '\315';resulto->en[69] = '\000'; /*cz Ii*/
      resulto->en[70] = '\311';resulto->en[71] = '\000'; /*cz Ei*/
      resulto->en[72] = '\215';resulto->en[73] = '\000'; /*cz Tv*/
      resulto->en[74] = '\332';resulto->en[75] = '\000'; /*cz Ui*/
      resulto->en[76] = '\323';resulto->en[77] = '\000'; /*cz Oi*/
      resulto->en[78] = '\317';resulto->en[79] = '\000'; /*cz Dv*/
      resulto->en[80] = '\322';resulto->en[81] = '\000'; /*cz Nv*/
      resulto->en[82] = '\000'; 
      return resulto;
   }/*if (konverto=='w') al Windows-1250*/

   if (konverto=='i') { /*al ISO-8859-2 (ISO Latin 2)*/
      resulto->en[0]  = 'u';   resulto->en[1]  = 'x'; /*ux*/
      resulto->en[2]  = 's';   resulto->en[3]  = 'x'; /*sx*/
      resulto->en[4]  = 'g';   resulto->en[5]  = 'x'; /*gx*/
      resulto->en[6]  = 'h';   resulto->en[7]  = 'x'; /*hx*/
      resulto->en[8]  = 'j';   resulto->en[9]  = 'x'; /*jx*/
      resulto->en[10] = 'c';   resulto->en[11] = 'x'; /*cx*/
      resulto->en[12] = 'U';   resulto->en[13] = 'x'; /*UX*/
      resulto->en[14] = 'S';   resulto->en[15] = 'x'; /*SX*/
      resulto->en[16] = 'G';   resulto->en[17] = 'x'; /*GX*/
      resulto->en[18] = 'H';   resulto->en[19] = 'x'; /*HX*/
      resulto->en[20] = 'J';   resulto->en[21] = 'x'; /*JX*/
      resulto->en[22] = 'C';   resulto->en[23] = 'x'; /*CX*/
      resulto->en[24] = '\354';resulto->en[25] = '\000'; /*cz ev*/
      resulto->en[26] = '\271';resulto->en[27] = '\000'; /*cz sv*/
      resulto->en[28] = '\350';resulto->en[29] = '\000'; /*cz cv*/
      resulto->en[30] = '\370';resulto->en[31] = '\000'; /*cz rv*/
      resulto->en[32] = '\276';resulto->en[33] = '\000'; /*cz zv*/
      resulto->en[34] = '\375';resulto->en[35] = '\000'; /*cz yi*/
      resulto->en[36] = '\341';resulto->en[37] = '\000'; /*cz ai*/
      resulto->en[38] = '\355';resulto->en[39] = '\000'; /*cz ii*/
      resulto->en[40] = '\351';resulto->en[41] = '\000'; /*cz ei*/
      resulto->en[42] = '\273';resulto->en[43] = '\000'; /*cz tv*/
      resulto->en[44] = '\372';resulto->en[45] = '\000'; /*cz ui*/
      resulto->en[46] = '\371';resulto->en[47] = '\000'; /*cz uo*/
      resulto->en[48] = '\363';resulto->en[49] = '\000'; /*cz oi*/
      resulto->en[50] = '\357';resulto->en[51] = '\000'; /*cz dv*/
      resulto->en[52] = '\362';resulto->en[53] = '\000'; /*cz nv*/
      resulto->en[54] = '\314';resulto->en[55] = '\000'; /*cz Ev*/
      resulto->en[56] = '\251';resulto->en[57] = '\000'; /*cz Sv*/
      resulto->en[58] = '\310';resulto->en[59] = '\000'; /*cz Cv*/
      resulto->en[60] = '\330';resulto->en[61] = '\000'; /*cz Rv*/
      resulto->en[62] = '\256';resulto->en[63] = '\000'; /*cz Zv*/
      resulto->en[64] = '\335';resulto->en[65] = '\000'; /*cz Yi*/
      resulto->en[66] = '\301';resulto->en[67] = '\000'; /*cz Ai*/
      resulto->en[68] = '\315';resulto->en[69] = '\000'; /*cz Ii*/
      resulto->en[70] = '\311';resulto->en[71] = '\000'; /*cz Ei*/
      resulto->en[72] = '\253';resulto->en[73] = '\000'; /*cz Tv*/
      resulto->en[74] = '\332';resulto->en[75] = '\000'; /*cz Ui*/
      resulto->en[76] = '\323';resulto->en[77] = '\000'; /*cz Oi*/
      resulto->en[78] = '\317';resulto->en[79] = '\000'; /*cz Dv*/
      resulto->en[80] = '\322';resulto->en[81] = '\000'; /*cz Nv*/
      resulto->en[82] = '\000'; 
      return resulto;
   }/*if (konverto=='i') al ISO-8859-2 (ISO Latin 2)*/

   if (konverto=='c') { /*al CP852 (PC Latin 2)*/
      resulto->en[0]  = 'u';   resulto->en[1]  = 'x'; /*ux*/
      resulto->en[2]  = 's';   resulto->en[3]  = 'x'; /*sx*/
      resulto->en[4]  = 'g';   resulto->en[5]  = 'x'; /*gx*/
      resulto->en[6]  = 'h';   resulto->en[7]  = 'x'; /*hx*/
      resulto->en[8]  = 'j';   resulto->en[9]  = 'x'; /*jx*/
      resulto->en[10] = 'c';   resulto->en[11] = 'x'; /*cx*/
      resulto->en[12] = 'U';   resulto->en[13] = 'x'; /*UX*/
      resulto->en[14] = 'S';   resulto->en[15] = 'x'; /*SX*/
      resulto->en[16] = 'G';   resulto->en[17] = 'x'; /*GX*/
      resulto->en[18] = 'H';   resulto->en[19] = 'x'; /*HX*/
      resulto->en[20] = 'J';   resulto->en[21] = 'x'; /*JX*/
      resulto->en[22] = 'C';   resulto->en[23] = 'x'; /*CX*/
      resulto->en[24] = '\330';resulto->en[25] = '\000'; /*cz ev*/
      resulto->en[26] = '\347';resulto->en[27] = '\000'; /*cz sv*/
      resulto->en[28] = '\237';resulto->en[29] = '\000'; /*cz cv*/
      resulto->en[30] = '\375';resulto->en[31] = '\000'; /*cz rv*/
      resulto->en[32] = '\247';resulto->en[33] = '\000'; /*cz zv*/
      resulto->en[34] = '\354';resulto->en[35] = '\000'; /*cz yi*/
      resulto->en[36] = '\240';resulto->en[37] = '\000'; /*cz ai*/
      resulto->en[38] = '\241';resulto->en[39] = '\000'; /*cz ii*/
      resulto->en[40] = '\202';resulto->en[41] = '\000'; /*cz ei*/
      resulto->en[42] = '\234';resulto->en[43] = '\000'; /*cz tv*/
      resulto->en[44] = '\243';resulto->en[45] = '\000'; /*cz ui*/
      resulto->en[46] = '\205';resulto->en[47] = '\000'; /*cz uo*/
      resulto->en[48] = '\242';resulto->en[49] = '\000'; /*cz oi*/
      resulto->en[50] = '\324';resulto->en[51] = '\000'; /*cz dv*/
      resulto->en[52] = '\345';resulto->en[53] = '\000'; /*cz nv*/
      resulto->en[54] = '\267';resulto->en[55] = '\000'; /*cz Ev*/
      resulto->en[56] = '\346';resulto->en[57] = '\000'; /*cz Sv*/
      resulto->en[58] = '\254';resulto->en[59] = '\000'; /*cz Cv*/
      resulto->en[60] = '\374';resulto->en[61] = '\000'; /*cz Rv*/
      resulto->en[62] = '\246';resulto->en[63] = '\000'; /*cz Zv*/
      resulto->en[64] = '\355';resulto->en[65] = '\000'; /*cz Yi*/
      resulto->en[66] = '\265';resulto->en[67] = '\000'; /*cz Ai*/
      resulto->en[68] = '\326';resulto->en[69] = '\000'; /*cz Ii*/
      resulto->en[70] = '\220';resulto->en[71] = '\000'; /*cz Ei*/
      resulto->en[72] = '\233';resulto->en[73] = '\000'; /*cz Tv*/
      resulto->en[74] = '\351';resulto->en[75] = '\000'; /*cz Ui*/
      resulto->en[76] = '\340';resulto->en[77] = '\000'; /*cz Oi*/
      resulto->en[78] = '\322';resulto->en[79] = '\000'; /*cz Dv*/
      resulto->en[80] = '\325';resulto->en[81] = '\000'; /*cz Nv*/
      resulto->en[82] = '\000'; 
      return resulto;
   }/*if (konverto=='c') al CP852 (PC Latin 2)*/


   /*malbona parametro 'konverto'*/
   miaFree("inicKodado;resulto->de",resulto->de);
   miaFree("inicKodado;resulto->en",resulto->en);
   miaFree("inicKodado;resulto",resulto);
   return NULL;
}

/*-----------------------------------------------------------------------------------
** Donu konverton - teksto 'kio' estos konvertita laux 'kiel', poste teksto 'kio' 
** estos ruinigita, kaj konvertita teksto estos eligo
**-----------------------------------------------------------------------------------*/
char* konvertuKodado(KONVKODADO *kiel, char *kio) {
   char *resulto=(char*)miaMalloc("konvertuKodado;resulto",sizeof(char)*LINLONGO);
   char *mntr=resulto;
   char *mntk = kio;
   char buf[3];
   char *tmp;
   buf[2]='\0';
   while (*mntk) {
      if ((*mntk=='\303')||(*mntk=='\304')||(*mntk=='\305')) {
         buf[0]=*mntk;
         buf[1]=*(mntk+1);
         if ((tmp = strstr(kiel->de,buf))!=NULL) {
            mntk+=2;
            tmp = kiel->en + (tmp-kiel->de);
            *mntr++ = *tmp++;
            if (*tmp) { /*se dua hxaraktero ne estas '\0'*/
               *mntr++ = *tmp;
            }
         } else {
            *mntr++ = *mntk++;
         }
      } else {
         *mntr++ = *mntk++;
      }
   }
   *mntr='\0';
   miaFree("konvertuKodado;kio",kio);
   return resulto;
}

/*-----------------------------------------------------------------------------------
** Malbloki memoron, strukturo KONVKODADO estos ruinigita
**-----------------------------------------------------------------------------------*/
void neniiguKodado (KONVKODADO **kiu) {
   if (*kiu==NULL) return;
   miaFree("neniiguKodado;*kiu->de",(*kiu)->de);
   miaFree("neniiguKodado;*kiu->en",(*kiu)->en);
   miaFree("neniiguKodado;*kiu",*kiu);
   *kiu=NULL;

}

