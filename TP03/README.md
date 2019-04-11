# MIF12-Lab 3 ASYNC

  * Thierry Excoffier,  Université Lyon 1, LIP [email](mailto:thierry.excoffier@univ-lyon1.fr)
  * Version: 2019.04 
  * Deadline : 29/3 18H sur Tomuss (à préciser?)

## Mini Cours

Sur la page web de Thierry [ici](http://perso.univ-lyon1.fr/thierry.excoffier/COURS/COURS/TEMPS_REEL/cours.html)


### Quelle architecture pour quelle application.

+----------------+------------------------------------------------------------+
| Parallélisme   | Un traitement unique prenant du temps.                     |
|                |                                                            |
|                | On le répartit le travail intelligemment.                  |
+----------------+------------------------------------------------------------+
| Temps partagé  | Partager le temps d'un processeur entre des tâches.        |
|                |  * mauvaise idée en non-interactif                         |
|                |  * augmente le temps de réponse                            |
|                |  * casse les caches                                        |
|                |  * changement de contexte.                                 |
|                |                                                            |
|                | Souvent utilisé car les IO sont synchrones.                |
|                |                                                            |
|                | Très utile quand on ne connaît pas le temps d'exécution.   |
+----------------+------------------------------------------------------------+
| Multi-tâche    | Gestion des zones critiques.                               |
|                |                                                            |
|                | Optimum : une tâche par processeur                         |
+----------------+------------------------------------------------------------+
| Asynchrone /   | Une seule tâche traitant une liste d'événements.           |
|                |                                                            |
| Événementiel   | Utilisation optimum des ressources CPU et cache.           |
|                |                                                            |
|                | Avantage : pas de section critique.                        |
|                |                                                            |
|                | Inconvénient : les traitements doivent être courts.        |
|                | car non-préemptible.                                       |
+----------------+------------------------------------------------------------+
| Ordonnancement | Le serveur optimise la liste des événements à traiter      |
| applicatif     | afin de minimiser l'utilisation CPU et disque.             |
|                |                                                            |
|                | Par exemple détruire 2 caractères plutôt que faire         |
|                | deux destructions de 1 caractère.                          |
+----------------+------------------------------------------------------------+

### La notion de 'continuation'

La programmation événementielle n'est pas simple car les programmes ne sont plus écrits linéairement.

La programmation asynchrone peut être faite en utilisant la notion de **continuation**.

Contrairement au ``return`` qui termine une fonction
la continuation permet de suspendre l'exécution
jusqu'à ce que l'appelant la redémarre.

Googlez : ``Coroutines in C``

En Python, elles font partie du langage :

```Python
    def les_cubes():
      i = 0
      while True:
        yield i * i * i
        i += 1
  
    for cube in les_cubes():
      print(cube)
```
Concaténation de 2 itérateurs :

```Python
	def concat(a, b):
        for i in a:
            yield i
        for i in b:
            yield i
  
    for x in concat(['a', 'b'], les_cubes()):
        print(x)
```

Le ``yield from``:

```Python
    def concat(a, b):
        yield from a
        yield from b
```
Iterateur retournant une valeur finale :

```Python
    def calcule():
        for i in range(10):
            # Calcul long
            yield i
        return 42
    
    def affiche(generateur):
        try:
            while True:
                print("etape=", next(generateur))
        except StopIteration as retour:
            print(retour)
    
    affiche(calcule())
```
Version avec ``yield from``

```Python
    def affiche(generateur):
        retour = yield from generateur
        print(retour)
    
    for i in affiche(calcule()):
        print(i)
```
On peut envoyer une valeur à ``yield`` :

```Python
    def additionneur():
        sum = 0
        while True:
            v = yield sum
            sum += v
    
    a = additionneur()
    a.send(None) # Fait démarrer la fonction (reçoit 0)
    for i in range(10):
        print(i, a.send(i))
```
On peut imaginer les fonctions comme des processus
qui ont une file d'événements à traiter qu'elles lisent
avec ``yield`` et qu'elle peuvent envoyer à d'autres
avec ``send``.
Mais pour le moment tout les traitements sont synchrones.


### Programmation asynchrone en Python.
===================================

Toutes les entrées sorties utilisent des continuations
afin de rendre la main à l'orchestrateur.

  * Pas bloquante.
  
  * Lecture linéaire du programme.

  * Dans le répertoire _code : `mini.py` version avec ``yield from``, et
    `mini_2.py`, version avec ``await`` 



## Sujet Python

The kick-off code of this lab is available under the [code directory](https://github.com/lauregonnord/mif18-labs/blob/master/TP03/_code)

`demo_async` et `demo_async2`

### Rendu 

* Deadline 29/3/2019, 18H sous tomuss.
