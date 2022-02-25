#include <stdio.h>
#include "image_ppm.h"
#include <iostream>
#include <fstream>
using namespace std;

int moyenne(OCTET* img,int i,int j, int nH, int nW) {
  return  (img[(i-1)*nW+(j-1)]+img[(i)*nW+(j-1)]+img[(i+1)*nW+(j-1)]+img[(i-1)*nW+(j)]+img[(i)*nW+(j)]+img[(i+1)*nW+(j)]+img[(i-1)*nW+(j+1)]+img[(i)*nW+(j+1)]+img[(i+1)*nW+(j+1)])/9;
}


