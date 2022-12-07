/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:02:46 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/07 12:20:31 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class server
{

    private:
        struct addrinfo *_addrinfo;
        const struct addrinfo _hints;

        struct sockaddr_in  *_serv_addr;

        char    *_password;
        const char  *_port;
        const char  *_node;

        int _socket_serv;
        // liste user
        // liste channel

    public:
        server(char *port, char *password);
        ~server();
};