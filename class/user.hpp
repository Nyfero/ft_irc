/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:01:47 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/09 14:02:38 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "utils.hpp"

class user
{
    private:
        // int _socket_cli;
        struct addrinfo _addrinfo_cli;
        struct pollfd   _fd_poll; // peut etre enlever et juste garder socket

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

        /********************/
        /***** ACCESSOR *****/
        /********************/

        int Get_fd();

};
