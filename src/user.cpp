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
    _mode = 0;
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

int user::Get_mode() const {
    return _mode;
};

std::string user::Get_realname() const {
    return _realname;
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
        std::cout << "channel not found" << std::endl;
    }
    return 0;
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

void user::Set_mode(int mode) {
    _mode = mode;
};

void user::Set_realname(std::string realname) {
    _realname = realname;
};
