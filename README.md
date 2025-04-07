# Jeu de devinette

Ce projet est une implémentation en C d’un jeu de devinettes multijoueur utilisant les **sockets TCP/IP**. Le serveur choisit un nombre aléatoire entre 0 et 100, et les clients doivent le deviner. Le premier à trouver gagne !

---

## Fonctionnement du jeu

- Le **serveur** génère un nombre mystère.
- Les **clients** se connectent, s’identifient avec un **pseudo**, et tentent de deviner le nombre.
- Le serveur indique si la tentative est **trop petite**, **trop grande** ou **correcte**.
- Chaque client a droit à **10 tentatives**.
- Le premier joueur à trouver le nombre **gagne la partie**.
- Un message est envoyé à **tous les clients** pour annoncer le gagnant.
- Tous les clients sont ensuite déconnectés.

---

## Structure des fichiers

```
.
├── server.c     # Code source du serveur
├── client.c     # Code source du client
├── README.md    # Ce fichier
```

---

## Compilation

Utilisez `gcc` pour compiler les deux programmes :

### Serveur :

```bash
gcc server.c -o server -lpthread
```

### Client :

```bash
gcc client.c -o client
```

---

## Lancement

### 1. Lancer le serveur :

```bash
./server
```

Le serveur affiche le nombre à deviner dans la console (utile pour tester).

### 2. Lancer un ou plusieurs clients :

```bash
./client
```

Ensuite, entrez votre pseudo puis commencez à deviner.

---

## Commandes disponibles côté client

- Envoyez un nombre (entre 0 et 100) pour deviner.
- Tapez `exit` pour quitter la partie à tout moment.

---

## Exemple d’interaction

```bash
Entrez votre pseudo : Alice
Entrez un nombre entre 0 et 100 (ou 'exit' pour quitter) : 50
Serveur : Trop petit
Entrez un nombre entre 0 et 100 (ou 'exit' pour quitter) : 75
Serveur : Trop grand
Entrez un nombre entre 0 et 100 (ou 'exit' pour quitter) : 62
Serveur : Bravo, vous avez gagné !
```

---

## Remarques

- Le serveur accepte plusieurs connexions simultanées via **pthread**.
- Si un client dépasse les 10 tentatives, il est déconnecté.
- Le serveur ferme toutes les connexions une fois que quelqu’un gagne.

---

## Améliorations possibles

- Relancer une partie automatiquement.
- Ajouter un système de scores ou de classement.
- Interface graphique en C.
- Envoyer directement un message à tout les clients quand un utilisateur a gagné

---

## 👤 Auteur

Projet réalisé par **Rajaonarivo Fiderana** , **Randrianjanaka Israel** , **Igiraneza Bessy**

---