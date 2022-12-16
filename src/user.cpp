# include "../class/utils.hpp"
# include "../class/user.hpp"

/*********************/
/**** CONSTRUCTOR ****/
/*********************/

user::user(){}

user::user(int  socket, addrinfo info):
_addrinfo_cli(info)
{
    std::cout << "/*** CONSTRUCTOR USER ***/"<< std::endl;
    (void) _addrinfo_cli;
    _fd_poll.fd = socket,
    _fd_poll.revents = 0,
    _fd_poll.events = POLLIN,
    std::cout << "User create: " << _fd_poll.fd << std::endl;
}

/********************/
/**** DESTRUCTOR ****/
/********************/

user::~user()
{
    std::cout << "User destroy: " << _fd_poll.fd << std::endl;
    close(_fd_poll.fd);
}

/********************/
/***** OPERATOR *****/
/********************/

/********************/
/***** ACCESSOR *****/
/********************/

int user::Get_fd(){
    return (this->_fd_poll.fd);
}

/**************/
/****  MOD ****/
/**************/

int user::Add_channel(channel chan)
{
     std::cout << "/*** USER::ADD_CHANNEL ***/"<< std::endl; 
     (void)chan;
     return (0);
}

int user::Remove_Channel(channel chan)
{
    std::cout << "/*** USER::REMOVE_CHANNEL ***/"<< std::endl;
    (void)chan;
    return (0);
}
