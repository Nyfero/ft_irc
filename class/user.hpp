# pragma once

# include "utils.hpp"

class channel;

class user
{
    private:

        // Nom d'utilisateur (identifiant -> ne change pas)
        const std::string   _username;
        // Pseudo d'utilisateur
        std::string         _nickname;

        // Channels du client
        std::vector<channel *> _channel_register;

        // Structure des requetes du client et son fd
        struct pollfd   _fd_poll_client;
        
        // (optionnel) Adresse IP client
        struct addrinfo _addrinfo_client;

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

        int Get_fd_client();

        /**************/
        /****  MOD ****/
        /**************/

        int Add_channel(channel *chan);
        int Remove_Channel(channel *chan);

};
