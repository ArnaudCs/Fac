import matplotlib.pyplot as plt
import numpy as np
from math import sqrt
import math
import random
from dessins import *

#fig, ax = plt.subplots()  # Create a figure containing a single axes.
#ax.plot([1, 2, 3, 4], [1, 4, 2, 3])  # Plot some data on the axes.
print(" -------- EXERCICE 1 --------")
print(" ")
print("Question 1 --------")
print(" ")

def distance(A, B) :
    xa = A[0]
    xb = B[0]
    ya = A[1]
    yb = B[1]
    s = sqrt((xb-xa)**2 + (yb-ya)**2)
    return s

A = (121, 77)
B = (48, 70)
C = (12, 72)

print("Ditance A-B =", distance(A, B))
print("Ditance A-c =", distance(A, C))
print("Ditance B-C =", distance(B, C))

print(" ")
print("Question 2 ---------")
print(" ")

P = [(6,20),(67,18),(96,4),(32,45)]
def aretes(P) : 
    L = []
    T = len(P)
    for i in range (T) :
        for j in range (i+1, T) :
            L.append([i, j, distance(P[i], P[j])])
    return L
        
print(aretes(P))

print(" ")
print("Question 3 ---------")
print(" ")

def pointsAleatoires(n, xmax, ymax) :
    L = []
    for i in range (n) :
        L.append([randint(0, xmax), randint(0, ymax)])
    return L

R = pointsAleatoires(4, 10, 20)
print("4 points aléatoires avec 0<x<10 et 0<y<20 :", R)
#dessinPoints(R)

print(" ")
print("Question 4 ---------")
print(" ")

def listesAdjacence(n, A) :
    L={}
    for i in range (n) :
        s = []
        for j in range (len(A)) :
            if A[j][0] == i:
                s.append(A[j][1])
            elif A[j][1] == i :
                s.append(A[j][0])
        L[i] = s 
    return L

A = [(0,1),(0,2),(0,3),(1,2),(1,3),(2,3)]
print("Liste d'adjacence de 4 points", listesAdjacence(4, A))

dessinGraphe(R, listesAdjacence(4, A))

print(" ")
print(" -------- EXERCICE 2 --------")
print(" ")
print("Question 1 --------")
print(" ")


def poidsMin(Arete):
  min = 0
  for i in range(len(Arete)):
    if Arete[i][2] < Arete[min][2]:
      min = i
  return min

def arbreCouvrant(Aretes, n) : 
    A = {}
    C = [i for i in range (n)]
    m = n
    for i in Aretes :
        if C[Aretes[i][0]] != C[Aretes[i][1]] :
            A = i
            for i in range (n) :
                if C[i] == C[Aretes[i][1]] :
                    m = m-1
        if m==1 :
            break
    return A        

A = aretes([(6,20),(67,18),(96,4),(32,45)])
print(arbreCouvrant(A, 4))

    
    
    
    
    
    
    
    
    