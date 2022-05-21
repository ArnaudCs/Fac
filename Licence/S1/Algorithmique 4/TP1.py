from random import *
from matplotlib import pyplot as plt
from datetime import datetime
#Exercice 1

def entiersAleatoires(n,a,b) :
    L = []
    for i in range (1, n+1) :
        L.append(randint(a,b))
    return L

R1 = entiersAleatoires(1000, 1, 100)

def entiersAleatoires2(n,a,b) :
    L = []
    for i in range (1, n+1) :
        L.append(randrange(a, b))
    return L

print(" ")
R2 = entiersAleatoires2(1000, 1, 100)
plt.hist(R1, bins=100)
plt.hist(R2, bins=100)
plt.show()

# [1, n] ce qui explique qu'on ai jamais 100 avec randint car il borne la valeur b 
#alors qu'avec randrange on a [1, n) qui nous permet d'avoir 100

print("----------------------------------------------")
#Exercice 2

def flottantsAleatoires(n) :
    L = []
    for i in range (0, n) :
        L.append(random())
    return L

print("FlottantsAleatoires")
print(flottantsAleatoires(10), "\n")

def flottantsAleatoires2(n, a, b) :
    L = []
    for i in range (0, n) :
        L.append(uniform(a, b))
    return L

print("FlottantsAleatoires2")
print(flottantsAleatoires2(10, 1, 100), "\n")
R3 = flottantsAleatoires(1000)
R4 = flottantsAleatoires2(1000, -3, 10)
plt.plot(R3)
plt.plot(R4)
plt.show()
#random fais que des tirages entre 0 et 1

print("----------------------------------------------")

def pointsDisque(n) : 
    L = []
    for i in range (0, n) :
        x = uniform(-1, 1)
        y = uniform(-1, 1)
        while (x**2 + y**2 ) > 1:
            x = uniform(-1, 1)
            y = uniform(-1, 1) 
        L.append([x, y])
    return L

print(pointsDisque(10), "\n")
    
def pointsDisque2(n) : 
    L = []
    for i in range (0, n) :
        x = uniform(-1, 1)
        y = uniform(-1, 1)
        while (x**2 + y**2 ) > 1:
            y = uniform(-1, 1) 
        L.append([x, y])
    return L

print(pointsDisque2(10))
R5 = pointsDisque(10000)
R6 = pointsDisque2(10000)

def affichagePoints(L):
    
    X = [x for x,y in L] # on récupère les abscisses...
    Y = [y for x,y in L] # ... et les ordonnées
    plt.scatter(X, Y, s = 1) # taille des points minimale
    plt.axis('square') # même échelle en abscisse et ordonnée
    plt.show()
    
affichagePoints(R5)
affichagePoints(R6)
print("Avec un x fixé on obtient plus de points sur les extrémités")
print("----------------------------------------------" + "\n")

#Exercice 4
def aleatoireModulo(N) :
    k = N.bit_length()
    return getrandbits(k) % N

print(aleatoireModulo(10))

def aleatoireRejet(N) :
    k = N.bit_length()
    x = getrandbits(k)
    while x > N:
        x = getrandbits(k)
    return x

print("aleatoireRejet = ", aleatoireRejet(10))

def suiteAleatoireBit(n, b):
    L = []
    for i in range (0, n) :
        L.append(aleatoireModulo(b))
    return L

#plt.hist(suiteAleatoireBit(10000, 101), bins=50)

#Exercice 2 
print("----------------------------------------------" + "\n")
T1 = [2, 1, 2, 1, 2, 2, 4, 1]
def eltMajDet(T) :
    cpt = 0
    for i in range (len(T)) :
        for j in range (len(T)) :
            if (T[i]==T[j]) :
                cpt = cpt + 1
        if cpt >= len(T)/2 :
            return T[i]
        
print("element majoritaire = ", eltMajDet(T1))
        
def eltMajProba(T) : 
    cpt = 0
    while 1 == 1:
        i = randint(0, len(T))
        for j in range (len(T)) :
            if (T[i]==T[j]) :
                cpt = cpt + 1
        if cpt >= len(T)/2 :
            return T[i]
        
print("element majoritaire 2 = ", eltMajProba(T1))
        
def tabAlea(n, a, b, k) :
    T = []
    x = randint(a, b)
    for i in range (0, k):
        T.append(x)
    for i in range(k, n) :
        y = randint(a ,b)
        while y == x :
            y = randint(a, b)
        T.append(y)
    shuffle(T)
    return T

print("TabAlea",tabAlea(10, 0, 10, 5))
print("----------------------------------------------" + "\n")

def tabDeb(n, a, b, k) :
    T = []
    x = randint(a, b)
    for i in range (0, k):
        T.append(x)
    for i in range(k, n) :
        y = randint(a ,b)
        while y == x :
            y = randint(a, b)
        T.append(y)
    return T

print("TabDeb = ", tabDeb(10, 0, 10, 5))

def tabFin(n, a, b, k) :
    T = []
    x = randint(a, b)
    for i in range(0, n-k) :
        y = randint(a ,b)
        while y == x :
            y = randint(a, b)
        T.append(y)
    for i in range (n-k, n):
        T.append(x)
    return T

print("TabFin", tabFin(10, 0, 10, 5))


start = datetime.now()
# some kind of code
T1 = tabAlea(1000, 0, 10, 500)
def eltMajDet(T) :
    cpt = 0
    for i in range (len(T)) :
        for j in range (len(T)) :
            if (T[i]==T[j]) :
                cpt = cpt + 1
        if cpt >= len(T)/2 :
            return T[i]
        
print("element majoritaire = ", eltMajDet(T1))
print(datetime.now() - start)

start = datetime.now()
# some kind of code
T1 = tabAlea(1000, 0, 10, 500)
def eltMajProba(T) : 
    cpt = 0
    while 1 == 1:
        i = randint(0, len(T))
        for j in range (len(T)) :
            if (T[i]==T[j]) :
                cpt = cpt + 1
        if cpt >= len(T)/2 :
            return T[i]
        
print("element majoritaire 2 = ", eltMajProba(T1))
print(datetime.now() - start)
print("----------------------------------------------" + "\n")

#Exercice 2.2
def suivant(xn, a, c, m) :
    return a*xn + c) % n
    
def valeurs(x0, a, c, m, N) : 
    














