import os

res = os.name
print(res)
if (res == "posix") :
    print("votre systeme d'exploitation est " +  res + " : Linux")
elif (res == "nt") :
    print("Votre systeme d'exploitation est " +  res + " : Windows")