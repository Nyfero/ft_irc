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

        std::string Get_away_reply() const;


    /***********************************/
    /*******   MODIFIER   **************/
    /***********************************/

        int     Add_mode(char m);
        int     Oper_add_mode(char m);
        int     Remove_mode(char m);
        int     Oper_remove_mode(char m);

        void    Set_away(bool away);
        void    Set_invisible(bool invisible);
        void    Set_wallops(bool wallops);
        void    Set_restricted(bool restricted);
        void    Set_operator(bool op);

        void    Set_away_reply(std::string away_reply);


    /***********************************/
    /*******   FUNCTIONS   *************/
    /***********************************/

        std::string Print_mode() const;
};

