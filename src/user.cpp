# include "../class/utils.hpp"

/*********************/
/**** CONSTRUCTOR ****/
/*********************/

user::user(){}

user::user(int  socket, addrinfo info)
    : _addrinfo_client(info) {
    (void) _addrinfo_client;
    _mode = new Mode;
    _login_status = 0;
    _fd_poll_client.fd = socket,
    _fd_poll_client.revents = 0,
    _fd_poll_client.events = POLLIN,
    std::cout << "*** User create with fd: " << _fd_poll_client.fd << " ***" << std::endl;
};

/********************/
/**** DESTRUCTOR ****/
/********************/

user::~user() {
    std::cout << "*** User destroy: " << _fd_poll_client.fd << " ***" << std::endl;
    delete _mode;
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

int user::Get_login_status() const {
    return _login_status;
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

Mode        *user::Get_mode() const {
    return _mode;
};

std::string user::Get_realname() const {
    return _realname;
};

std::vector<channel *>  user::Get_channel_register() const{
    return _channel_register;
};

bool user::Is_user_channel(channel *chan) const{
    std::vector<user *> list;

    if (!chan)
        return false;
    list = chan->Get_list_channel_user();
    for (size_t i = 0; i < list.size(); i++)
    {
        if (_nickname == list[i]->Get_nickname())
            return true;

    }
    return false;
}

bool user::Is_op_channel(channel *chan) const{
    std::vector<user *> list;

    if (!chan)
        return false;
    list = chan->Get_list_operator();
    for (size_t i = 0; i < list.size(); i++)
    {
        if (_nickname == list[i]->Get_nickname())
            return true;

    }
    return false;
}


/**************/
/****  MOD ****/
/**************/

int user::Add_channel(channel *chan) {
    std::cout << "*** USER::ADD_CHANNEL ***"<< std::endl; 
    _channel_register.push_back(chan);
    return 0;
};

int user::Remove_Channel(channel *chan) {
    std::vector<channel *>::iterator it, ite = _channel_register.end();
    unsigned int    i = 0;

    for (it = _channel_register.begin(); it != ite; it++) {
        if (_channel_register[i]->Get_channel_name() == chan->Get_channel_name()) {
            std::cout << "*** USER::REMOVE_CHANNEL ***"<< std::endl;
            _channel_register.erase(it);
            break;
        }
        i++;
    }
    if (it == ite) {
        std::cerr << "*** channel not found ***" << std::endl;
    }
    return 0;
};

void    user::Remove_all_channel(){
    std::cout << "*** USER::REMOVE_ALL_CHANNEL ***"<< std::endl;
    if (!_channel_register.empty())
        _channel_register.clear();
};

void user::Set_login_status(int modif) {
    _login_status = modif;
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
            _mode->Set_away(true);
        }
        else if (mode[1] == 'i') {
            _mode->Set_invisible(true);
        }
        else if (mode[1] == 'w') {
            _mode->Set_wallops(true);
        }
        else if (mode[1] == 'r') {
            _mode->Set_restricted(true);
        }
        else if (mode[1] == 'o') {
            _mode->Set_operator(true);
        }
    }
    else if (mode[0] == '-') {
        if (mode[1] == 'a') {
            _mode->Set_away(false);
        }
        else if (mode[1] == 'i') {
            _mode->Set_invisible(false);
        }
        else if (mode[1] == 'w') {
            _mode->Set_wallops(false);
        }
        else if (mode[1] == 'r') {
            _mode->Set_restricted(false);
        }
        else if (mode[1] == 'o') {
            _mode->Set_operator(false);
        }
    }
};

void user::Set_realname(std::string realname) {
    _realname = realname;
};


