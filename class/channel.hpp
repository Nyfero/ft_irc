# pragma once

# include "utils.hpp"

class user;

class channel
{
    private:

        /***************************/
        /*** PRIVATE MEMBER TYPE ***/
        /***************************/

        // Nom du channel
        const std::string   _channel_name;
        // Liste des utilisateurs du channel
        std::vector<user *> _list_channel_user;
        // Liste des utilisateurs invited
        std::vector<user *> _list_invited_user;
        // Liste des operateurs du channel
        std::vector<user *> _list_operator;
        // Key channel
        std::string _key;

        // Channel's topic
        std::string _topic;

        // Channel's mode
        bool    _invite_only;
        bool    _topic_settable;
        bool    _channel_private;

    public:

        /***************************/
        /******* CONSTRUCTOR *******/
        /***************************/
        channel(std::string name, user *creator);
        channel(std::string name, user *creator, std::string key);

        /**************************/
        /******* DESTRUCTOR *******/
        /**************************/
        ~channel();

        /**************/
        /****  MOD ****/
        /**************/

        int     Add_user(user *use);
        int     Remove_user(user *use);

        void    Add_oper(user *sender, user *use);

        void    Add_invited_user(user *use);
        void    Remove_invited_user(user *use);
        
        void    Mod_topic(std::string str);
        void    Set_channel_key(std::string key);
        void    Set_invite_only(bool b);
        void    Set_topic_settable(bool b);
        void    Set_channel_private(bool b);

        /************************/
        /******* ACCESSOR *******/
        /************************/

        const std::string       Get_channel_name() const;
        const std::string       Get_channel_key() const;
        const std::string       Get_channel_topic() const;
        std::vector<user *>     Get_list_channel_user() const;
        std::vector<user *>     Get_list_operator() const;
        bool                    Get_invite_only() const;
        bool                    Get_topic_settable() const;
        bool                    Get_channel_private() const;
        std::string             Print_mode() const;
        std::vector<user *>     Get_invited_user() const;
        
        bool                    _Is_invit(user *use) const;

        void    print_user_channel();
        void    print_oper_channel();

};
