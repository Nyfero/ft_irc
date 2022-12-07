/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 06:59:27 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/07 12:35:24 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../class/utils.hpp"

int ft_listen(int socket, int argv2)
{
    int res = listen(socket, argv2);

    if (res)
        std:: cout << "listen: Erreur" << std::endl;
    else
        std:: cout << "listen: OK" << std::endl;
    return (res);
}

int main(int argc, char **argv)
{
    int             socket_serv;

    (void)argc;
    (void)argv;
    int socket_cli;
    if (argc != 2)
    {
        std::cout << "need 1 argument" << std::endl;
        return (0);
    }

// Define with getaddrinfo

    struct addrinfo *res_addrinfo;
    struct addrinfo hints;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    if (getaddrinfo("127.0.0.1", argv[1], &hints, &res_addrinfo) != 0)
    {
        std::cout << "getaddrinfo = Erreur" << std::endl;
        std::cout << "errno = " << errno << std::endl;
        std::cout << gai_strerror(errno) << std::endl;
        return (0);
    }
    else
        std::cout << "getaddrinfo = OK" << std::endl;

    struct sockaddr_in *server_addr_0 = (struct sockaddr_in *)res_addrinfo->ai_addr;
    std::cout << "server ip = " << inet_ntoa(server_addr_0->sin_addr) << std::endl;

// Creation socket
    if ((socket_serv = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        std::cout << "erreur creation socket" << std::endl;
    else
        std::cout << "Socket creer num: " << socket_serv << std::endl;

// Option Socket

    // if (setsockopt(socket_serv, int, int, const void*, socklen_t))
    // {
    //     std:: cout << "setsockopt: Erreur" << std::endl;
    //     close(socket_serv);
    //     return (0);
    // }
    // else
    //     std:: cout << "setsockopt: OK" << std::endl;

// Bind

    // struct sockaddr   addr_0;

    // addr_0.sa_family = AF_INET;


    if (bind(socket_serv, (struct sockaddr *)server_addr_0, sizeof(*server_addr_0)))
    {
        std::cout << "bind: Erreur" << std::endl;
        std::cout << "errno = " << errno << std::endl;
        std::cout << strerror(errno) << std::endl;
        
        close(socket_serv);
        freeaddrinfo(res_addrinfo);
        return (0);
    }
    else
        std:: cout << "bind: OK" << std::endl;


// listen

    if (ft_listen(socket_serv, 2))
    {
        close(socket_serv);
        freeaddrinfo(res_addrinfo);
        return (0);
    }

// Passage en mode non bloquant et seul utilisation autorise par le sujet
   // fcntl(socket_serv, F_SETFL, O_NONBLOCK);

// accept
// Permet de recuperer le socket du client, c'est par ce dernier qu'on fera les recv/send || read/write
// il remplit aussi une struct en 2eme arg

    struct sockaddr addr_cli;
    unsigned int addrLen = sizeof(addr_cli);

    if ((socket_cli = accept(socket_serv, &addr_cli, &addrLen)) == -1)
    {
        if (errno == 11)
            std:: cout << "accept: La socket est non bloquante et aucune connexion n'est présente dans la file." << std::endl;
        else
            std:: cout << "accept: Erreur" << std::endl << "errno = " << errno << std::endl;
    }
    else
    {
        char ip[INET_ADDRSTRLEN];
	    inet_ntop(AF_INET, &addr_cli, ip, INET_ADDRSTRLEN);
        std::cout << "Connexion de " << ip << std::endl;

        std:: cout << "accept: OK" << std::endl;
        std::cout << "socket_clit :" << socket_cli << std::endl << "socket_serveur :" << socket_serv << std::endl;

// poll
// attend un changement d'état des descripteurs contenus
// en theorie il vaut mieux attendre un changement d'etat c qui signigie qu'on recoit une demande poru ensuite y repondre
// quand le client se connecte la socket contient son nom

// attente de l'un des événements suivants : saisie au clavier, connexion d'un client, envoi d'un message par un client.

// Si un client se connecte, nous acceptons la connexion et nous attendons la réception de son pseudo, 
// une fois reçu, nous créons un objet de type Client (voir source),
// contenant le socket pour discuter avec lui ainsi que son pseudo.
// Nous ajoutons maintenant notre client à notre tableau de clients.

// Si un client envoie un message, nous le diffusons à tous les autres clients.
// Lorsqu'un client se déconnecte (Ctrl+C), recv retourne 0,
// nous gérons donc ce cas et donc nous supprimons notre client de notre tableau tout en notifiant aux autres 
// clients sa déconnexion.

        struct pollfd   fd_poll;
        fd_poll.fd = socket_serv;
        fd_poll.revents = 0;
        fd_poll.events = POLLIN;

        int tmp;
        char recu[100];
        int cond = 1;
        while (cond)
        {
            tmp = poll(&fd_poll, socket_cli, -1);
            if (POLLHUP)
            {
                cond = 0;
                std::cout << "client deconnecter" << std::endl;
            }
            else if (POLLIN)
            {
                recv(socket_cli, recu, 99, 0);
                std::cout << recu << std::endl;
                send(socket_cli, "Merci du message\n\r", sizeof("Merci du message\n\r"), 0);
            }
            // std::cout << "tmp: " << tmp << std::endl;

            // ssize_t size_recv = 1;
            // while (size_recv > 0)
            // {
            //     size_recv = recv(socket_cli, recu, 1, 0);
            //     std::cout <<recu;
            //     // std::cout << size_recv;
            // }
            // std::cout << std::endl;

        }

// send est recu a la fin du serveur
        send(socket_cli, "aurevoir\r\n", sizeof("aurevoir\r\n"), 0);

    }

// Fermeture socket
    if (close(socket_serv) == -1)
        std::cout << "erreur fermeture" << std::endl;
    else
        std::cout << "Socket close" << std::endl;
    freeaddrinfo(res_addrinfo);

    return (0);
}