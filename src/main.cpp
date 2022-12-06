/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 06:59:27 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/06 10:51:08 by jgourlin         ###   ########.fr       */
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
    int             socket_0;

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
    if ((socket_0 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        std::cout << "erreur creation socket" << std::endl;
    else
        std::cout << "Socket creer num: " << socket_0 << std::endl;

// Option Socket

    // if (setsockopt(socket_0, int, int, const void*, socklen_t))
    // {
    //     std:: cout << "setsockopt: Erreur" << std::endl;
    //     close(socket_0);
    //     return (0);
    // }
    // else
    //     std:: cout << "setsockopt: OK" << std::endl;

// Bind

    // struct sockaddr   addr_0;

    // addr_0.sa_family = AF_INET;


    if (bind(socket_0, (struct sockaddr *)server_addr_0, sizeof(*server_addr_0)))
    {
        std::cout << "bind: Erreur" << std::endl;
        std::cout << "errno = " << errno << std::endl;
        std::cout << strerror(errno) << std::endl;
        
        close(socket_0);
        freeaddrinfo(res_addrinfo);
        return (0);
    }
    else
        std:: cout << "bind: OK" << std::endl;


// listen

    if (ft_listen(socket_0, 2))
    {
        close(socket_0);
        freeaddrinfo(res_addrinfo);
        return (0);
    }

// Passage en mode non bloquant
   // fcntl(socket_0, F_SETFL, O_NONBLOCK);

// accept

    struct sockaddr addr_cli;
    unsigned int addrLen = sizeof(addr_cli);

    if ((socket_cli = accept(socket_0, &addr_cli, &addrLen)) == -1)
    {
        if (errno == 11)
            std:: cout << "accept: La socket est non bloquante et aucune connexion n'est présente dans la file." << std::endl;
        else
            std:: cout << "accept: Erreur" << std::endl << "errno = " << errno << std::endl;
    }
    else
        std:: cout << "accept: OK" << std::endl;

// Fermeture socket
    if (close(socket_0) == -1)
        std::cout << "erreur fermeture" << std::endl;
    else
        std::cout << "Socket close" << std::endl;
    freeaddrinfo(res_addrinfo);

    return (0);
}