#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
  
    if (argc != 3) 
    {
        printf("Usage: ImageIn.pgm ImageOut.pgm\n"); 
        exit (1) ;
    }
   
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);

    OCTET *ImgIn, *ImgOut, *ImgOut1, *ImgOut2;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    int gradV, gradH;
    for (int i=0; i < nH; i++) {
        for (int j=0; j < nW; j++) {
            if (i == 0 || i == nH) {
                gradV = ImgIn[i*nW+j];
            }
            else {
                gradV = ImgIn[(i+1)*nW+j] - ImgIn[(i-1)*nW+j];
            }
            if (j == 0 || j == nW) {
                gradH = ImgIn[i*nW+j];
            }
            else {
                gradH = ImgIn[i*nW+j+1] - ImgIn[i*nW+j-1];
            }
            ImgOut[i*nW+j] = sqrt((gradV*gradV) + (gradH * gradH));
        }
    }
    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);

    return 1;
}