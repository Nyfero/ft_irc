# pragma once

# include "utils.hpp"

class user
{
    private:

        // Nom d'utilisateur (identifiant -> ne change pas)
        std::string   _username;

        // Nom reel du client
        std::string         _realname;

        // Pseudo d'utilisateur
        std::string         _nickname;

        // Hostname du client
        std::string         _hostname;

        // Mode du client
        Mode            _mode;

        // Channels du client
        std::vector<channel *> _channel_register;

        // Si le client est enregistre sur le serveur
        bool                _is_register;

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

        int Get_fd_client() const;
        bool Get_is_register() const;
        std::string Get_username() const;
        std::string Get_nickname() const;
        std::string Get_hostname() const;
        Mode    Get_mode() const;
        std::string Get_realname() const;
        std::vector<channel *>  Get_channel_register() const;

        /**************/
        /****  MOD ****/
        /**************/

        int Add_channel(channel *chan);
        int Remove_Channel(channel *chan);
        void Remove_all_channel();
        void Set_is_register(bool modif);
        void Set_nickname(std::string nickname);
        void Set_username(std::string username);
        void Set_hostname(std::string hostname);
        void Set_mode(std::string mode);
        void Set_realname(std::string realname);

        void    Print_channel_user();
};
