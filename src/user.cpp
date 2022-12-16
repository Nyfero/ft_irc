# include "../class/utils.hpp"

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

user::~user()
{
    std::cout << "User destroy: " << _fd_poll.fd << std::endl;
    close(_fd_poll.fd);
}

int user::Get_fd(){
    return (this->_fd_poll.fd);
}

