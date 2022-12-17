# include "../class/utils.hpp"

/*********************/
/**** CONSTRUCTOR ****/
/*********************/

user::user(){}

user::user(int  socket, addrinfo info)
    : _addrinfo_client(info) {
    std::cout << "/*** CONSTRUCTOR USER ***/"<< std::endl;
    (void) _addrinfo_client;
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

int user::Get_fd_client() {
    return _fd_poll_client.fd;
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
