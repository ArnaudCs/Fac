// Histogramme d'une image

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm Seuil \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut, *Histo;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
  
  for(int i=0;i<256;i++)
    Histo[i]=0;

  for(int i=0;i<nW;i++)
    for(int j=0;j<nH;j++)
       Histo[ImgIn[i][j]]++;

   ecrire_image_pgm(cNomImgEcrite, Histo, nW, nH);
   free(ImgIn); free(ImgOut);

   return 1;
}
