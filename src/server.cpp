# include "../class/utils.hpp"
# include "../class/server.hpp"

/*************************/
/*******CONSTRUCTOR*******/
/*************************/

server::server(){}

server::server(char *port, char *password):
_addrinfo(NULL),
_password(password),
_port(port),
_node("127.0.0.1"),
_socket_serv(-1),
_stop(1)
{
    int res;

    std::cout << "*** server Constructor ***" << std::endl;
    // _hints.ai_family = AF_INET;
    // _hints.ai_socktype = SOCK_STREAM;
    // _hints.ai_protocol = 0;

    memset(&_hints, 0, sizeof(struct addrinfo));
    _hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    _hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    _hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    _hints.ai_protocol = 0;          /* Any protocol */
    _hints.ai_canonname = NULL;
    _hints.ai_addr = NULL;
    _hints.ai_next = NULL;

    (void)_password;
    if ((res = _Init_server()) < 0 )
    {
        std::cout << "Error Init server:" << res << std::endl; 
    }
    else
    {
        //socket creer et server en ecoute
        _Infinit_while();
        freeaddrinfo(_addrinfo);
        close(_socket_serv);
    }
    
}

/************************/
/****** DESTRUCTOR ******/
/************************/

server::~server()
{
    std::cout << "DESTRUCTOR SERVER" << std::endl;
}

/***************************/
/******** INIT_SERV ********/
/***************************/

int    server::_Init_server()
{
    std::cout << "*** _Init_server ***" << std::endl;
    if (getaddrinfo(_node, _port, &_hints, &_addrinfo))
    {
        std::cout << "Error: getaddrinfo" << std::endl;
        return (-1);
    }

    // SOCKET
    if ((_socket_serv = socket(_addrinfo->ai_family, _addrinfo->ai_socktype, _addrinfo->ai_protocol)) == -1)
    {
        std::cout << "Error: Creation socket:" << errno << std::endl;
        freeaddrinfo(_addrinfo);
        return (-2);
    }
    std::cout << "Socket serv:" << _socket_serv << std::endl;
    // SOCKET OPTION
    // BIND
    if (bind(_socket_serv, _addrinfo->ai_addr, _addrinfo->ai_addrlen))
    {
        std::cout << "Error: Bind socket:" << errno << std::endl;
        std::cout << EADDRINUSE << std::endl;
        freeaddrinfo(_addrinfo);
        close (_socket_serv);
        return (-3);
    }
    // LISTEN
    if (listen(_socket_serv, MAX_LISTEN))
    {
        close(_socket_serv);
        freeaddrinfo(_addrinfo);
        return (-4);
    }
    _fdpf.fd = _socket_serv;
    _fdpf.events = POLLIN;
    _fdpf.revents = 0;
    return (0);
}

/***************************/
/*******INFINIT_WHILE*******/
/***************************/

void    server::_Infinit_while()
{
    std::cout << "*** Infinit_while ***" << std::endl;
    std::vector<pollfd>::iterator it;
    // Gerer l'ajout d'USER
    // Gerer les commandes des USERS
    
    fcntl(_socket_serv, F_SETFL, O_NONBLOCK); // pour qu'accept ne soit pas bloquant

    int tmp = 0;
    _fds.push_back(_fdpf);
    while (tmp < 8)
    {
        // poll(tab pollfd, size tab, timer)
        poll(_fds.data(), _fds.size(), -1);

        it = _fds.begin();
        // check tous les fds
        while (it != _fds.end())
        {
            std::cout << "it->fd: " << it->fd<< " revents: " << it->revents << std::endl;

            std::cout << "POLLIN =" << POLLIN << std::endl;
            std::cout << "POLLHUP =" << POLLHUP << std::endl;
            
            if (it->revents == POLLHUP) // deconnexion
            {
                std::cout << "*******DECO FD********" << std::endl; 
                _Remove_user(it);
                break;
            }
            if (it->revents == POLLIN) // données en attente de lecture...
            {
                std::cout << "POLL revents fd:" << it->fd << std::endl;
                if (it->fd == _socket_serv) // sur la socket server
                {
                    _Add_user();
                    // modif du vector -> unvalid read/  Conditional jump
                    // Besoin de redefnir it = begin check si begin < it < end
                    // ou break et recommencer la boucle
                    // if (it < _fds.begin() || it >= _fds.end())
                    //     it = _fds.begin();
                    break;
                }
                else // depuis un client
                {
                    _Input_cli(it->fd);
                }
            }
            it++;
        }
        std::cout << "fds: size: " << _fds.size() << std::endl;
        sleep(1);
        tmp++;
    }
    std::vector<user*>::iterator ituser;
    for (ituser = _user.begin(); ituser != _user.end(); ituser = _user.begin())
        _Remove_user(ituser);

}

/******************/
/**** MOD USER ****/
/******************/

void    server::_Add_user()
{
    std::cout << "*** _Add_User ***" << std::endl;

    int socket_cli = 0;
    struct addrinfo    addrinfo_cli;
    struct pollfd   pf;

    addrinfo_cli.ai_addr = NULL;
    addrinfo_cli.ai_addrlen = 0;


    std::cout << "elemt _user:" << _user.size() << std::endl;
    socket_cli = accept(_socket_serv, addrinfo_cli.ai_addr, &addrinfo_cli.ai_addrlen);
   
    if (socket_cli < 0)
    {
        std::cout << "Accept FAIL" << std::endl;
        if (errno == 11)
            std::cout << "Non bloquant no new connect" << std::endl;
        else
            std::cout << "Error Accept in _Add_user:" << errno << std::endl;
    }
    else
    {
        std::cout << "Accept done" << std::endl;
        _user.push_back(new user(socket_cli, addrinfo_cli));
        pf.fd = socket_cli;
        pf.events = POLLIN;
        pf.revents = 0;
        _fds.push_back(pf);


        std::cout << "New User on socket: " << socket_cli << std::endl;
    }
    std::cout << "*** _Add_User END ***" << std::endl;
}

// REMOVE_USER

void    server::_Remove_user(std::vector<user*>::iterator pos)
{
    std::vector<pollfd>::iterator it;
    user *tmp = _user.at(std::distance(_user.begin(), pos));

    for (it = _fds.begin(); it < _fds.end(); it++)
        if (tmp->Get_fd() == it->fd)
            break;

    std::cout << "*** _Remove_User pos ***" << std::endl;
    _user.erase(pos);
    _fds.erase(it);
    delete tmp;
}

void    server::_Remove_user(std::vector<pollfd>::iterator pos)
{
    std::vector<user *>::iterator it = _user.begin();

    for (unsigned int i = 0; i < _user.size(); i++)
    {
        if (_user[i]->Get_fd() == pos->fd)
            break;
        it++;
    }
    user *tmp = _user.at(std::distance(_user.begin(), it));

    std::cout << "*** _Remove_User fd ***" << std::endl;
    _user.erase(it);
    _fds.erase(pos);
    delete tmp;
}

/******************/
/**** GET USER ****/
/******************/

user   *server::_Get_userbyfd(int fd)
{
    std::vector<user *>::iterator   it;

    for (unsigned int i = 0; i < _user.size(); i++)
        if (_user[i]->Get_fd() == fd)
            return (_user[i]);
    return (NULL);
}

/*********************/
/**** MOD CHANNEL ****/
/*********************/


// remplacer par JOIN ?
void    server::_Add_channel(std::string name, user creator)
{
    std::cout << "*** _Add_channel ***" << std::endl;

    // check name channel valide
    // check pas de doublon de channel

    _channel.push_back(new channel(name, creator));

}

void    server::_Remove_channel(channel *chan) // voire si passe les name ou channel*
{
    std::cout << "*** _Remove_channel ***" << std::endl;

    // retirer tous les user du channel && retirer ce channel des vector user._channel
    //


    delete chan;
}

/****************/
/**** DIVERS ****/
/****************/

int server::_Input_cli(int fd)
{
        std::cout << "*** _Input_cli: " << fd << "***" << std::endl;
    char        inpt[50];
    ssize_t         ret = -1;

    user    *test = _Get_userbyfd(fd);
    if ((ret = recv(fd, inpt, 49, 0)) == -1)
        return (-1);
    inpt[ret] = 0;

    std::cout << std::endl << "///" << inpt << "///" << std::endl;

    test->str.append(inpt);
    if (test->str.find("\n", 0) != std::string::npos) // ligne complete -> traite -> delete
    {
        std::cout << "   res:" << test->str; // Just to show in server terminal
        // Use line
        test->str.clear();// delete
    }
    std::cout << "*** _END inpt"  << std::endl;
    return (0);
}

int server::_Output_cli(int fd, std::string msg)
{
    std::cout << "*** _Output_cli: " << fd << "***" << std::endl;
    ssize_t ret;

    msg.append("\r\n"); // voir si on ajoute ici ou dans les cmd de bases
    ret = send(fd, msg.c_str(), msg.size(), 0);

    return (0);
}
