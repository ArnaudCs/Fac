// test_couleur.cpp : Seuille une image en niveau de gris

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

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

 for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       if(i == 0 || j == 0 || i == nH || j == nW) {ImgOut[i*nW+j] = ImgIn[i*nW+j];}
       else ImgOut[i*nW+j] = (ImgIn[i*nW+j] + ImgIn[(i-1)*nW+j] + ImgIn[(i+1)*nW+j] + ImgIn[i*nW+(j-1)] + ImgIn[i*nW+(j+1)])/5;
     }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn); free(ImgOut);

   return 1;
}