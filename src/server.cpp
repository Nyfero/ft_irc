/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 11:57:21 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/09 10:39:53 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../class/utils.hpp"
# include "../class/server.hpp"

/*************************/
/*******CONSTRUCTOR*******/
/*************************/

server::server(){}

server::server(char *port, char *password):
_password(password),
_port(port),
_node("127.0.0.1")
{
    std::cout << "*** server Constructor ***" << std::endl;
    _hints.ai_family = AF_INET;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_protocol = 0;
    (void)_password;
    _Init_server();
    //socket creer et server en ecoute
    
    // peut etre creer un socket vide en plus et si client vient recreer socket vide en + etc 
    _Infinit_while();
}

/**************************/
/******DESCONSTRUCTOR******/
/**************************/

server::~server()
{
    std::cout << "DESTRUCTOR SERVER" << std::endl;
    close(_socket_serv);
    freeaddrinfo(_addrinfo);
}

/*************************/
/********INIT_SERV********/
/*************************/

int    server::_Init_server()
{
    std::cout << "*** _Init_server ***" << std::endl;
    if (getaddrinfo(_node, _port, &_hints, &_addrinfo))
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
    return (0);
}

/***************************/
/*******INFINIT_WHILE*******/
/***************************/

void    server::_Infinit_while()
{
    std::cout << "*** Infinit_while ***" << std::endl;
    // Gerer l'ajout d'USER
    // Gerer les commandes des USERS
    
    fcntl(_socket_serv, F_SETFL, O_NONBLOCK); // pour qu'accept ne soit pas bloquant

    int tmp = 0;
    while (tmp < 8)
    {
        // poll(fd_poll, _socket_serv + 2, -1);
        _Add_user();
        // if (fd_poll[0].revents & POLLIN)
        // {
        //     std::cout << "ALPHA" << std::endl;
            
        // }
        sleep(1);
        tmp++;
    }
    std::vector<user*>::iterator it;
    for (it = _user.begin(); it < _user.end(); it++)
        _Remove_user(it);

}

/******************/
/**** MOD USER ****/
/******************/

void    server::_Add_user()
{
    std::cout << "*** _Add_User ***" << std::endl;

    int socket_cli;
    addrinfo    addrinfo_cli;

    std::cout << "elemt _user:" << _user.size() << std::endl;
    socket_cli = accept(_socket_serv, addrinfo_cli.ai_addr, &addrinfo_cli.ai_addrlen);
    if (socket_cli < 0)
    {
        if (errno == 11)
            std::cout << "Non bloquant no new connect" << std::endl;
        else
        {
            std::cout << EBADF << std::endl;
            std::cout << "Error Accept in _Add_user:" << errno << std::endl;
        }
    }
    else
    {
        //user *tmp = new user(socket_cli, addrinfo_cli);
        //_user.push_back(new user(socket_cli, addrinfo_cli));
        _user.push_back(new user(socket_cli, addrinfo_cli));
        std::cout << "New User on socket: " << socket_cli << std::endl;
    }
}

void    server::_Remove_user(std::vector<user*>::iterator pos)
{
    user *tmp = _user.at(std::distance(_user.begin(), pos));

    std::cout << "*** _Remove_User ***" << std::endl;
    _user.erase(pos);
    delete tmp;

}