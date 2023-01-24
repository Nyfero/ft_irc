
#pragma once

# include "utils.hpp"

# define MAX_LISTEN 42
# define SIZE_INPT 50

class user;
class channel;

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
        
        // Name serveur
        const std::string   _name_serveur;
        // Mot de passe du serveur
        const std::string _password;
        // Mot de passe de l'admin du serveur
        const std::string _admin_password;
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
        user    *_Get_user_by_nick(std::string str);
        
        
        /* MOD CHANNEL */
        channel *_Add_channel(std::string name, user *creator);
        channel *_Add_channel(std::string name, user *creator, std::string key);
        void    _Remove_channel(channel *chan);


        /* DVERS */
        int     _Input_client(std::vector<pollfd>::iterator it);
        int     _Output_client(int fd, std::string msg);
        int     _Output_channel(channel *chan, std::string msg);
        int     _Output_all_user_channel(user *user, std::string msg);
        void    _Print_channel();
        void    _Print_user();
        bool    _User_is_in_chan(user *use, channel *chan);

        /* GET CHANNEL */
        channel     *_Channel_already_exist(std::string str);

        /* COMMAND*/
                /* Join */
        int                      _Join_treat(user *user, std::vector<std::string> chan, std::vector<std::string> key);
        int                      _Join_treat(user *user, std::vector<std::string> chan);
        void                     _Join_rpl(user *user, channel *chan);

        bool                     _parse_privmsg(user *sender, struct s_IRCMessage cmd);
        std::vector<std::string> _target_handle(struct s_IRCMessage cmd);
        std::vector<int>         _targetfds_creator_privmsg(user *sender, std::vector<std::string> target);
        bool                     _add_channel_targetfds_privmsg(user *sender, std::vector<int> *targets_fds, std::string target);
        bool                     _add_user_targetfds_privmsg(user *sender, std::vector<int> *targets_fds, std::string target);
        std::string              _create_msg(struct s_IRCMessage cmd);
                    /* Notice */
        bool                     _parse_notice(struct s_IRCMessage cmd);
        std::vector<int>         _targetfds_creator_notice(user *sender, std::vector<std::string> target);
        bool	                 _add_channel_targetfds_notice(user *sender, std::vector<int> *targets_fds, std::string target);
        bool	                 _add_user_targetfds_notice(std::vector<int> *targets_fds, std::string target);
                    /* Wallops */
        bool                     _parse_wallops(struct s_IRCMessage cmd);
        std::vector<int>         _targetfds_creator_wallops(void);
        bool	                 _add_channel_targetfds_wallops(user *sender, std::vector<int> *targets_fds, std::string target);
                    /* Invite */
        bool	                 _parse_invite(user *sender, struct s_IRCMessage cmd);
        user	                 *_check_nick_invite(user *sender, struct s_IRCMessage cmd);
        channel	                 *_check_chan_invite(user *sender, struct s_IRCMessage cmd);
        bool                     _user_already_member(user *target_nick, channel *target_chan);
        void                     _invite_success(user *sender, user *target_nick, channel *target_chan, s_IRCMessage cmd);
                    /* Kick */
        bool	                 _parse_kick(user *sender, struct s_IRCMessage cmd);
        user                     *_check_nick_kick(user *sender, struct s_IRCMessage cmd);
        std::vector<channel *>   _split_channels_kick(user *sender, struct s_IRCMessage cmd);
        channel                  *_chan_kick_creator(user *sender, std::string chan_to_kick);
        channel                  *_user_not_in_channel(user *sender, user *target_nick, channel *channel_kick);
        bool                     _kick_from_channel(user *target_nick, channel *channel_kick);
        void                     _kick_success_message(user *target_nick, channel *channel_kick, struct s_IRCMessage cmd);

                    /* List */
        std::vector<std::string> _split_channel_list(std::string target);
        channel *                _filter_channel_list(std::string target_splited);
        void                     _list_display(user *sender, std::vector<channel *> channel_targeted);

        
                    /* BOT */
        void                     _Bot_main(user *sender, struct s_IRCMessage cmd);
       
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

        int    Check_command(user *user, std::string str);
        bool    Check_prefix(user *user, std::string str);

        void    Pass_cmd(user *user, struct s_IRCMessage cmd);
        void    Nick_cmd(user *user, struct s_IRCMessage cmd);
        void    User_cmd(user *user, struct s_IRCMessage cmd);
        void    Oper_cmd(user *user, struct s_IRCMessage cmd);
        void    Mode_cmd(user *user, struct s_IRCMessage cmd);
        void    Join_cmd(user *user, struct s_IRCMessage cmd);
        void    Part_cmd(user *user, struct s_IRCMessage cmd);
        void    Topic_cmd(user *user, struct s_IRCMessage cmd);
        int     Quit_cmd(user *user, struct s_IRCMessage cmd);
        void    List_cmd(user *user, struct s_IRCMessage cmd);
        void    Names_cmd(user *user, struct s_IRCMessage cmd);
        void    Invite_cmd(user *user, struct s_IRCMessage cmd);
        void    Kick_cmd(user *user, struct s_IRCMessage cmd);
        void    Privmsg_cmd(user *user, struct s_IRCMessage cmd);
        void    Notice_cmd(user *user, struct s_IRCMessage cmd);
        void    Away_cmd(user *user, struct s_IRCMessage cmd);
        void    Users_cmd(user *user, struct s_IRCMessage cmd);
        void    Wallops_cmd(user *user, struct s_IRCMessage cmd);
        void    Pong_cmd(user *user, struct s_IRCMessage cmd);
};