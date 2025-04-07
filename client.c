#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock_client;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    int guess;

    // Création du socket
    sock_client = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_client == -1) {
        perror("Erreur de création du socket");
        exit(1);
    }

    // Configuration du serveur
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connexion
    if (connect(sock_client, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Erreur de connexion");
        close(sock_client);
        exit(1);
    }

    printf(" Connecté au serveur.\n");

    // Envoi du pseudo
    printf("Entrez votre pseudo : ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    send(sock_client, buffer, strlen(buffer), 0);

    // Boucle de jeu
    while (1) {
        printf("Entrez un nombre entre 0 et 100 (ou 'exit' pour quitter) : ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "exit") == 0) {
            send(sock_client, buffer, strlen(buffer), 0);
            printf(" Déconnexion demandée.\n");
            break;
        }

        // Convertir et envoyer le nombre
        guess = atoi(buffer);
        if (guess == 0 && strcmp(buffer, "0") != 0) {
            printf(" Entrée invalide. Essayez encore.\n");
            continue;
        }

        if (send(sock_client, &guess, sizeof(guess), 0) == -1) {
            perror("Erreur d'envoi");
            break;
        }

        // Réponse du serveur
        memset(buffer, 0, BUFFER_SIZE);
        int n = recv(sock_client, buffer, BUFFER_SIZE - 1, 0);
        if (n <= 0) {
            printf(" Déconnecté du serveur.\n");
            break;
        }

        buffer[n] = '\0';
        printf(" Serveur : %s\n", buffer);

        if (strstr(buffer, "Bravo") || strstr(buffer, "dépassé") || strstr(buffer, "gagné")) {
            break;
        }
    }

    close(sock_client);
    printf(" Connexion fermée.\n");
    return 0;
}
