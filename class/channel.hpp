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
        // Liste des operateurs du channel
        std::vector<user *> _list_operator;
        // Key channel
        std::string _key;



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

        int Add_user(user *use);
        int Remove_user(user *use);

        void Add_oper(user *use);

        /************************/
        /******* ACCESSOR *******/
        /************************/

        const std::string       Get_channel_name() const;
        const std::string       Get_channel_key() const;
        std::vector<user *>     Get_list_channel_user() const;
        std::vector<user *>     Get_list_operator() const;
        const std::string       Get_channel_key() const;

        void    print_user_channel();
        void    print_oper_channel();

};
