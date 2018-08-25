#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <excpt.h>
#include <locale.h>
#include <wchar.h>
#include "aff.h"
#define rndmx 2147483647

////////////////////////////////////////////////////////////////////////////////////////////////////
////                                  PARTIE FICHIER                                           /////
////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------- fonction qui compte le nombre de ligne d'un fichier ---------/////

int nb_ln(FILE *f)
{ int cpt =0;
  char ligne[100];

  while(!eof(f))
  { fgets(ligne,100,f);

    cpt++;

  }
  return cpt-1 ;

}

//------------------------------- fonction qui compte le nombre de char d'un fichier ---------/////

int nb_ch(FILE *f)
{ int cpt=0;
  char c;

  while(!eof(f))
    { c=fgetc(f);


      if(c!=' ' && c!=10){ cpt++;}
    }
    return cpt ;

}

//------------------------------- fonction qui compte le nombre de mots d'un fichier ---------/////

int nb_m(FILE *f)
{int cpt=0;
 char ligne[100],*l;
 char dil[]="_ -''\"//\\,;.:!?()*{}[]<>\n\t$";

 while(!eof(f))
    { fgets(ligne,100,f);
      l=strtok(ligne,dil);
      while(l!=NULL)
      {
          cpt++;
        l=strtok(NULL,dil);


      }

    }

return cpt;
}

////----------------------------rechere si un caractere est present dans une chaine ------------------------------////

int s_c(char str[],char c)
{ int i;
  for(i=0;i<strlen(str);i++)
    {if(str[i]==c)return 1 ;
    }
  return 0;
}


int last_word(char ligne[100])
{ char dil[]="_ -''""//\\,;.:!?()*{}[]<>\n\t$";
  int i=0;
  int z=0;
  for(i=0;i<100;i++)
  { if(s_c(dil,ligne[i]))
      { z=i;

      }


  }
  if(s_c(ligne,10))return 100;
return z;
}






//-----------------------         le module display --------------------------------------//


void Display(char name[20])
{ FILE *f;
  char ligne[100];
  int nb_ll,nb_mm,nb_chh,cpt=0;
  int x,y,i;
  char *old_locale, *saved_locale;
 f=fopen(name,"r+");

 ///////////////////////////////////////
 ///      pour l'affichage          ///

 old_locale = setlocale (LC_ALL, NULL);
 saved_locale = strdup (old_locale);

////////////////////////////////////////

 if(f==NULL)puts("Fichier introuvable !");
 else

 {
 nb_ll=nb_ln(f);rewind(f);
 nb_mm=nb_m(f);rewind(f);
 nb_chh=nb_ch(f);rewind(f);
 puts("\n");

     while(!eof(f))
    { if(cpt==0){
        system("cls");
       textbackground(2);
        textcolor(15);
        printf("\n\n                                                  PAGE 1                                                  \n\n\n");
        textcolor(15);
        textbackground(0);}
      //// recuperation de la ligne
      fgets(ligne,100,f);
      cpt++;

      int a=last_word(ligne);



      textcolor(15);
      textbackground(0);

      setlocale(LC_CTYPE,"");

      if(!s_c(ligne,10) && !s_c(ligne,'\n')){


      printf("%s\n",ligne);}
      else{
            printf("%s",ligne);
          }

      //// verifie si on est pas encore passer la page suivante
      if(cpt%25 ==0){
                           printf("\n");
                           textbackground(4);
                           textcolor(15);
                           gotoxy(16,wherey()+3);
                           puts("Clicker pour passer a la page suivante ");
                           gotoxy(15,wherey());
                           system("pause");
                           textbackground(0);
                           system("cls");
                           textbackground(2);
                           textcolor(15);
                           puts("\n");
                           printf("                                                  PAGE %d                                                  \n\n\n",(cpt/25)+1);
                           textcolor(15);
                           textbackground(0);

                           }


     ///////---------------- Pour l'affichage ----------------------------------------
     if(cpt%25==1){
     x=wherex();
     y=wherey();
     bande();

     textcolor(15);
     textbackground(2);
     gotoxy(106,12);printf("NB Ligne\n");
     gotoxy(109,13);printf("%d\n",nb_ll);
     gotoxy(106,15);printf("NB Mots");
     gotoxy(109,16);printf("%d\n",nb_mm);
     gotoxy(106,18);printf("NB Char");
     gotoxy(109,19);printf("%d\n",nb_chh);
     gotoxy(106,21);printf("NB Pages");
     gotoxy(109,22);printf("%d\n",nb_ll/25 +1);

     gotoxy(x,y);}



    }

 }








 fclose(f);
  setlocale (LC_ALL, saved_locale);
  free (saved_locale);


}

//// --------------------- fonction qui recherche l'occurence et indique la position du mot et la ligne et la page  ///


int cherche_occ(char ligne[100],char mot[20],int ln, int pos)
{ int ind=0,trv=0,i,cpt=0;
  char dil[]="_ -''\"//\\,;.:!?()*{}[]<>\n\t$";

     while(ind!=100)
        {
            trv=1;


            for(i=0;i<strlen(mot);i++)
          { if(ligne[i+ind]!=mot[i]) trv=0;

          }

          if(trv)
            {
                  if (ind==0)
                    { if(!s_c(dil,ligne[ind+strlen(mot)])) trv=0;
                    }
                  else{
                         if(!s_c(dil,ligne[ind-1]))
                         {trv=0;}
                         if((ind+strlen(mot))!=100)
                         { if(!s_c(dil,ligne[ind+strlen(mot)]))trv=0;
                         }

                      }
             }

          if(trv)
            {
              cpt++;
             textbackground(0);
             textcolor(pos%14 +1);
             gotoxy(2,wherey()+1);
             printf(">>");
             textcolor(15);
             printf("le mot se trouve a la position %d de la ligne %d de la page %d\n",ind+1,(ln%25),(ln/25)+1 );
            }
            if(trv){
            ind+=strlen(mot);}
            else{ind++;}
          }
    return  cpt;
}

//// ---------------------------------- Le module search_replace ------------------------------------------------

void Search_Replace(char f_name[20],char mot[25],char rpm[25])
{ FILE *f,*p;

  char ligne[150];
  char db_ligne[150];
  char dil[]="_ -''\"//\\,;.:!?()*{}[]<>\n\t$";

  int nb_oc=0,ind=0,ln=0,trv=1,cpt=0,trv2=0,i;

  rename(f_name,"cop.txt");
  f=fopen(f_name,"w+");
  p=fopen("cop.txt","r+");
  if(p==NULL || f==NULL)puts("Fichier introuvable");


  while(!eof(p))
    { ////recuperation de la ligne

      fgets(ligne,100,p);
      ln++;
      ind=0;

      trv2=0;
      nb_oc+=cherche_occ(ligne,mot,ln,ln);

      while(ind!=150)
        {

            trv=1;

           //// recherche du mot
            for(i=0;i<strlen(mot);i++)
          { if(ligne[i+ind]!=mot[i]) trv=0;

          }
          //// virifie que le mpt n'est pas comptrie dans un autre
          if(trv)
            {
                  if (ind==0)
                    { if(!s_c(dil,ligne[ind+strlen(mot)])) trv=0;
                    }
                  else{
                         if(!s_c(dil,ligne[ind-1]))
                         {trv=0;}
                         if((ind+strlen(mot))!=100)
                         { if(!s_c(dil,ligne[ind+strlen(mot)]))trv=0;
                         }

                     }
             }



          if(trv)
            { trv2=1;
              //// ecrie la chaine qui est  avant le mot dans db_ligne
              for(i=0;i<ind;i++)
                { db_ligne[i]=ligne[i];
                }
              //// ecrie le mot dans db_ligne apartir de la position aproprier
              for(i=ind;i<ind+strlen(rpm);i++){
                db_ligne[i]=rpm[i-ind];
              }
              //// ecrie la chaine qui vient apres le mot dans db_ligne
              for(i=ind+strlen(rpm);i<150;i++){ db_ligne[i]=ligne[i-(strlen(rpm)-strlen(mot))];}

            }
          if(trv){
          ind+=strlen(rpm);
          for(i=0;i<150;i++)
            { ligne[i]='\0';
            }
           // remplace la ligne par db_ligne ( le processeur se termine quand on remplace toute les occurances)
           strcpy(ligne,db_ligne);
                 }
          else{ind++;}





        }


       if(trv2){
       fprintf(f,"%s",db_ligne);
               }
       else{fprintf(f,"%s",ligne);
           }




    }
    ////----------------- pour l'affichage ----------------------------
    textbackground(2);
    textcolor(15);
    gotoxy(110,14);
    printf("NB OC ");
    gotoxy(111,16);
    printf("%d",nb_oc);
    fclose(f);
    fclose(p);
    remove("cop.txt");



}









////////////////////////////////////////////////////////////////////////////////////////////////////
////                                  PARTIE RSA                                               /////
////////////////////////////////////////////////////////////////////////////////////////////////////



// ----------------------------- Fonction puissance ---------------------------------

long puis(long n,long p )
{ long i,z=1 ;
  for(i=0;i<p;i++)
  { z*=n;
  }
  return z;
}

// --------------------------- Fonction qui verifie si un nombre est premier ---------

int prime(long a)
{ long i;
  for(i=2;i<=(a/2);i++)
  { if(a%i==0){ return 0;}

  }
  return 1;
}
//---------------------------- Fonction qui calcule le PGCD de deux nombres ------------------
long pgcd(long a,long b)
{ long i,h;
  for(i=1;i<=(a>b?b:a);i++)
    { if((a%i==0) && (b%i==0)) h=i; }
    return h;
}
// -------
int eof(FILE *f) {
    char a = getc(f);fseek(f,-1, SEEK_CUR);
    if(a==EOF){return 1;}
    return 0;
}
// ------------------------ Fonction qui calcule le modulo inverse ----------------------------

long mod_inv(long a, long diviseur)
{
	long b0 = diviseur, t, q;
	long x0 = 0, x1 = 1;
	if (diviseur == 1) return 1;
	while (a > 1) {
		q = a / diviseur;
		t = diviseur, diviseur = a % diviseur, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}

//------------------------- Module de creation ds cles d e n -------------------------------

void cree_cle(long *n, long *e, long *d)
{  long p=0,q=0,w;
   long z,h;
   long i ;
   int trv ;

 // creation de deux nombres premiers distinct
        srand(time(NULL));
        while(((!prime(p))||!(prime(q)))||(p==q))
             {
                 p= rand()%500 ;
                 q= rand()%500 ;
             }


   *n=(q*p);// calculer n
   w=(q-1)*(p-1);// claculer w(n)

   //  claculer e   max(p,q)<e<w(n) et pgcd(e,w(n))=1

  *e=(p>q?p:q)+1;
    while((pgcd(*e,w)!=1 || *e>w ) || (*e<(p>q?p:q)))
          {   *e=rand()%(w+1);  }

   // calculer d      max(p,q)<d<w(n) et d*e est congrus a 1 modulo w(n)

    *d=mod_inv(*e,w);

}

// --!----------------------Fonction de cryptage ------------------------------------------------

long crypte( char m,long e, long n)
{ long i;
  long long sortie=1 ;
  long z=m;



for(i=0;i<e;i++)
    { sortie*=z;
      sortie = fmod(sortie,n);
    }

return sortie;
}

// ---------------------------- Fonction de decryptage ------------------------------------------

char decrypte(long x,long d,long n)
{long i;

long long sortie=1 ;

for(i=0;i<d;i++)
    { sortie*=x;
      sortie = fmod(sortie,n);
    }


return sortie; }

//  ---------------------- Module pour crypter un fichier -------------------------------


void crypte_RSA(char src[20],char dest[20],long e,long n)
{FILE *f,*q;
int i=0;
char c;
long z;
int nb_c,cpt=0;
//-------- pour l'affichage -------------------------------------------------------
chaine M[]={"001010101110001010100000100001100000000001101011110101010101001100101",
            "011111111111001010101100100001111101010101101001010101010101001100101",
            "101010101110101010101100101011101001101000100111101001101001001001110",
            "001010101110001010100000100001111111111111101001010101010101001100101",
            "001010101110001010100000100001100001010101101001010101010101001100101",
             };
//----------------------------------------------------------------------------------

//------ Ouverture du fichier source et du fichier destination ----------
f=fopen(src,"r+");
q=fopen(dest,"wb+");


            //--------------------------- Special affichage -------------------------------------------------------
            srand(time(NULL));
            textcolor(2);
            textbackground(0);
            printdirection("101010101110101010100000101011101101110010010101110110011010011011001","right",20,i+20,0);
            printdirection("001010001110101000100000101011101011100111010010011110010110010101010","right",20,i+21,0);
            printdirection("101111111110101110100000101111101101010101010010010101010101001001001","right",20,i+22,0);
            printdirection("101010101110101010101100101011101001101000100111101001101001001001110","right",20,i+23,0);
            printdirection("001010101110001010100000100001100001010101101001010101010101001100101","right",20,i+24,0);
            printdirection("001010101110001010100000100001100001010101101001010101010101001100101","right",20,i+25,0);
            printdirection("001010101110001010100000100001111111111111101001010101010101001100101","right",20,i+26,0);
             textbackground(2);
             textcolor(15);
             gotoxy(20,30);

             printf("                     PROGRESSION :     %c                            ",37);
            //-------------------------------------------------------------------------------------------------------

nb_c=nb_ch(f);
rewind(f);



if(f!=NULL)
 { do
   { ////---------------- tant que le fichier n'est totalement traiter  on chiffre le char et on ecire le resulat dans la destination
     c=fgetc(f);
     if(c!=EOF){
     z=crypte(c,e,n);

     fwrite(&z,sizeof(long),4,q);
     if(c!=' ' && c!=10){
     cpt++;}
     if(cpt>nb_c){puts("sale con !");}

              //------------- Special affichage ------------------
               textcolor(2);
               textbackground(0);
               printdirection(M[rand()%5].mots,"right",20,rand()%(26-20)+20,0);
              //--------------------------------------------------
     }

     textbackground(2);
     textcolor(15);
     gotoxy(55,30);
     printf("%d",(cpt*100) / nb_c);


    }while(c!=EOF);
            textcolor(15);
            textbackground(2);
            printdirection("                           FIN DU CRYPTAGE                           ","right",20,i+18,0);


}
else  { gotoxy(31,35); puts("Le Fichier introduie n'existe  pas !"); }
fclose(f);
fclose(q);
}

// ----------------------------- module decryptage d'un fichier ------------------------------

void decrypte_RSA(char src[20],char dest[20],long d,long n,int nb_c)
{FILE *f,*q;
int i=0;
// ------------- pour l'affichage -----------------------------------------------//
chaine l[]={"001010101110001010100000100001100000000001101011110101010101001100101",
            "011111111111001010101100100001111101010101101001010101010101001100101",
            "101010101110101010101100101011101001101000100111101001101001001001110",
            "001010101110001010100000100001111111111111101001010101010101001100101",
            "001010101110001010100000100001100001010101101001010101010101001100101",
             };
//----------------------------------------------------------------------------------
char c;
long z;
int cpt=0;
//   ---- Ouverture du fichier source et du fichier destination -------------
f=fopen(src,"rb");
q=fopen(dest,"w+");


            //--------------------------- Special affichage -------------------------------------------------------
            srand(time(NULL));
            textcolor(2);
            textbackground(0);
            printdirection("101010101110101010100000101011101101110010010101110110011010011011001","right",20,i+20,0);
            printdirection("001010001110101000100000101011101011100111010010011110010110010101010","right",20,i+21,0);
            printdirection("101111111110101110100000101111101101010101010010010101010101001001001","right",20,i+22,0);
            printdirection("101010101110101010101100101011101001101000100111101001101001001001110","right",20,i+23,0);
            printdirection("001010101110001010100000100001100001010101101001010101010101001100101","right",20,i+24,0);
            printdirection("001010101110001010100000100001100001010101101001010101010101001100101","right",20,i+25,0);
            printdirection("001010101110001010100000100001111111111111101001010101010101001100101","right",20,i+26,0);
             textbackground(2);
             textcolor(15);
             gotoxy(20,30);

             printf("                     PROGRESSION :     %c                            ",37);
            //-------------------------------------------------------------------------------------------------------



if(f!=NULL)
 {
  while(!eof(f))
    { ////---------------- tant que le fichier n'est totalement traiter  on dechiffre le nombre et on ecire le resulat dans la destination
    fread(&z,sizeof(long),4,f);

     c=decrypte(z,d,n);

     cpt++;

     fprintf(q,"%c",c);

              //------------- Special affichage ------------------
              textcolor(2);
              textbackground(0);
               printdirection(l[rand()%5].mots,"right",20,rand()%(26-20)+20,0);
              //--------------------------------------------------
     textbackground(2);
     textcolor(15);


     gotoxy(55,30);
     printf("%d",(cpt*100)/nb_c);

    }

            textcolor(15);
            textbackground(2);
            printdirection("                         FIN DU DECRYPTAGE                           ","right",20,i+18,0);

}
else{   gotoxy(31,35);
        puts("Le Fichier introduie n'existe  pas !");}
fclose(f);
fclose(q);
}
void menu(void)
{ int *e,*d,*n;
  int x,y,nb_c;
  FILE *f;
  cree_cle(&n,&e,&d);
  char name[20],mot[20],rmp[20];
  char dest[20];
  char psw[20]; ////////--- pour l'acces aux cles ---////////////
  char *old_locale, *saved_locale;
  deb:
  basemenu();
  switch(choix())
  { case 1: system("cls");
           textcolor(2);
           bande();
           printdirection("Introduire le nom du Fichier:","right",5,2,20);
           gotoxy(35,2);textcolor(15);scanf("%s",&name);
           Display(name);
           gotoxy(10,wherey()+2);
           system("pause");
           break;
    case 2: system("cls");
            x=wherex();
            y=wherey();

            bande();
            if(wherey()==40){bande();}


            textcolor(4);

            printdirection("INTRODUIRE LE NOM DU FICHIER SOURCE:","right",4,11,20);
            gotoxy(45,11);
            textcolor(15);
            scanf("%s",&name);
            textcolor(4);


            printdirection(">> INTRODUIRE LE MOT A REMPLACER :","right",4,14,20);
            gotoxy(45,14);
            textcolor(15);

            old_locale = setlocale (LC_ALL, NULL);
            saved_locale = strdup (old_locale);

            scanf("%s",&mot);
            textcolor(4);

            printdirection(">> INTRODUIRE LE MOT SUBSTITUTIVE :","right",4,16,20);
            gotoxy(45,16);
            textcolor(15);
            scanf("%s",&rmp);
            system("cls");bande();
            gotoxy(x,y);

            setlocale(LC_CTYPE,"");
            Search_Replace(name,mot,rmp);
            setlocale (LC_ALL, saved_locale);
            textbackground(2);
            textcolor(15);
            gotoxy(20,35);
            printdirection("FIN AVEC SUCCES","right",20,wherey()+1,5);
            gotoxy(20,wherey()+1);
            textbackground(0);
            system("pause");
            break;


    case 3:
            system("cls");
            bande();
            textcolor(15);
            textbackground(2);
            printdirection("                                           EN MODE CRYPTAGE                                              ","right",0,3,4);
            textcolor(4);
            textbackground(0);
            printdirection("INTRODUIRE LE NOM DU FICHIER SOURCE:","right",4,11,20);
            gotoxy(45,11);
            textcolor(15);
            scanf("%s",&name);
            textcolor(4);
            printdirection("INTRODUIRE LE NOM DU FICHIER DESTINATION:","right",4,13,20);
            gotoxy(48,13);
            textcolor(15);
            scanf("%s",&dest);
            crypte_RSA(name,dest,e,n);
            gotoxy(30,35);
            system("pause");
            break;
    case 4:
            system("cls");
            bande();
            textcolor(15);
            textbackground(2);
            printdirection("                                           EN MODE DECRYPTAGE                                            ","right",0,3,3);

            textcolor(4);

            printdirection("INTRODUIRE LE NOM DU FICHIER SOURCE:","right",4,11,20);
            gotoxy(45,11);
            textcolor(15);
            scanf("%s",&name);
            textcolor(4);
            printdirection("INTRODUIRE LE NOM DU FICHIER DESTINATION:","right",4,13,20);
            gotoxy(48,13);
            textcolor(15);
            scanf("%s",&dest);
            ////-------- pour l'affichage ----------
            f=fopen(name,"rb");
            nb_c=Prog(f);
            fclose(f);
            ////------------------------------------
            decrypte_RSA(name,dest,d,n,nb_c);
            gotoxy(30,35);
            system("pause");
            break;
    case 5:
           system("cls");
           bande();
           textcolor(15);
           textbackground(2);
           printdirection("POUR CONSULTER LES CLEES INTRODUIRE UN MOT DE PASSE","right",4,13,20);textbackground(2);
           printdirection("PSW :","right",4,16,20);
           textbackground(0);
           gotoxy(12,16);scanf("%s",&psw);
           if(!strcmp(psw,"007"))
            { textbackground(2);
              printdirection(" LA CLE PUBLIC E : ","right",8,20,15);gotoxy(30,20);textbackground(0);printf("%d",e);textbackground(2);
              printdirection(" LA CLE PUBLIC N : ","right",8,22,15);gotoxy(30,22);textbackground(0);printf("%d",n);textbackground(2);
              printdirection(" LA CLE PRIVEE D : ","right",8,24,15);gotoxy(30,24);textbackground(0);printf("%d",d);



            }
            else
             {

               textbackground(4);
                textcolor(15);
                printdirection("MOT DE PASSE INCORRECT","right",30,26,20);

             }
             gotoxy(10,30);
             system("pause");
             break;




    case 6:

          goto fin;

  }
  goto deb;
  fin:
  fin();
}

////////////////---------------------------------------------------------SPECIALE AFFICHAGE ------------------------------------------
int Prog(FILE *f)
{

  static long z=0;
  int b=0;

  while(!eof(f))
    { fread(&z,sizeof(long),4,f);


       b++;

    }

    return b ;

}

int main(int argc, char *argv[])
{ long  *n,*e,*d;
  long h;
  unsigned char z,k ;
  char ligne[100]="hola it me it p";
  char l[10];
  char l2[10];


intro();

menu();
/*scanf("%s",&l);
printf("%c",l[2]);




  Search_Replace("t.txt",l,"clé");*/











    return 0;
}

