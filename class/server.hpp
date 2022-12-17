#pragma once

# include "utils.hpp"

# define MAX_LISTEN 2
# define SIZE_INPT 50

class server
{

    private:

        /***************************/
        /*** PRIVATE MEMBER TYPE ***/
        /***************************/

        // Structure des informations du serveur
        struct addrinfo *_addrinfo;
        struct addrinfo _hints;

        // Structure des requetes de connexion des clients
        struct pollfd   _serv_poll_fd;
        
        // Mot de passe du serveur
        const std::string _password;
        // Port du serveur
        const char  *_port;
        // Adresse reseau du serveur
        const char  *_node;
        // Socket du serveur
        int _socket_serv;

        // Liste des clients connectes au serveur
        std::vector<user*>      _list_user;
        // Liste des channels du serveur
        std::vector<channel*>   _list_channel;
        // Liste des pollfd du serveur
        std::vector<pollfd>     _list_poll_fd;



        /**********************************/
        /****  PRIVATE MEMBER FUNCTION ****/
        /**********************************/

        int    _Init_server();
        void   _Launch_server();

        /* MOD USER */
        void    _Add_user();
        void    _Remove_user(std::vector<user*>::iterator pos);
        void    _Remove_user(std::vector<pollfd>::iterator pos);


        /* GET USER */
        user   *_Get_user_by_fd(int fd);
        
        
        /* MOD CHANNEL */
        void    _Add_channel(std::string name, user *creator);
        void    _Remove_channel(channel *chan);


        /* DVERS */
        int     _Input_client(std::vector<pollfd>::iterator it);
        int     _Output_client(int fd, std::string msg);

    public:

        /*********************/
        /**** CONSTRUCTOR ****/
        /*********************/

        server(char *port, char *password);
        


        /********************/
        /**** DESTRUCTOR ****/
        /********************/

        ~server();


        /***********************/
        /*****   COMMAND   *****/
        /***********************/

        void    Enter(user* user, std::string str);
        void    join(user *user, std::string chan, std::string key);
        void    join(user *user, std::string chan);
        void    join(user *user);
        int    pass(user *user, std::string pass);
        void    nick(user *user, std::string nick);

};