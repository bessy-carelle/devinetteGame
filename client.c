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

    // Créer le socket
    sock_client = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_client == -1) {
        perror("Erreur de création du socket");
        exit(1);
    }

    // Configuration de l’adresse du serveur
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connexion au serveur
    if (connect(sock_client, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Erreur de connexion");
        close(sock_client);
        exit(1);
    }

    printf("Connecté au serveur.\n");

    // Envoyer le pseudo
    printf("Entrez votre pseudo : ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    send(sock_client, buffer, strlen(buffer), 0);

    // Boucle principale du jeu
    while (1) {
        printf("Tapez 'exit' pour quitter sinon ENTREZ VOTRE REPONSE: ");
        char input[BUFFER_SIZE];
        fgets(input, sizeof(input), stdin);

        if (strncmp(input, "exit", 4) == 0) {
            send(sock_client, "exit", strlen("exit"), 0);
            break;
        }

        int guess = atoi(input);
        send(sock_client, &guess, sizeof(guess), 0);

        // Lire la réponse du serveur
        memset(buffer, 0, BUFFER_SIZE);
        recv(sock_client, buffer, BUFFER_SIZE - 1, 0);
        printf("Serveur : %s\n", buffer);

        if (strstr(buffer, "gagné") != NULL || strstr(buffer, "Au revoir") != NULL)
            break;
    }

    close(sock_client);
    printf("Connexion fermée.\n");
    return 0;
}
