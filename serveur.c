#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_message(int sock, const char *message) {
    send(sock, message, strlen(message), 0);
}

void handle_client_guess(int client_sock) {
    char buffer[BUFFER_SIZE];
    int number_to_guess = 42;  // Exemple de numéro à deviner
    char welcome_message[] = "Bienvenue, vous devez deviner le numéro : 42";

    // Envoi du message de bienvenue
    send_message(client_sock, welcome_message);

    // Demander à l'utilisateur de taper sa réponse
    send_message(client_sock, "Entrez votre réponse : ");

    // Recevoir la réponse du client
    recv(client_sock, buffer, sizeof(buffer) - 1, 0);
    int guess = atoi(buffer);

    if (guess == number_to_guess) {
        send_message(client_sock, "Bravo, vous avez trouvé le bon numéro !");
    } else {
        send_message(client_sock, "Désolé, ce n'est pas le bon numéro.");
    }
}

int main() {
    int sock_serveur, sock_client;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    // Création du socket
    sock_serveur = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_serveur == -1) {
        perror("Échec de la création du socket");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Liaison du socket
    if (bind(sock_serveur, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Échec de la liaison du socket");
        return 1;
    }

    // Écoute des connexions entrantes
    if (listen(sock_serveur, 10) == -1) {
        perror("Échec de l'écoute des connexions");
        return 1;
    }

    printf("Serveur en écoute sur le port %d...\n", PORT);

    while (1) {
        // Acceptation des connexions entrantes
        sock_client = accept(sock_serveur, (struct sockaddr *)&cli_addr, &cli_len);
        if (sock_client == -1) {
            perror("Échec de l'acceptation de la connexion");
            continue;
        }

        printf("Un client s'est connecté\n");

        // Gérer le jeu avec le client
        handle_client_guess(sock_client);

        // Fermer la connexion avec le client après la session
        close(sock_client);
    }

    // Fermer le socket du serveur
    close(sock_serveur);
    return 0;
}
