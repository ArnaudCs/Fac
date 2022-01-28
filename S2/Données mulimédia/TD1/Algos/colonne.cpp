// histo.cpp : renvoie un histogramme des niveaux de gris de l'image

#include <stdio.h>
#include "image_ppm.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
    char cNomImgLue[250], typeofProfile[1];
    int nH, nW, nTaille, numberRowCol;

    if (argc != 4) 
        {
        printf("Usage: ImageIn.pgm Type Number\n"); 
        exit (1) ;
        }

    sscanf (argv[1],"%s",cNomImgLue);
    sscanf (argv[2],"%s",typeofProfile);
    sscanf (argv[3],"%d",numberRowCol);
    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    int histogramme[256] = {0};

    if (typeofProfile == "c")
    {
        for (int i=0; i < nH; i++) {
            histogramme[ImgIn[i*nW+numberRowCol]]++;
        }
    }
    else if (typeofProfile == "l")
    {
        for (int j=0; j < nW; j++) {
            histogramme[ImgIn[numberRowCol*nW+j]]++;
        }
    }

    FILE* f;
    f = fopen("profil.dat", "w");
    if (f == NULL) {
        printf("God damn !");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 256; i++) {
        fprintf(f, "%d\t%d\n", i, histogramme[i]);
    }
    fclose(f);
    free(ImgIn);
    return 1;
}

