# pragma once

# include "utils.hpp"

class channel;

class user
{
    private:
        // int _socket_cli;
        struct addrinfo _addrinfo_cli;
        struct pollfd   _fd_poll; // peut etre enlever et juste garder socket
        
        // nom channel auxquels user appartient -> check avec vector<channel> de user
        std::vector<channel *> _channel;
        
        std::string _nickname;
        std::string _username;

    public:
        std::string str;

        // channel _channel;

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

        /**************/
        /****  MOD ****/
        /**************/

        int Add_channel(channel *chan);
        int Remove_Channel(channel *chan);

};
