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

        void    Check_command(user *user, std::string str);

        void    Pass_cmd(user *user, std::string cmd);
        void    User_cmd(user *user, std::string cmd);
        void    Nick_cmd(user *user, std::string cmd);
        void    Mode_cmd(user *user, std::string cmd);
        void    Quit_cmd(user *user, std::string cmd);
        void    Join_cmd(user *user, std::string cmd);
        void    Part_cmd(user *user, std::string cmd);
        void    Names_cmd(user *user, std::string cmd);
        void    Invite_cmd(user *user, std::string cmd);
        void    Kick_cmd(user *user, std::string cmd);
        void    Privmsg_cmd(user *user, std::string cmd);
        void    Notice_cmd(user *user, std::string cmd);
        void    Away_cmd(user *user, std::string cmd);
        void    Users_cmd(user *user, std::string cmd);
        void    Wallops_cmd(user *user, std::string cmd);
};