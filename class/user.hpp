/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:01:47 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/09 09:44:06 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "utils.hpp"

class user
{
    private:
        // int _socket_cli;
        struct addrinfo _addrinfo_cli;
        struct pollfd   _fd_poll;
        //struct sockaddr_in  *_cli_addr;

    public:

        /*********************/
        /**** CONSTRUCTOR ****/
        /*********************/
        user(); // non utilisee
        user(int socket, addrinfo info);
        // cpy

        /********************/
        /**** DESTRUCTOR ****/
        /********************/

        ~user();

        /********************/
        /***** OPERATOR *****/
        /********************/
        // operator=

};
