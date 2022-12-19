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
    _local_operator = false;
    _server_notice = false;
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

bool    Mode::Get_local_operator() const {
    return _local_operator;
};

bool    Mode::Get_server_notice() const {
    return _server_notice;
};

/********************/
/***** MODIFIER *****/
/********************/

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

void    Mode::Set_local_operator(bool local_op) {
    _local_operator = local_op;
};

void    Mode::Set_server_notice(bool server_notice) {
    _server_notice = server_notice;
};

/*********************/
/***** FUNCTIONS *****/
/*********************/

std::string Mode::Print_mode() const {
std::cout << "PRINT MODE" << std::endl;
    std::string mode = "Mode :";
    if (_away == true) {
        mode.append("\n- Away");
    }
    if (_invisible == true) {
        mode.append("\n- Invisible");
    }
    if (_wallops == true) {
        mode.append("\n- Wallops");
    }
    if (_restricted == true) {
        mode.append("\n- Restricted");
    }
    if (_operator == true) {
        mode.append("\n- Operator");
    }
    if (_local_operator == true) {
        mode.append("\n- Local Operator");
    }
    if (_server_notice == true) {
        mode.append("\n- Server Notice");
    }
    return mode;
};
