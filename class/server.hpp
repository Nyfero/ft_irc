/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:02:46 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/09 10:34:40 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector>
# include "user.hpp"

# define MAX_LISTEN 42

class server
{

    private:
        /*** PRIVATE MEMBER TYPE ***/
        struct addrinfo *_addrinfo;
        struct addrinfo _hints; // normalement const struct addrinfo
        struct sockaddr_in  *_serv_addr;

        char    *_password;
        const char  *_port;
        const char  *_node;

        int _socket_serv;
        //unsigned int    _nb_client; (size vector)
        // liste user -> vector
        std::vector<user*>   _user;

        /****  PRIVATE MEMBER FUNCTION ****/
        int    _Init_server();
        void    _Infinit_while();

        void    _Add_user();
        void    _Remove_user(std::vector<user*>::iterator pos);


    public:

        /*********************/
        /**** CONSTRUCTOR ****/
        /*********************/

        server(); // non utilisee
        server(char *port, char *password);
        // cpy
        
        /********************/
        /**** DESTRUCTOR ****/
        /********************/
        ~server();

        /********************/
        /***** OPERATOR *****/
        /********************/
        // operator =
};