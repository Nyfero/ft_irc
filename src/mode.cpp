#include "../class/utils.hpp"

/*********************/
/**** CONSTRUCTOR ****/
/*********************/

Mode::Mode() {
    _away = false;
    _invisible = false;
    _wallops = false;
    _restricted = false;
    _operator = false;
};


/********************/
/**** DESTRUCTOR ****/
/********************/

Mode::~Mode() {
};


/********************/
/***** ACCESSOR *****/
/********************/

bool    Mode::Get_away() const {
    return _away;
};

bool    Mode::Get_invisible() const {
    return _invisible;
};

bool    Mode::Get_wallops() const {
    return _wallops;
};

bool    Mode::Get_restricted() const {
    return _restricted;
};

bool    Mode::Get_operator() const {
    return _operator;
};

std::string Mode::Get_away_reply() const {
    return _away_reply;
};

/********************/
/***** MODIFIER *****/
/********************/

int     Mode::Add_mode(char m) {
    if (m == 'i') {
        Set_invisible(true);
    }
    else if (m == 'w') {
        Set_wallops(true);
    }
    else {
        return 1;
    }
    return 0;
};

int     Mode::Oper_add_mode(char m) {
    if (m == 'a') {
        Set_away(true);
    }
    else if (m == 'i') {
        Set_invisible(true);
    }
    else if (m == 'w') {
        Set_wallops(true);
    }
    else if (m == 'r') {
        Set_restricted(true);
    }
    else {
        return 1;
    }
    return 0;
};

int     Mode::Remove_mode(char m) {
    if (m == 'i') {
        Set_invisible(false);
    }
    else if (m == 'w') {
        Set_wallops(false);
    }
    else {
        return 1;
    }
    return 0;
};

int     Mode::Oper_remove_mode(char m) {
    if (m == 'a') {
        Set_away(false);
    }
    else if (m == 'i') {
        Set_invisible(false);
    }
    else if (m == 'w') {
        Set_wallops(false);
    }
    else if (m == 'r') {
        Set_restricted(false);
    }
    else {
        return 1;
    }
    return 0;
};

void    Mode::Set_away(bool away) {
    _away = away;
};

void    Mode::Set_invisible(bool invisible) {
    _invisible = invisible;
};

void    Mode::Set_wallops(bool wallops) {
    _wallops = wallops;
};

void    Mode::Set_restricted(bool restricted) {
    _restricted = restricted;
};

void    Mode::Set_operator(bool op) {
    _operator = op;
};

void    Mode::Set_away_reply(std::string away_reply) {
    _away_reply = away_reply;
};

/*********************/
/***** FUNCTIONS *****/
/*********************/

std::string Mode::Print_mode() const {
    std::string str = "+";
    if (_away == true) {
        str.append("a");
    }
    if (_invisible == true) {
        str.append("i");
    }
    if (_wallops == true) {
        str.append("w");
    }
    if (_restricted == true) {
        str.append("r");
    }
    if (_operator == true) {
        str.append("o");
    }
    return str;
};
