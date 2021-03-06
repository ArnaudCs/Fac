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

   OCTET *ImgIn, *ImgOut, *ImgOut2;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
   allocation_tableau(ImgOut2, OCTET, nTaille);
  
   //   for (int i=0; i < nTaille; i++)
   // {
   //  if ( ImgIn[i] < S) ImgOut[i]=0; else ImgOut[i]=255;
   //  }

  for (int i=1; i < nH-1; i++) {
      for (int j=1; j < nW-1; j++) {
          if(ImgIn[(i-1)*nW+(j-1)] == 0 || ImgIn[(i-1)*nW+j] == 0 || ImgIn[(i-1)*nW+(j+1)] == 0 || ImgIn[(i)*nW+(j-1)] == 0 || ImgIn[(i)*nW+(j+1)] == 0 || ImgIn[(i+1)*nW+(j-1)] == 0 || ImgIn[(i+1)*nW+j] == 0 || ImgIn[(i+1)*nW+(j+1)] == 0) {
                ImgOut[i*nW+j]= 0;
          }
          else {
                ImgOut[i*nW+j]=255;
          }
      }
  }

// début de la dilatation 
  for (int i=1; i < nH-1; i++) {
        for (int j=1; j < nW-1; j++) {
            if(ImgOut[(i-1)*nW+(j-1)] == 255 || ImgOut[(i-1)*nW+j] == 255 || ImgOut[(i-1)*nW+(j+1)] == 255 || ImgOut[(i)*nW+(j-1)] == 255 || ImgOut[(i)*nW+(j+1)] == 255 || ImgOut[(i+1)*nW+(j-1)] == 255 || ImgOut[(i+1)*nW+j] == 255 || ImgOut[(i+1)*nW+(j+1)] == 255) {
                ImgOut2[i*nW+j]= 255;
            }
            else {
                ImgOut2[i*nW+j]=0;
            }
        }
  }



   ecrire_image_pgm(cNomImgEcrite, ImgOut2,  nH, nW);
   free(ImgIn); free(ImgOut);
   return 1;
}