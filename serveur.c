#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_TRIES 10  // Nombre maximum de tentatives par client

int number_to_guess;  // Le nombre à deviner pour tous les clients
int client_sockets[1024];  // Tableau des sockets des clients
int client_count = 0;  // Compteur de clients connectés

void broadcast_message(const char *message) {
    // Envoie un message à tous les clients connectés
    for (int i = 0; i < client_count; i++) {
        send(client_sockets[i], message, strlen(message), 0);
    }
}

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    int guess = -1;
    int attempts = 0;  // Compteur de tentatives
    char buffer[BUFFER_SIZE] = {0};

    char pseudo[BUFFER_SIZE];
    memset(pseudo, 0, BUFFER_SIZE);

    // Lire le pseudo envoyé par le client
    int pseudo_len = recv(client_socket, pseudo, BUFFER_SIZE - 1, 0);
    if (pseudo_len <= 0) {
        printf("Erreur lors de la réception du pseudo.\n");
        close(client_socket);
        free(arg);
        pthread_exit(NULL);
    }
    pseudo[pseudo_len] = '\0';
    printf(" Nouveau joueur connecté : %s\n", pseudo);


    while (1) {
        // Lire le nombre envoyé par le client
        memset(buffer, 0, BUFFER_SIZE);
        int valread = recv(client_socket, buffer, BUFFER_SIZE - 1, MSG_PEEK);
        if (valread <= 0) {
            printf("Client %s déconnecté.\n", pseudo);
            break;
        }

        // Vérifie si c'est du texte (exit)
        if (valread < sizeof(int)) {
            recv(client_socket, buffer, BUFFER_SIZE - 1, 0);  // Consommer
            buffer[valread] = '\0';
            if (strcmp(buffer, "exit") == 0) {
                printf("🚪 %s a quitté la partie.\n", pseudo);
                break;
            } else {
                strcpy(buffer, "Commande inconnue.");
                send(client_socket, buffer, strlen(buffer), 0);
                continue;
            }
        }

        // Lecture de l'entier normalement
        recv(client_socket, &guess, sizeof(guess), 0);


        if (valread <= 0) {
            printf("Client déconnecté.\n");
            break;
        }

        printf("Tentative du client : %d\n", guess);
        attempts++;  // Incrémenter le compteur de tentatives

        // Vérifier si le client a dépassé le nombre de tentatives
        if (attempts > MAX_TRIES) {
            strcpy(buffer, "Vous avez dépassé le nombre maximum de tentatives. Vous êtes déconnecté.");
            send(client_socket, buffer, strlen(buffer), 0);
            close(client_socket);
            break;  // Quitter la boucle et déconnecter le client
        }

        // Vérifier la tentative
        if (guess < number_to_guess) {
            strcpy(buffer, "Trop petit");
        } else if (guess > number_to_guess) {
            strcpy(buffer, "Trop grand");
        } else {
        printf(" %s a deviné le bon nombre (%d) !\n", pseudo, number_to_guess);

        strcpy(buffer, "Bravo, vous avez gagné !");
        send(client_socket, buffer, strlen(buffer), 0);

        // Annonce du gagnant à tous les clients
        char announce[BUFFER_SIZE];
        snprintf(announce, BUFFER_SIZE, " Le joueur %s a deviné le bon nombre !\n", pseudo);
        broadcast_message(announce);

            // Fermer la connexion avec tous les clients
            for (int i = 0; i < client_count; i++) {
                close(client_sockets[i]);
            }

            break; // Fin de la partie, on quitte la boucle
        }

        // Envoyer un retour au client
        send(client_socket, buffer, strlen(buffer), 0);
    }

    // Retirer le client de la liste des clients
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] == client_socket) {
            // Déplacer les éléments du tableau pour combler le vide
            for (int j = i; j < client_count - 1; j++) {
                client_sockets[j] = client_sockets[j + 1];
            }
            client_count--;
            break;
        }
    }

    // Fermer le socket du client
    free(arg);  // Libérer la mémoire allouée pour le socket
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));  // Initialisation du générateur de nombres aléatoires
    number_to_guess = rand() % 101; // Génération du nombre à deviner entre 0 et 100
    printf("Le nombre à deviner est : %d\n", number_to_guess); // Pour vérifier, uniquement côté serveur

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Accepte toute connexion locale
    address.sin_port = htons(PORT);

    // Attachement du socket au port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }

    // Écoute des connexions entrantes
    if (listen(server_fd, 5) < 0) {
        perror("Erreur listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion...\n");

    while (1) {
        // Accepter une nouvelle connexion
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Erreur accept");
            continue;
        }

        printf("Nouveau client connecté.\n");

        // Ajouter le socket du client à la liste des clients
        client_sockets[client_count++] = new_socket;

        // Allouer de la mémoire pour le socket du client
        int *client_socket = malloc(sizeof(int));
        *client_socket = new_socket;

        // Créer un thread pour gérer le client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_socket) != 0) {
            perror("Erreur pthread_create");
            free(client_socket);
            close(new_socket);
        }

        // Détacher le thread pour qu'il se libère automatiquement
        pthread_detach(thread_id);
    }

    // Fermer le socket du serveur (ce code ne sera jamais atteint dans cette boucle infinie)
    close(server_fd);

    return 0;
}
