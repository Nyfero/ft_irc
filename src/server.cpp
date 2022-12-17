# include "../class/utils.hpp"

bool g_stop;

/*************************/
/*******CONSTRUCTOR*******/
/*************************/

server::server(char *port, char *password)
    : _addrinfo(NULL), _password(password), _port(port), _node("127.0.0.1"), _socket_serv(-1) {
    int res;
    g_stop = 1;

    std::cout << "*** server Constructor ***" << std::endl;

    memset(&_hints, 0, sizeof(struct addrinfo));
    _hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    _hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    _hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    _hints.ai_protocol = 0;          /* Any protocol */
    _hints.ai_canonname = NULL;
    _hints.ai_addr = NULL;
    _hints.ai_next = NULL;

    (void)_password;
    if ((res = _Init_server()) < 0 ) {
        std::cout << "Error Init server:" << res << std::endl; 
    }
    else {
        //socket creer et server en ecoute
        _Launch_server();
        freeaddrinfo(_addrinfo);
        close(_socket_serv);
    }  
};

/************************/
/****** DESTRUCTOR ******/
/************************/

server::~server() {
    std::cout << "DESTRUCTOR SERVER" << std::endl;
};

/***************************/
/******** INIT_SERV ********/
/***************************/

int    server::_Init_server() {
    std::cout << "*** _Init_server ***" << std::endl;
    if (getaddrinfo(_node, _port, &_hints, &_addrinfo)) {
        std::cout << "Error: getaddrinfo" << std::endl;
        return -1;
    }

    // SOCKET
    if ((_socket_serv = socket(_addrinfo->ai_family, _addrinfo->ai_socktype, _addrinfo->ai_protocol)) == -1) {
        std::cout << "Error: Creation socket:" << errno << std::endl;
        freeaddrinfo(_addrinfo);
        return -2;
    }
    std::cout << "Socket serv:" << _socket_serv << std::endl;
    // SOCKET OPTION
    // BIND
    if (bind(_socket_serv, _addrinfo->ai_addr, _addrinfo->ai_addrlen)) {
        std::cout << "Error: Bind socket:" << errno << std::endl;
        std::cout << EADDRINUSE << std::endl;
        freeaddrinfo(_addrinfo);
        close (_socket_serv);
        return -3;
    }

    // LISTEN
    if (listen(_socket_serv, MAX_LISTEN)) {
        close(_socket_serv);
        freeaddrinfo(_addrinfo);
        return -4;
    }
    _serv_poll_fd.fd = _socket_serv;
    _serv_poll_fd.events = POLLIN | POLLOUT;
    _serv_poll_fd.revents = 0;
    return 0;
};

/***************************/
/*******INFINIT_WHILE*******/
/***************************/

void    server::_Launch_server() {
    std::cout << "*** Infinit_while ***" << std::endl;
    std::vector<pollfd>::iterator it;
    // Gerer l'ajout d'USER
    // Gerer les commandes des USERS
    
    fcntl(_socket_serv, F_SETFL, O_NONBLOCK); // pour qu'accept ne soit pas bloquant

    _list_poll_fd.push_back(_serv_poll_fd);

    int limit = 0;
    while (g_stop && limit <= 10) {
        // poll(tab pollfd, size tab, timer)
        poll(_list_poll_fd.data(), _list_poll_fd.size(), -1);

        it = _list_poll_fd.begin();
        // check tous les fds
        while (it != _list_poll_fd.end()) {
            std::cout << "it->fd: " << it->fd<< " revents: " << it->revents << std::endl;
            
            if (it->revents & POLLHUP) { // deconnexion
                std::cout << "*******DECO FD********" << std::endl; 
                _Remove_user(it);
                break;
            }
            if (it->revents & POLLIN) { // données en attente de lecture...
                std::cout << "POLL revents fd:" << it->fd << std::endl;
                if (it->fd == _socket_serv) { // sur la socket server
                    _Add_user();
                    break;
                }
                else { // depuis un client
                    if (_Input_client(it) == -2) {
                        break;
                    }
                }
            }
            it++;
        }
        std::cout << "fds: size: " << _list_poll_fd.size() << std::endl;
        sleep(1);
        limit++;
    }
    std::vector<user*>::iterator ituser;
    for (ituser = _list_user.begin(); ituser != _list_user.end(); ituser = _list_user.begin()) {
        _Remove_user(ituser);
    }
};

/******************/
/**** MOD USER ****/
/******************/

void    server::_Add_user() {
    std::cout << "*** _Add_User ***" << std::endl;

    int socket_cli = 0;
    struct addrinfo    addrinfo_cli;
    struct pollfd   pf;

    addrinfo_cli.ai_addr = NULL;
    addrinfo_cli.ai_addrlen = 0;

    std::cout << "elemt _user:" << _list_user.size() << std::endl;
    socket_cli = accept(_socket_serv, addrinfo_cli.ai_addr, &addrinfo_cli.ai_addrlen);
   
    if (socket_cli < 0) {
        std::cout << "Accept FAIL" << std::endl;
        if (errno == 11) {
            std::cout << "Non bloquant no new connect" << std::endl;
        }
        else {
            std::cout << "Error Accept in _Add_user:" << errno << std::endl;
        }
    }
    else {
        std::cout << "Accept done" << std::endl;
        _list_user.push_back(new user(socket_cli, addrinfo_cli));
        pf.fd = socket_cli;
        pf.events = POLLIN;
        pf.revents = 0;
        _list_poll_fd.push_back(pf);


        std::cout << "New User on socket: " << socket_cli << std::endl;
    }
    std::cout << "*** _Add_User END ***" << std::endl;
};

// REMOVE_USER

void    server::_Remove_user(std::vector<user*>::iterator pos) {
    std::vector<pollfd>::iterator it;
    user *tmp = _list_user.at(std::distance(_list_user.begin(), pos));

    for (it = _list_poll_fd.begin(); it < _list_poll_fd.end(); it++) {
        if (tmp->Get_fd_client() == it->fd) {
            break;
        }
    }

    std::cout << "*** _Remove_User pos ***" << std::endl;
    std::cout << "*** _FD = " << tmp->Get_fd_client() << "***" << std::endl;
    close (tmp->Get_fd_client());
    _list_user.erase(pos);
    _list_poll_fd.erase(it);
    delete tmp;
};

void    server::_Remove_user(std::vector<pollfd>::iterator pos) {
    std::vector<user *>::iterator it = _list_user.begin();

    for (unsigned int i = 0; i < _list_user.size(); i++) {
        if (_list_user[i]->Get_fd_client() == pos->fd) {
            break;
        }
        it++;
    }
    user *tmp = _list_user.at(std::distance(_list_user.begin(), it));

    std::cout << "*** _Remove_User fd ***" << std::endl;
    close (tmp->Get_fd_client());
    _list_user.erase(it);
    _list_poll_fd.erase(pos);
    delete tmp;
};

/******************/
/**** GET USER ****/
/******************/

user   *server::_Get_user_by_fd(int fd) {
    std::vector<user *>::iterator   it;

    for (unsigned int i = 0; i < _list_user.size(); i++) {
        if (_list_user[i]->Get_fd_client() == fd) {
            return (_list_user[i]);
        }
    }
    return NULL;
};

/*********************/
/**** MOD CHANNEL ****/
/*********************/


// remplacer par JOIN ?
void    server::_Add_channel(std::string name, user *creator) {
    std::cout << "*** _Add_channel ***" << std::endl;

    // check name channel valide
    // check pas de doublon de channel

    _list_channel.push_back(new channel(name, creator));

};

void    server::_Remove_channel(channel *chan) {
    std::cout << "*** _Remove_channel ***" << std::endl;

    // retirer tous les user du channel && retirer ce channel des vector user._channel
    //


    delete chan;
};

/****************/
/**** DIVERS ****/
/****************/

int server::_Input_client(std::vector<pollfd>::iterator it) {
    std::cout << std::endl << "*** _Input_client: " << it->fd << "***" << std::endl;
    int         fd = it->fd;
    char        inpt[SIZE_INPT];
    ssize_t     ret = -1;
    std::string res;
    size_t      found;

    user    *test = _Get_user_by_fd(fd);
    if ((ret = recv(fd, inpt, SIZE_INPT - 1, 0)) == -1) {
        return -1;
    }
    if (!ret) { // disconnect
        _Remove_user(it);
        return -2;
    }
    inpt[ret] = 0;

    // append inpt dans srt de user
    test->str.append(inpt);
    while ((found = test->str.find("\n", 0)) != std::string::npos) { // ligne complete -> traite -> delete
        std::cout << "  b_str:" << test->str << std::endl; // Just to show in server terminal
        
        
        res = test->str.substr(0, found); // get first line in res
        test->str.erase(0, found + 1); // get after first line in str
        
        // std::cout  <<"  e_str:" << test->str << std::endl;
        // std::cout << "    res:" << res << std::endl;

        // Use line ex: USE_CMD(user &client, std::string line)
        Enter(test, res);
        // client = test -> cellu qui lance la command
        // line = res -> command a traiter
    }
    std::cout << "*** _END inpt"  << std::endl;
    return 0;
};

int server::_Output_client(int fd, std::string msg) {
    std::cout << "*** _Output_client: " << fd << "***" << std::endl;
    ssize_t ret;

    msg.append("\r\n"); // voir si on ajoute ici ou dans les cmd de bases
    ret = send(fd, msg.c_str(), msg.size(), 0);

    return 0;
};
