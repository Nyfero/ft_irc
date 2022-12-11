/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:02:46 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/11 18:10:32 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector>
# include "user.hpp"

# define MAX_LISTEN 42

class server
{

    private:
        /***************************/
        /*** PRIVATE MEMBER TYPE ***/
        /***************************/
        struct addrinfo *_addrinfo;
        struct addrinfo _hints; // normalement const struct addrinfo
        struct pollfd   _fdpf;
        

        char    *_password;
        const char  *_port;
        const char  *_node;

        int _socket_serv;
        //unsigned int    _nb_client; (size vector)
        // liste user -> vector
        std::vector<user*>   _user;

        std::vector<pollfd>   _fds;

        // faire tab pollfd -> vector ?
        // tab user -> map ?
        // les 2 fd sont commun et uniques

        /**********************************/
        /****  PRIVATE MEMBER FUNCTION ****/
        /**********************************/
        int    _Init_server();
        void    _Infinit_while();

        /* MOD USER */
        void    _Add_user();
        void    _Remove_user(std::vector<user*>::iterator pos);
        void    _Remove_user(std::vector<pollfd>::iterator pos);

        /* DVERS */  
        int     _Input_cli(int fd);
        int     _Output_cli(int fd, std::string msg);

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