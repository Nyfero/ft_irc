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


    public:

        /***************************/
        /******* CONSTRUCTOR *******/
        /***************************/
        channel(std::string name, user *creator);

        /**************************/
        /******* DESTRUCTOR *******/
        /**************************/
        ~channel();

        /**************/
        /****  MOD ****/
        /**************/

        // int Add_user(user use);
        // int Remove_user(user use);

        /************************/
        /******* ACCESSOR *******/
        /************************/

        const std::string       Get_channel_name() const;
        std::vector<user *>     Get_list_channel_user() const;
        std::vector<user *>     Get_list_operator() const;

};
