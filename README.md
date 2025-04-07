## Jeu de devinette

Ce projet est une implémentation en C d’un jeu de devinettes multijoueur utilisant les **sockets TCP/IP**. Le serveur choisit un nombre aléatoire entre 0 et 100, et les clients doivent le deviner. Le premier à trouver gagne !

---

##  Fonctionnement du jeu

- Le **serveur** génère un nombre mystère.
- Les **clients** se connectent, s’identifient avec un **pseudo**, et tentent de deviner le nombre.
- Le serveur indique si la tentative est **trop petite**, **trop grande** ou **correcte**.
- Chaque client a droit à **10 tentatives**.
- Le premier joueur à trouver le nombre **gagne la partie**.
- Un message est envoyé à **tous les clients** pour annoncer le gagnant.
- Tous les clients sont ensuite déconnectés.

---

##  Structure des fichiers

```bash
.
├── server.c     
├── client.c     
├── README.md   