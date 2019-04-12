#!/usr/bin/python3

"""
yield : ce sont des points de préemptions qui remontent tout en haut
en indiquant le générateur qui doit être lancé.

Quand l'ordonnanceur veux faire avancer une fonction,
il fait 'next' pour faire avancer la tâche.

Une tâche est donc une fonction générateur avec comme attributs :
  * 'etat'       : Le générateur en cours d'exécution
  * 'attend'     : L'exécution du générateur est suspendue car il attend
                   la fin d'une entrée/sortie ou d'un timer.
"""



class Tache:
    def __init__(self, fonction):
        self.etat = fonction()
        self.attend = None

class Ordonnanceur:
    def __init__(self):
        self.taches = []
    def ajoute(self, tache):
        self.taches.append(Tache(tache))
    def boucle(self):
        while True:
            for tache in self.taches:
                if tache.attend:
                    continue
                try:
                    # On avance la tâche d'un yield
                    next(tache.etat)
                except StopIteration:
                    # La tâche est terminée
                    self.taches.remove(tache)
                    break

class Les_Entiers:
    def __init__(self):
        self.i = 0
    def read(self):
        yield        # Attend que l'ordonnanceur nous lance
        self.i += 1  # Fait le calcul
        return self.i    # La valeur de retour

class Afficheur:
    def write(self, texte):
        yield
        print('(', texte)
        yield
        print(')')
        return "OK"

class Processus:
    def __init__(self, entree, sortie):
        self.entree = entree
        self.sortie = sortie
    def start(self):
        while True:
            data = yield from self.entree.read()
            ok = yield from self.sortie.write(data*data)
            if ok != "OK":
                bug

class TicTac:
    def __init__(self, sortie):
        self.sortie = sortie
    def start(self):
        for i in range(10):
            yield from self.sortie.write('Tic')
            yield from self.sortie.write('Tac')

def main():
    ordonnanceur = Ordonnanceur()
    entree = Les_Entiers()
    sortie = Afficheur()
    ordonnanceur.ajoute(Processus(entree, sortie).start)
    ordonnanceur.ajoute(TicTac(sortie).start)
    ordonnanceur.boucle()

main()
