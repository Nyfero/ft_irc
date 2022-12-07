/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 11:57:21 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/07 16:40:27 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../class/utils.hpp"
# include "../class/server.hpp"

/*************************/
/*******CONSTRUCTOR*******/
/*************************/

server::server(char *port, char *password): _port(port),
_password(password),
_node("127.0.0.1"),
_nb_client(0)
{
    _hints.ai_family = AF_INET;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_protocol = 0;

    _Init_server();
    //socket creer et server en ecoute
    
    // peut etre creer un socket vide en plus et si client vient recreer socket vide en + etc 
    _Infinit_while();
}

/**************************/
/******DESCONSTRUCTOR******/
/**************************/

server::~server()
{}

/*************************/
/********INIT_SERV********/
/*************************/

int    server::_Init_server()
{
    if (!getaddrinfo(_node, _port, &_hints, &_addrinfo))
    {
        std::cout << "Error: getaddrinfo" << std::endl;
        return (-1);
    }
    _serv_addr = (struct sockaddr_in *)_addrinfo->ai_addr;

    // SOCKET
    if ((_socket_serv = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "Error: Creation socket" << std::endl;
        freeaddrinfo(_addrinfo);
        return (-2);
    }
    // SOCKET OPTION
    // BIND
    if (bind(_socket_serv, (struct sockaddr *)_serv_addr, sizeof(*_serv_addr)))
    {
        std::cout << "Error: Creation socket" << std::endl;
        freeaddrinfo(_addrinfo);
        close (_socket_serv);
        return (-3);
    }
    // LISTEN
    if (listen(_socket_serv, MAX_LISTEN))
    {
        close(_socket_serv);
        freeaddrinfo(_addrinfo);
        return (-4);
    }
}

/*************************/
/*******INFIN_WHILE*******/
/*************************/

void    server::_Infinit_while()
{
    // Gerer l'ajout d'USER
    // Gerer les commandes des USERS
    fcntl(_socket_serv, F_SETFL, O_NONBLOCK); // pour qu'accept ne soit pas bloquant
    
}