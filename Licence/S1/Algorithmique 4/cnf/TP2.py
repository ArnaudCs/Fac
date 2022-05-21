#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Oct 13 21:55:02 2021

@author: arnaud.cossu@etu.umontpellier.fr
"""

from datetime import datetime


true = True
false = False
#Exercice 1
#Q1
print("\nQuestion 1")
print("---------------")

print("Résultat de la fonction clause")
def clause(s) :
    L1 = s.split()
    L = [int(x) for x in L1]
    del L[-1]
    return L

print("1 2 -3 0 = ", clause("1 2 -3 0 \n"))


#Q2
print("\nQuestion 2")
print("---------------")
def parseur(nom) :
    L = []
    F = []
    n = 0
    with open(nom + ".cnf") as f:
        for ligne in f :
          if ligne != "" :
            L.append(ligne)

    for i in range (0, len(L)):
      if (L[i] != "\n") and (L[i][0] != "c") :
        if L[i][0] == "p" :
          n = L[i].split()[-2]
        else :
          F.append(clause(L[i]))
            
    return (F, n)

print("\nRésultat de la fonction parseur")
print(parseur("simple_v3_c2"))

#Exercice 1
#Q2.1
print("\nQuestion 2.1")
print("---------------")
def est_valide(F, I):
  for i in range (0, len(F)) :
    clause = false
    for j in range (0, len(F[i])) :
      if (F[i][j] * I[abs(F[i][j])-1]) > 0 :
        clause = true
    if not(clause) :
      return false
  return true



F = parseur("simple_v3_c2")[0]
print("Résultat de est valide :", est_valide(F, [1, 1, -1]))
print("Résultat de est valide :", est_valide(F, [-1, -1, -1]))

#Q2.2
print("\nQuestion 2.2")
print("---------------")
def aff_suivante(A) :
    n = len(A)
    i = 0
    while i<n and A[i] == 1 :
        A[i] = -1
        i = i + 1
    if i == n :
        return "Fin"
    A[i] = 1
    return A

print("Résultat de affiche suivante :", aff_suivante([1, 1, -1]))

#Q2.3
print("\nQuestion 2.3")
print("---------------")
def sat_exhau(F, n) :
    A = []
    for i in range (0, n):
        A.append(-1)
    while not(est_valide(F, A)) :
        A = aff_suivante(A)
    if A == "Fin" :
        return "Insatisfiable"
    return A

print(F)
F1 = parseur("random-10-sat")
print("Resultat sat_exhau de F :", sat_exhau(F, 3))
print("Resultat sat_exhau de random 10 :", sat_exhau(F1[0], int(F1[1])))

print("\nQuestion 3.1")
print("---------------")

def elimination(F, n, b) :
    H = []
    for c in F :
        T = []
        SAT = False
        for l in c :
            if abs(l) == n and (l*b)>0 : 
                SAT = True
            elif abs(l) != n :
                T.append(l)
        if not(SAT) :
            H.append(T)
    return H

F4 = parseur("random-5-sat")
print("Resultat pour élimination de random-5 :", elimination(F4[0], int(F4[1]), -1))

print("\nQuestion 3.2")
print("---------------")
def sat_backtrack(F, n) :
    A = []
    if F == []:
        for i in range (0, n) :
            A.append(1)
        return A
    if ([] in F) :
        return "Insatisfiable"
    for b in range (-1, 2, 2) :
        T = elimination(F, n, b)
        A = sat_backtrack(T, n-1)
        if A != "Insatisfiable" :
            return A + [b]
    return "Insatisfiable"

H6 = parseur("hole6")
print(sat_backtrack(F4[0], int(F4[1])))
print(sat_backtrack(H6[0], int(H6[1])))
print(sat_backtrack(F, 3))
    

#Exercice 2
print("\nExercice 2\n")

print("\nQuestion 1.1")
print("---------------")
    
def lecture_sudoku(nom) :
    L = []
    L1 = []
    with open(nom + ".txt") as f:
        for ligne in f :
            s = ligne.split()
            L1 = [int(x) for x in s]
            for x in L1 :
                L.append(x)
    x = L[0]
    del L[0]
    return (L, x)

print(lecture_sudoku("sudoku-3-moyen-7"))

print("\nQuestion 1.2")
print("---------------")
def ecriture_sudoku(G, n, nom) :
    with open(nom + ".txt", 'w') as f:
        f.write(str(n)+"\n")
        for i in range (0, n**2) :
            for j in range(0, n**2) :
                if j != 0 :
                    f.write(" ")
                f.write(str(G[i*(n**2) + j]))
            f.write("\n")

G = lecture_sudoku("sudoku-3-moyen-7")[0]
ecriture_sudoku(G, 3, "philippe")
S1 = lecture_sudoku("sudoku-3-facile-2")
ecriture_sudoku(S1[0], S1[1], "zjohn")
                
print("\nQuestion 1.3")
print("---------------")
def affiche_sudoku(G, n) :
    for i in range (0, n**2) :
        for j in range(0, n**2) :
            if j != 0 :
                print(" ", end = "")
            if (G[i*(n**2) + j]) == 0 :
                print("_", end="")
            else :
                print((G[i*(n**2) + j]), end="")
        print("")
        
affiche_sudoku(G, 3)
    
print("\nQuestion 2.1")
print("---------------")
def zone(n, u):
    L = []
    i = int(u / (n**2))
    j = int(u % (n**2))
    zi = int(i / n)*n
    zj = int(j / n)*n
    for k in range(n):
        for l in range(n):
            L.append((zi+k)*(n**2)+(zj+l))
    return L

for i in range(0, 81, 12):
    print("zone(3,",i,") :", zone(3, i))
    
print("\nQuestion 2.2")
print("---------------")
def valide(G, n, u, x):
    G[u] = 0
    for i in zone(n, u):
        if x == G[i]:
            return False
    j = u % (n**2)
    i = int(u / (n**2))
    for k in range(n**2):
        if (k != j and x == G[i*(n**2)+k]) or (k != i and x == G[k*(n**2)+j]):
            return False
    return True

for i in range(9):
    print(i+1,"en 5 : ",valide(S1[0], S1[1], 5, i+1))
    
print("\nQuestion 2.3")
print("---------------")
def sudoku(nom):
    G = lecture_sudoku(nom)
    print(nom, ":")
    affiche_sudoku(G[0], G[1])
    print("résolu : ")
    temps1 = datetime.now()
    affiche_sudoku(BT_sudoku(G[0], G[1], 0), G[1])
    print("temps mis =",datetime.now()-temps1,"\n") 


def BT_sudoku(G, n, u):
    #print('')
    #affiche(G,n)
    while (u < n**4) and (G[u] != 0):
        u = u + 1
    if u == (n**4):
        return G
    for x in range (n**2):
        if valide(G, n, u, x+1):
            G[u] = x+1
            S = BT_sudoku(G, n, u+1)
            if isinstance(S, list):
                return S
    G[u] = 0
    return False

print("\nQuestion 3.1")
print("---------------")
def suivant(T):
    n = len(T)
    i = 1
    while i < n and T[i-1]>T[i]:
        i += 1
    if i==n:
        return "Fin"

    j = 0
    for i in range(n-1):
        if T[i] < T[i+1]:
            j = i
    l = 0
    for i in range(n):
        if T[j]<T[i]:
            l = i
    tmp = T[j]
    T[j] = T[l]
    T[l] = tmp
    
    for i in range(1, int((n-j)/2)+1):
        tmp = T[j+i]
        T[j+i] = T[n-i]
        T[n-i] = tmp
    return T

T1 = [x for x in range(6)]

print("\nQuestion 3.2")
print("---------------")
def dico(a, b, c):
    D = {}
    cpt = 1
    for x in (a, b, c):
        for i in range(len(x)):
            if x[i] not in D.keys():
                D[x[i]] = cpt
                cpt += 1
    return D

D1 = dico('OASIS', 'SOLEIL', 'MIRAGE')
print(D1)
    
print("\nQuestion 3.4")
print("---------------")
def valeur(m, D, p) :
    for i in range(len(p)) :
        l = m[i]
        x = D[l]
        a = p[x]
        m[i] = a
    return m
                        
print("\nQuestion 4.1")
print("---------------")
def affichage_reine(Q) :
    for i in range(len(Q)) : 
        for j in range(len(Q)) :
            if Q[i]==j :
                print("X",end=" ")
            else :
                print("_",end=" ")
        print("")
    print("")

print("affichage_reine de [1, 3, -1, -1] :")
affichage_reine([1, 3, -1, -1])

print("\nQuestion 4.2")
print("---------------")

def reine_possible(Q, x, y):
    if (Q[x] != -1):
      return False
    Q[x] = y
    for i in range(len(Q)-1):
        for j in range(i+1, len(Q)):
            if (Q[i] != -1) and (Q[j] != -1) and ((Q[i] == Q[j]) or (Q[j]==Q[i]+j-i) or (Q[j]==Q[i]-j+i)):
                return False
    return True

print(reine_possible([1, 3, -1, -1], 2, 2))

print("\nQuestion 4.3")
print("---------------")


def placement_reines(Q,r):
  if r==len(Q):
    return Q
  else:
    for i in range(len(Q)):
      ok = True
      for j in range(r):
        if Q[j]==i or Q[j]==i+r-j or Q[j]==i-r+j:
          ok = False
      if ok:
        Q[r]=i
        S = placement_reines(Q,r+1)
        if isinstance(S, list):
          return S
    return False

placement_reines([1,3,-1,-1],2)

print("\nQuestion 4.4")
print("---------------")

def reine(n):
  Q = []
  for i in range(n):
    Q.append(-1)
  S = placement_reines(Q,0)
  if isinstance(S, list):
    affichage_reine(S)
  else:
    print("Pas possible")


for i in range(5):
  print("i =",i+1)
  reine(i+1)


    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    