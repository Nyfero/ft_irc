# include "../class/utils.hpp"

/*********************/
/**** CONSTRUCTOR ****/
/*********************/

user::user(){}

user::user(int  socket, addrinfo info)
    : _addrinfo_client(info) {
    std::cout << "/*** CONSTRUCTOR USER ***/"<< std::endl;
    (void) _addrinfo_client;
    _is_register = false;
    _fd_poll_client.fd = socket,
    _fd_poll_client.revents = 0,
    _fd_poll_client.events = POLLIN,
    std::cout << "User create: " << _fd_poll_client.fd << std::endl;
};

/********************/
/**** DESTRUCTOR ****/
/********************/

user::~user() {
    std::cout << "User destroy: " << _fd_poll_client.fd << std::endl;
    close(_fd_poll_client.fd);
};

/********************/
/***** OPERATOR *****/
/********************/

/********************/
/***** ACCESSOR *****/
/********************/

int user::Get_fd_client() const {
    return _fd_poll_client.fd;
};

bool user::Get_is_register() const {
    return _is_register;
};

std::string user::Get_username() const {
    return _username;
};

std::string user::Get_nickname() const {
    return _nickname;
};

std::string user::Get_hostname() const {
    return _hostname;
};

Mode    user::Get_mode() const {
    return _mode;
};

std::string user::Get_realname() const {
    return _realname;
};

std::vector<channel *>  user::Get_channel_register() const{
    return _channel_register;
};


/**************/
/****  MOD ****/
/**************/

int user::Add_channel(channel *chan) {
    std::cout << "/*** USER::ADD_CHANNEL ***/"<< std::endl; 
    _channel_register.push_back(chan);
    return 0;
};

int user::Remove_Channel(channel *chan) {
    std::cout << "/*** USER::REMOVE_CHANNEL ***/"<< std::endl;
    std::vector<channel *>::iterator it, ite = _channel_register.end();
    unsigned int    i = 0;

    for (it = _channel_register.begin(); it != ite; it++) {
        if (_channel_register[i]->Get_channel_name() == chan->Get_channel_name()) {
            std::cout << "channel found" << std::endl;
            _channel_register.erase(it);
            break;
        }
        i++;
    }
    if (it == ite) {
        std::cout << "channel not found for user" << std::endl;
    }
    return 0;
};

void    user::Remove_all_channel(){
    std::cout << "/*** USER::REMOVE_ALL_CHANNEL ***/"<< std::endl;
    if (!_channel_register.empty())
        _channel_register.clear();
};

void user::Set_is_register(bool modif) {
    _is_register = modif;
};

void user::Set_nickname(std::string nickname) {
    _nickname = nickname;
};

void user::Set_username(std::string username) {
    _username = username;
};

void user::Set_hostname(std::string hostname) {
    _hostname = hostname;
};

void user::Set_mode(std::string mode) {
    if (isNumber(mode)) {
        return;
    }
    else if (mode[0] == '+') {
        if (mode[1] == 'a') {
            _mode.Set_away(true);
        }
        else if (mode[1] == 'i') {
            _mode.Set_invisible(true);
        }
        else if (mode[1] == 'w') {
            _mode.Set_wallops(true);
        }
        else if (mode[1] == 'r') {
            _mode.Set_restricted(true);
        }
        else if (mode[1] == 'o') {
            _mode.Set_operator(true);
        }
        else if (mode[1] == 'O') {
            _mode.Set_local_operator(true);
        }
        else if (mode[1] == 's') {
            _mode.Set_server_notice(true);
        }
    }
    else if (mode[0] == '-') {
        if (mode[1] == 'a') {
            _mode.Set_away(false);
        }
        else if (mode[1] == 'i') {
            _mode.Set_invisible(false);
        }
        else if (mode[1] == 'w') {
            _mode.Set_wallops(false);
        }
        else if (mode[1] == 'r') {
            _mode.Set_restricted(false);
        }
        else if (mode[1] == 'o') {
            _mode.Set_operator(false);
        }
        else if (mode[1] == 'O') {
            _mode.Set_local_operator(false);
        }
        else if (mode[1] == 's') {
            _mode.Set_server_notice(false);
        }
    }
};

void user::Set_realname(std::string realname) {
    _realname = realname;
};


void    user::Print_channel_user()
{
    std::cout << "     user channel: " << _channel_register.size() <<std::endl;
    for (size_t i=0; i< _channel_register.size();i++)
    {
        std::cout << i << ": " << _channel_register[i]->Get_channel_name() << std::endl;
    }
    std::cout << std::endl;
}

