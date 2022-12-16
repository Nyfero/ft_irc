# pragma once

# include "utils.hpp"

class user
{
    private:
        // int _socket_cli;
        struct addrinfo _addrinfo_cli;
        struct pollfd   _fd_poll; // peut etre enlever et juste garder socket

    public:
        std::string str;

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
