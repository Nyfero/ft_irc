#pragma once

#include "utils.hpp"

class Mode
{
    private:

    /***********************************/
    /*****   PRIVATE MEMBER TYPE   *****/
    /***********************************/

        bool    _away;
        bool    _invisible;
        bool    _wallops;
        bool    _restricted;
        bool    _operator;
        bool    _local_operator;
        bool    _server_notice;

        std::string _away_reply;

    public:

    /***********************************/
    /*******   CONSTRUCTOR   ***********/
    /***********************************/

        Mode();


    /***********************************/
    /*******   DESTRUCTOR   ************/
    /***********************************/
    
        ~Mode();


    /***********************************/
    /*******   ACCESSOR   **************/
    /***********************************/

        bool    Get_away() const;
        bool    Get_invisible() const;
        bool    Get_wallops() const;
        bool    Get_restricted() const;
        bool    Get_operator() const;
        bool    Get_local_operator() const;
        bool    Get_server_notice() const;

        std::string Get_away_reply() const;


    /***********************************/
    /*******   MODIFIER   **************/
    /***********************************/

        void    Set_away(bool away);
        void    Set_invisible(bool invisible);
        void    Set_wallops(bool wallops);
        void    Set_restricted(bool restricted);
        void    Set_operator(bool op);
        void    Set_local_operator(bool local_op);
        void    Set_server_notice(bool server_notice);

        void    Set_away_reply(std::string away_reply);


    /***********************************/
    /*******   FUNCTIONS   *************/
    /***********************************/

        std::string Print_mode() const;
};

