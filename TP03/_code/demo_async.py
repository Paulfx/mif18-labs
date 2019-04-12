#!/usr/bin/python3

"""
Ce programme (mono thread) contient :
  * Une tâche 'tictac' qui affiche une valeur toutes les secondes pendant
    20 secondes.
  * Une tâche 'read_file' qui fait l'écho de ce qui est tapé au clavier
    et lance le calcul du MD5 de manière asynchrone.
  * Un serveur socket 'echo' qui fait l'écho de ce qu'on lui envoie.
  * Un client 'client_echo' qui vérifie si le serveur fonctionne.
  * Un serveur HTTP qui affiche la page web avec le MD5 de la requête.
  * Une tâche 'incremente' qui boucle en incrémentant 2 valeurs.
  * Une tâche 'verifie' qui boucle en vérifiant que
    les 2 valeurs incrémentées sont toujours identiques.
  * Une tâche 'overflow' qui lance plein de 'client_echo' en même temps
    pour faire planter le serveur.
  * Une tâche 'parallel_requests' qui lance 2 requêtes en parallèle
    et attend les résultats.
  * Une tâche 'md5sum' lancée à la demande par le serveur web.

Votre travail :

  * Lire ce code (n'hésitez pas à poser des questions)
  * Lancez-le et regardez ce qui est affiché
  * Vérifiez visuellement que les 2 requêtes en parallèles sont
    bien lancées en parallèle.
  * Tapez des choses au clavier (terminez par 'Entrée')
  * Lancez, éventuellement en parallèle :
            telnet 127.0.0.1 10000
    Tapez des choses au clavier (terminez par 'Entrée')
    Fermez la connexion par les deux méthodes, qu'est-ce que cela change ?
  * Cassez la section critique modifiant 'information' en ajoutant un 'yield'
    'yield' permet de quitter temporairement la fonction en retournant
    une valeur à la fonction appelante.
    Les points d'ordonnancement sont seulement à ces moments.
  * Pourquoi 'tictac()' est pseudo périodique et pas périodique ?
  * En supposant qu'un seule tâche prenne 100% du CPU, quelle est la durée
    maximale de retard de redémarrage d'une tâche ?

Moyennant quelques macros, on peut implémenter tout ceci en langage C.

On peut aussi tout réécrire en utilisant les mots-clefs : async et await

Toutes les tâches tournent dans un seul processus sans thread,
sauf MD5SUM qui lance un processus pour simuler une tâche
prenant beaucoup de CPU.

"""

import sys
import asyncio
import os
import functools
import cgi
import concurrent

default_port = 10000
http_port = 8080
message = b"Fermer la connexion avec ^D ou bien ^] puis 'quit'\n"
CtrlD = b"\004"
colors = {'T': 5, 'C': 3, 'E': 2, 'R': 6, 'M': 7, 'H': 4, 'P': 1}
backlog = 50            # Argument de 'listen' pour la file d'attente accept
overflow_wait = 1       # Nb de secondes avant de démarrer l'overflow du serveur
overflow_nb = 10        # Nb de requêtes lancées en même temps
overflow_smooth = False # Avec True ouvre les connexions progressivement

if not hasattr(asyncio, "ensure_future"): # Vieille version de Python
    asyncio.ensure_future = asyncio.async

@functools.lru_cache(maxsize=None)
def color(c):
    "Chaîne de caractères permettant de changer la couleur d'affichage"
    with os.popen("tput setaf {}".format(colors.get(c.upper(), 7))) as f:
        return f.read().strip()

def log(*args, **keys):
    "Affichage colorié, temps réel, sans retour à la ligne"
    keys["flush"] = True
    keys["end"] = ""
    args = list(args)
    args[0] = color(args[0][0]) + args[0]
    args[-1] += color('') + ' '
    print(*args, **keys)

def tictac():
    ": Tâche pseudo-périodique"
    log("TictacStart")
    for dummy in range(20):
        log("T{}".format(information[0]))
        yield from asyncio.sleep(1)
    log("TictacStop")

def read_file(f):
    ": Lecture de fichier ligne par ligne"
    reader = asyncio.StreamReader()
    reader_protocol = asyncio.StreamReaderProtocol(reader)
    asyncio.get_event_loop().connect_read_pipe(lambda: reader_protocol, f)
    yield from asyncio.get_event_loop().connect_read_pipe(
        lambda: reader_protocol, f)

    log("ReadfileStart")
    while True:
        line = yield from reader.readline()
        if line == b'':
            break
        line = line.decode("utf-8").strip()
        log("Readfile:{}".format(line))
        asyncio.ensure_future(md5sum(line)) # Ajoute une tâche
    log("ReadfileStop")

def echo(port):
    ": Un serveur socket 'echo', tester avec : telnet 127.0.0.1 NuméroPort"
    class echoProtocol(asyncio.Protocol):
        def connection_made(self, transport):
            log('Eaccept')
            self.transport = transport
            self.transport.write(message)
        def data_received(self, data):
            if data.strip() == CtrlD:
                self.transport.close()
                return
            log('Eanswer')
            self.transport.write(data)
        def connection_lost(self, exc):
            log('Eclose')
        def eof_received(self):
            log('EEOF')
    log("EStart{}\n".format(port))
    loop = asyncio.get_event_loop()
    server = yield from loop.create_server(echoProtocol, '127.0.0.1', port,
                                           backlog=backlog)
    yield from server.wait_closed()
    log("EStop")

def client_echo(port):
    ": Un client 'echo'"
    n = information[2]
    log("Cstart{}".format(n))
    information[2] = n + 1
    while True:
        try:
            log("COpen{}".format(n))
            reader, writer = yield from asyncio.open_connection('127.0.0.1',
                                                                port)
            log("CRead{}".format(n))
            # Le timeout arrive seulement si backlog=1
            # A lire pour votre culture générale :
            #   how-tcp-backlog-works-in-linux
            #   tcp-stuck-connection-mystery
            data = yield from asyncio.wait_for(reader.readline(), timeout=5)
            break
        except concurrent.futures.TimeoutError:
            writer.close()
            log("CTimeOut{}".format(n))
        except ConnectionRefusedError:
            log("CRefused{}".format(n))
            yield from asyncio.sleep(1) # Pour ne pas submerger le serveur
        
    assert(data == message)

    test = 'test{}\n'.format(n).encode()
    writer.write(test)
    data = yield from reader.readline()
    assert(data == test)

    writer.write(CtrlD)
    data = yield from reader.readline()
    assert(data == b'')

    writer.close()
    information[2] -= 1
    log("Cstop{}".format(n))

    return test

def md5sum(text):
    """: Processus fils gourmand en CPU pour faire le calcul MD5 des saisies."""
    process = yield from asyncio.create_subprocess_shell(
        "sleep 2 ; md5sum",
        stdout = asyncio.subprocess.PIPE,
        stdin = asyncio.subprocess.PIPE)
    process.stdin.write(text.encode())
    process.stdin.close()
    value = yield from process.stdout.readline()
    log("MD5:{}".format(value.decode().split(' ')[0]))
    yield from process.wait()
    return value

def http_server():
    ": Serveur HTTP sur le port 8080"
    class HTTPProtocol(asyncio.Protocol):
        def connection_made(self, transport):
            log('Hconnect')
            self.transport = transport
        def data_received(self, data):
            log('Hdata')
            self.transport.write(b"HTTP/1.0 200 OK\n")
            self.transport.write(b"Content-Type: text/html\n")
            self.transport.write(b"\n")
            data = data.decode() # XXX C'est totalement incorrecte
            if data.startswith("GET /md5 "):
                # On a pas le droit de faire "yield from self.md5(data)" ici
                asyncio.ensure_future(self.md5(data))
            else:
                self.transport.write(b"<h1>Request:</h1><pre>")
                self.transport.write(cgi.escape(data).encode())
                self.transport.write(b"</pre><h1>MD5</h1>")
                self.transport.write(b'<iframe src="/md5"></iframe>')
                self.transport.close()
            log('Hanswer')
        def md5(self, data):
            log('H<MD5>')
            md5 = yield from md5sum(data)
            self.transport.write(md5)
            self.transport.close()
            log('H</MD5>')
        def connection_lost(self, exc):
            log('Hclose')
        def eof_received(self):
            log('HEOF')
    log("HStart{}\n".format(http_port))
    loop = asyncio.get_event_loop()
    server = yield from loop.create_server(HTTPProtocol, '127.0.0.1',http_port)
    yield from server.wait_closed()
    log("HStop")


information = [0, 0, 0]
def incremente():
    ": Utilise 100% du temps CPU pour incrémenter les 2 champs."
    while True:
        information[0] += 1
        information[1] += 1
        yield

def verifie():
    ": Utilise 100% du temps CPU pour vérifier les 2 champs."
    while True:
        assert(information[0] == information[1])
        yield
        
def overflow():
    ": envoie des requêtes simultanées au serveur après quelques secondes"
    yield from asyncio.sleep(overflow_wait)
    for dummy in range(overflow_nb):
        # Ajoute une tâche
        asyncio.ensure_future(client_echo(default_port))
        if overflow_smooth:
            yield

def parallel_requests():
    ": lance 2 requêtes en parallèle et attend les 2 résultats"
    results = yield from asyncio.gather(
        client_echo(default_port),
        client_echo(default_port)
    )
    log('P' + repr(results))

def main():
    asyncio.ensure_future(tictac())
    asyncio.ensure_future(read_file(sys.stdin))
    asyncio.ensure_future(echo(default_port))
    asyncio.ensure_future(client_echo(default_port))
    asyncio.ensure_future(incremente())
    asyncio.ensure_future(verifie())
    asyncio.ensure_future(overflow())
    asyncio.ensure_future(http_server())
    asyncio.ensure_future(parallel_requests())

    functions = [f
                 for f in globals().values()
                 if callable(f) and f.__doc__ and f.__doc__[0] == ':'
                 ]
    functions.sort(key = lambda f: f.__code__.co_firstlineno)

    print("Liste des tâches :")
    for f in functions:
        print("  * {}{:11s}{} {}".format(
            color(f.__name__[0]), f.__name__, color(''), f.__doc__))

    asyncio.get_event_loop().run_forever()

main()
