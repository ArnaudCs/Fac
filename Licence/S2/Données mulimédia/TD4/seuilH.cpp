#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S, S2;
  
  if (argc != 5) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm Seuil \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);
   sscanf (argv[3],"%d",&S);
   sscanf (argv[4],"%d",&S2);

   OCTET *ImgIn, *ImgOut, *ImgOut2;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
   allocation_tableau(ImgOut2, OCTET, nTaille);

 for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       if (ImgIn[i*nW+j] <= S) {ImgOut[i*nW+j] = 0;}
       else if (ImgIn[i*nW+j] >= S2) {ImgOut[i*nW+j] = 255;}
       else {ImgOut[i*nW+j] = ImgIn[i*nW+j];}
     }

 for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       if ((ImgOut[i*nW+j] > S) && (ImgOut[i*nW+j] < S2) && (ImgOut[(i-1)*nW+j] == 255 || ImgOut[(i+1)*nW+j] == 255|| ImgOut[i*nW+(j-1)] == 255 || ImgOut[i*nW+(j+1)] == 255 || ImgOut[(i-1)*nW+(j-1)] == 255 || ImgOut[(i-1)*nW+(j+1)] == 255 || ImgOut[(i+1)*nW+(j-1)] == 255 || ImgOut[(i+1)*nW+(j+1)] == 255)) {ImgOut2[i*nW+j] = 255;}
       else {ImgOut2[i*nW+j] = 0;}
     }

   ecrire_image_pgm(cNomImgEcrite, ImgOut2,  nH, nW);
   free(ImgIn); free(ImgOut); free(ImgOut2);

   return 1;
}