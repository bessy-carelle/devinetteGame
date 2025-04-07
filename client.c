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

    // 1. Création du socket
    sock_client = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_client == -1) {
        perror("Erreur de création du socket");
        exit(1);
    }

    // 2. Configuration du serveur
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Adresse locale

    // 3. Connexion au serveur
    if (connect(sock_client, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Erreur de connexion");
        close(sock_client);
        exit(1);
    }

    printf("Connecté au serveur.\n");

    //  ENVOI DU PSEUDO À LA CONNEXION
    printf("Entrez votre pseudo : ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Supprimer le retour à la ligne
    send(sock_client, buffer, strlen(buffer), 0);

    // 4. Boucle principale
    while (1) {
        // Réception du message du serveur (ex: bienvenue, réponse, classement...)
        int n = recv(sock_client, buffer, BUFFER_SIZE - 1, 0);
        if (n <= 0) {
            printf(" Serveur déconnecté.\n");
            break;
        }

        buffer[n] = '\0';
        printf(" Serveur : %s\n", buffer);

        // Si le serveur indique que la partie est terminée, sortir
        if (strstr(buffer, "Un joueur a déjà gagné") != NULL ||
            strstr(buffer, "Au revoir") != NULL) {
            break;
        }

        // Demander la réponse du joueur
        printf("Votre réponse :  /vous pouvez aussi tapez 'exit' pour quitter le jeu / 'oui' pour continuer  / 'non' pour arrêter) : ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Supprimer le retour à la ligne

        // Si l'utilisateur écrit 'exit', sortir de la boucle
        if (strcmp(buffer, "exit") == 0) {
            send(sock_client, buffer, strlen(buffer), 0);
            printf("👋 Déconnexion demandée.\n");
            break;
        }

        // Envoyer la commande ou la proposition au serveur
        send(sock_client, buffer, strlen(buffer), 0);
    }

    // 5. Fermeture
    close(sock_client);
    printf("Connexion fermée.\n");
    return 0;
}
 