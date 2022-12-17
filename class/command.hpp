#pragma once

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "utils.hpp"

class user;
class channel;

class command {

    private:
        /***************************/
        /*** PRIVATE MEMBER TYPE ***/
        /***************************/

        std::string arg;

    public:
        void    Enter(user* user, std::string str);
        void    join(user *user, std::string chan, std::string key);
        void    join(user *user, std::string chan);
        void    join(user *user);


};



#endif