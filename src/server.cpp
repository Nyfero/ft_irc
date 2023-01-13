# include "../class/utils.hpp"

bool g_stop;

/*************************/
/*******CONSTRUCTOR*******/
/*************************/

server::server(char *port, char *password)
    : _addrinfo(NULL), _name_serveur("localhost"), _password(password), _port(port), _node("127.0.0.1"), _socket_serv(-1) {
    int res;
    g_stop = 1;

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
        ;
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
    _serv_poll_fd.events = POLLIN;
    _serv_poll_fd.revents = 0;
    return 0;
};

/***************************/
/*******INFINIT_WHILE*******/
/***************************/

void    server::_Launch_server() {
    std::vector<pollfd>::iterator it;
    // Gerer l'ajout d'USER
    // Gerer les commandes des USERS
    
    fcntl(_socket_serv, F_SETFL, O_NONBLOCK); // pour qu'accept ne soit pas bloquant

    _list_poll_fd.push_back(_serv_poll_fd);

    while (g_stop) {
        _Print_channel();
        // poll(tab pollfd, size tab, timer)
        poll(_list_poll_fd.data(), _list_poll_fd.size(), -1);

        it = _list_poll_fd.begin();
        // check tous les fds
        while (it != _list_poll_fd.end()) {
            
            if (it->revents & POLLHUP) { // deconnexion
                _Remove_user(it);
                break;
            }
            if (it->revents & POLLIN) { // données en attente de lecture...
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
    }

    std::cout << "EXIT WHILE INFINIT" << std::endl;

    std::vector<user*>::iterator ituser;
    for (ituser = _list_user.begin(); ituser != _list_user.end(); ituser = _list_user.begin()) {
        _Remove_user(ituser);
    }

    for (size_t i = 0; !_list_channel.empty(); i++) {
        _Remove_channel(_list_channel[i]);
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
    std::cout << "reove user user" << std::endl;
    std::vector<pollfd>::iterator it;
    user *tmp = _list_user.at(std::distance(_list_user.begin(), pos));

    for (it = _list_poll_fd.begin(); it < _list_poll_fd.end(); it++) {
        if (tmp->Get_fd_client() == it->fd) {
            break;
        }
    }

    // supprimer l'use de tous les channe la auxquel i lappartient

    while (!tmp->Get_channel_register().empty())
    {
        channel *chan = tmp->Get_channel_register().front();
        chan->Remove_user(tmp); // retirer l'user du hannel 
        tmp->Remove_Channel(chan); // retirer channel du user 
        if (chan->Get_list_channel_user().empty()) // si channeldevient vide,le  delete
            _Remove_channel(chan);
        else if (chan->Get_list_operator().empty()) // sinon si l'user etait le dernier op on le remplace
            chan->Add_oper(chan->Get_list_operator().front());
    }

    std::cout << "*** _Remove_User pos ***" << std::endl;
    std::cout << "*** _FD = " << tmp->Get_fd_client() << "***" << std::endl;
    _Output_client(tmp->Get_fd_client(), "You have been disconnected");
    close (tmp->Get_fd_client());
    _list_user.erase(pos);
    _list_poll_fd.erase(it);
    delete tmp;
};

void    server::_Remove_user(std::vector<pollfd>::iterator pos) {
    std::cout << "reove user pollfd" << std::endl;

    std::vector<user *>::iterator it = _list_user.begin();

    for (unsigned int i = 0; i < _list_user.size(); i++) {
        if (_list_user[i]->Get_fd_client() == pos->fd) {
            break;
        }
        it++;
    }
    user *tmp = _list_user.at(std::distance(_list_user.begin(), it));

    // leave all channel
    while (!tmp->Get_channel_register().empty())
    {
        channel *chan = tmp->Get_channel_register().front();
        chan->Remove_user(tmp);
        tmp->Remove_Channel(chan);   
        if (chan->Get_list_channel_user().empty())
            _Remove_channel(chan);
        else if (chan->Get_list_operator().empty())
            chan->Add_oper(chan->Get_list_operator().front());
    }

    std::cout << "*** _Remove_User fd ***" << std::endl;
    _Output_client(tmp->Get_fd_client(), "You have been disconnected");
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

channel     *server::_Add_channel(std::string name, user *creator, std::string key){
    std::cout << "*** _Add_channel with key ***" << std::endl;
    channel *chan = new channel(name ,creator, key);
    _list_channel.push_back(chan);
    creator->Add_channel(chan);
    return (_list_channel.back());
};

channel     *server::_Add_channel(std::string name, user *creator) {
    std::cout << "*** _Add_channel ***" << std::endl;
    channel *chan = new channel(name ,creator);
    _list_channel.push_back(chan);
    creator->Add_channel(chan);
    return (_list_channel.back());
};

void    server::_Remove_channel(channel *chan) {
    std::cout << "*** _Remove_channel :" << chan->Get_channel_name() << "***" << std::endl;
    
    std::cout << "retirer user duchannel" << std::endl;
    for (size_t  i = 0; i < chan->Get_list_channel_user().size(); i++)
        chan->Get_list_channel_user()[i]->Remove_Channel(chan);

    std::cout << "retirer le channel de vector sever" << std::endl;
    std::vector<channel *>::iterator    it = _list_channel.begin();
    for (size_t i = 0; it != _list_channel.end(); it++)
    {
        std::cout << "Uwu" << std::endl;
        if (_list_channel[i]->Get_channel_name() == chan->Get_channel_name())
        {
            std::cout << "Owo" << std::endl;
            _list_channel.erase(it);
            std::cout << "Oasdwo" << std::endl;
            break;
        }
        i++;
    }
    delete chan;
};

/****************/
/**** DIVERS ****/
/****************/

int server::_Input_client(std::vector<pollfd>::iterator it) {
    int         fd = it->fd;
    char        inpt[SIZE_INPT];
    ssize_t     ret = -1;
    std::string res;
    size_t      found;

    user    *test = _Get_user_by_fd(fd);
    std::cout << "input client : 0" << std::endl;
    if ((ret = recv(fd, inpt, SIZE_INPT - 1, 0)) == -1) {
        return -1;
    }
    if (!ret) { // disconnect
        _Remove_user(it);
        return -2;
    }
    inpt[ret] = 0;

    // append inpt dans str de user
    test->str.append(inpt);
    while ((found = test->str.find("\n", 0)) != std::string::npos) { // ligne complete -> traite -> delete
        
        
        res = test->str.substr(0, found); // get first line in res
        test->str.erase(0, found + 1); // get after first line in str
        
        if ((found = res.find("\r", 0)) != std::string::npos)
            res.erase(found);
        std::cout << "input client : 3s" << std::endl;
        if (Check_command(test, res) < 0) // check si cmd valide
        {
            _Remove_user(it);
            return -2;
        }
    }
    return 0;
};

int server::_Output_client(int fd, std::string msg) {
    ssize_t ret;

    msg.append("\r\n"); // voir si on ajoute ici ou dans les cmd de bases
    ret = send(fd, msg.c_str(), msg.size(), 0);

    return 0;
};

int server::_Output_channel(channel *chan, std::string msg) {
    std::vector<user *> list = chan->Get_list_channel_user();
    std::vector<user *>::iterator it;

    for (it = list.begin(); it < list.end(); it++) {
        _Output_client((*it)->Get_fd_client(), msg);
    }
    return 0;
};
void    server::_Print_channel()
{
    std::cout << "nombrechannel: " << _list_channel.size() <<std::endl;
    for (size_t i=0; i< _list_channel.size();i++)
    {
        std::cout << i << ": " << _list_channel[i]->Get_channel_name() << std::endl;
        _list_channel[i]->print_user_channel();
        _list_channel[i]->print_oper_channel();
    }
};
