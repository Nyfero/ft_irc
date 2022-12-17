# pragma once

# include "utils.hpp"

class user;

class channel
{
    private:
        std::string         _name;
        std::vector<user *> _user;
        std::vector<user *> _op;
    public:

        /***************************/
        /******* CONSTRUCTOR *******/
        /***************************/
        channel(std::string name, user *creator);
        channel();
        // cpy

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

        std::string         Get_name();
        std::vector<user *> Get_user();
        std::vector<user *> Get_op();

};
