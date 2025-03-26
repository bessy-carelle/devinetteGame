#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int guess;

    // Création du socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convertir l'adresse IP en format réseau
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Adresse invalide");
        exit(EXIT_FAILURE);
    }

    // Connexion au serveur
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Échec de connexion");
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur. Essayez de deviner le nombre entre 0 et 100.\n");

    while (1) {
        // Demander à l'utilisateur de faire une supposition
        printf("Votre supposition : ");
        scanf("%d", &guess);

        // Envoyer la supposition au serveur (en tant qu'entier)
        if (send(sock, &guess, sizeof(guess), 0) == -1) {
            perror("Erreur en envoyant la supposition");
            break;
        }

        // Recevoir la réponse du serveur
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Connexion au serveur perdue.\n");
            break;
        }

        printf("Réponse du serveur : %s\n", buffer);

        // Vérifier si le jeu est terminé (le client a gagné ou la partie est terminée)
        if (strcmp(buffer, "Bravo, vous avez gagné !") == 0 || strcmp(buffer, "Un client a deviné le bon nombre, il a gagné !\n") == 0) {
            break;
        }
    }

    close(sock); // Fermer le socket client
    return 0;
}
