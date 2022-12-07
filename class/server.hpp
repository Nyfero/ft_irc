/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:02:46 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/07 16:32:31 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define MAX_LISTEN = 42

class server
{

    private:
        struct addrinfo *_addrinfo;
        struct addrinfo _hints; // normalement const struct addrinfo

        struct sockaddr_in  *_serv_addr;

        char    *_password;
        const char  *_port;
        const char  *_node;

        int _socket_serv;
        
        unsigned int    _nb_client;
        // liste user
        // liste channel


        int    _Init_server();
        void    _Infinit_while();

    public:
        server(char *port, char *password);
        ~server();
};