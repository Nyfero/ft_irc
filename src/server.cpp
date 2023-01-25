# include "../class/utils.hpp"

bool g_stop;

/*************************/
/*******CONSTRUCTOR*******/
/*************************/

server::server(char *port, char *password)
    : _addrinfo(NULL), _name_serveur("localhost"), _password(password), _admin_password("ecole42"), _port(port), _node("127.0.0.1"), _socket_serv(-1) {
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

    std::cout << "STARTING SERVER" << std::endl;
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
        std::cerr << "Error: getaddrinfo" << std::endl;
        return -1;
    }

    // SOCKET
    if ((_socket_serv = socket(_addrinfo->ai_family, _addrinfo->ai_socktype, _addrinfo->ai_protocol)) == -1) {
        std::cerr << "Error: Creation socket:" << errno << std::endl;
        freeaddrinfo(_addrinfo);
        return -2;
    }

    // BIND
    if (bind(_socket_serv, _addrinfo->ai_addr, _addrinfo->ai_addrlen)) {
        std::cerr << "Error: Bind socket:" << errno << std::endl;
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
    
    fcntl(_socket_serv, F_SETFL, O_NONBLOCK); // pour qu'accept ne soit pas bloquant
    _list_poll_fd.push_back(_serv_poll_fd);
    while (g_stop) {
        //  DEBUG
        // _Print_user();
        // _Print_channel();

        // poll(tab pollfd, size tab, timer)
        poll(_list_poll_fd.data(), _list_poll_fd.size(), -1);
        it = _list_poll_fd.begin();
        // check tous les fds
        while (it != _list_poll_fd.end()) {
            if (it->revents & POLLHUP) { // deconnexion
                _Output_all_user_channel(_Get_user_by_fd(it->fd), "");
                _Remove_user(it);
                break;
            }
            if (it->revents & POLLIN) { // données en attente de lecture...
                if (it->fd == _socket_serv) { // From server's socket
                    _Add_user();
                    break;
                }
                else { // From client
                    try{
                        if (_Input_client(it) == -2)
                            break;
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
            }
            it++;
        }
    }
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
    int socket_cli = 0;
    struct addrinfo    addrinfo_cli;
    struct pollfd   pf;

    addrinfo_cli.ai_addr = NULL;
    addrinfo_cli.ai_addrlen = 0;

    socket_cli = accept(_socket_serv, addrinfo_cli.ai_addr, &addrinfo_cli.ai_addrlen);
   
    if (socket_cli < 0) {
        std::cerr << "*** Invalid client ==> user rejected ***" << std::endl;
    }
    else {
        _list_user.push_back(new user(socket_cli, addrinfo_cli));
        pf.fd = socket_cli;
        pf.events = POLLIN;
        pf.revents = 0;
        _list_poll_fd.push_back(pf);
        std::cout << "*** New User on socket: " << socket_cli << " ***" << std::endl;
    }
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

    // check list invit channel
    std::vector<channel *>::iterator it2;    
    for (it2 = _list_channel.begin(); it2 != _list_channel.end(); it2++) { // check all chan
        channel *serv_chan = (*it2);

        if (serv_chan->_Is_invit(tmp))
            serv_chan->Remove_invited_user(tmp);
    }

    // supprimer l'use de tous les channe la auxquel il appartient
    while (!tmp->Get_channel_register().empty()) {
        channel *chan = tmp->Get_channel_register().front();
        chan->Remove_user(tmp); // retirer l'user du hannel 
        tmp->Remove_Channel(chan); // retirer channel du user
        if (chan->Get_list_channel_user().empty()) // si channeldevient vide,le  delete
            _Remove_channel(chan);
        else if (chan->Get_list_operator().empty()) // sinon si l'user etait le dernier op on le remplace
            chan->Add_oper(tmp, chan->Get_list_operator().front());
    }
    close (tmp->Get_fd_client());   // close socket
    _list_user.erase(pos);          // remove user from list_user
    _list_poll_fd.erase(it);        // remove user from poll_fd
    delete tmp;                     // del user
};

void    server::_Remove_user(std::vector<pollfd>::iterator pos) {
    std::vector<user *>::iterator it = _list_user.begin();

    for (unsigned int i = 0; i < _list_user.size(); i++) {
        if (_list_user[i]->Get_fd_client() == pos->fd)
            break;
        it++;
    }

    user *use = _Get_user_by_fd(pos->fd);

    std::vector<channel *>::iterator it2;
    for (it2 = _list_channel.begin(); it2 != _list_channel.end(); it2++) { // check all chan
        channel *serv_chan = (*it2);

        if (serv_chan->_Is_invit(use))
            serv_chan->Remove_invited_user(use);
    }

    // leave all channel
    std::vector<channel *> l_chan = use->Get_channel_register();
    for (size_t i = 0; i < l_chan.size(); i++) {
        l_chan[i]->Remove_user(use);
        use->Remove_Channel(l_chan[i]);

        std::vector<user *>::iterator it2;
        if (l_chan[i]->Get_list_channel_user().empty())
            _Remove_channel(l_chan[i]);
        else if (l_chan[i]->Get_list_operator().empty())
            l_chan[i]->Add_oper(use, l_chan[i]->Get_list_channel_user().front());
    }

    close (use->Get_fd_client());   // close socket
    _list_user.erase(it);           // remove user from list_user
    _list_poll_fd.erase(pos);       // remove user from poll_fd
    delete use;                     // del user
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
channel    *server::_Add_channel(std::string name, user *creator) {
    std::cout << "*** _Add_channel ***" << std::endl;
    _list_channel.push_back(new channel(name, creator));
    return (_list_channel.back());
};

channel     *server::_Add_channel(std::string name, user *creator, std::string key){
    std::cout << "*** _Add_channel with key ***" << std::endl;
    channel *chan = new channel(name ,creator, key);
    _list_channel.push_back(chan);
    return (_list_channel.back());
};

void    server::_Remove_channel(channel *chan) {
    std::cout << "*** _Remove_channel ***" << std::endl;
    
    for (size_t  i = 0; i < chan->Get_list_channel_user().size(); i++)
        chan->Get_list_channel_user()[i]->Remove_Channel(chan);

    std::vector<channel *>::iterator    it = _list_channel.begin();
    for (size_t i = 0; it < _list_channel.end(); it++) {
        if (_list_channel[i]->Get_channel_name() == chan->Get_channel_name()) {
            _list_channel.erase(it);
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
    user        *sender = _Get_user_by_fd(fd);

    if ((ret = recv(fd, inpt, SIZE_INPT - 1, 0)) == -1)
        return -1;
    if (!ret) { // disconnect
        _Output_all_user_channel(sender, "");
        _Remove_user(it);
        return -2;
    }
    inpt[ret] = 0;
    // append inpt in user's str
    sender->str.append(inpt);
    while ((found = sender->str.find("\n", 0)) != std::string::npos) { // ligne complete -> traite -> delete
        res = sender->str.substr(0, found); // get first line in res
        sender->str.erase(0, found + 1);    // get after first line in str
        if ((found = res.find("\r", 0)) != std::string::npos) // delete \r cause we doesn't like it
            res.erase(found);
        if (Check_command(sender, res) == -2) {// check si cmd valide and ret -2 for quit
            // if command is QUIT
            _Remove_user(it);
            return -2;
        }
    }
    return 0;
};

int server::_Output_client(int fd, std::string msg) {
    ssize_t ret;

    msg.append("\r\n");
    std::cout << fd << " >> " << msg << std::endl;  // print send message
    ret = send(fd, msg.c_str(), msg.size(), 0);
    return 0;
};

int server::_Output_channel(channel *chan, std::string msg) {
    std::vector<user *> list = chan->Get_list_channel_user();
    std::vector<user *>::iterator it;

    for (it = list.begin(); it < list.end(); it++)
        _Output_client((*it)->Get_fd_client(), msg);
    return 0;
};

int     server::_Output_all_user_channel(user *user, std::string msg) {
    std::vector<channel *> chan = user->Get_channel_register();
    std::string str;
    std::string prefix = ":" + user->Get_nickname() + "!" + user->Get_username() + "@" + user->Get_hostname();
    
    for (size_t i = 0; i < chan.size(); i++) {
        str = prefix + " PART " + chan[i]->Get_channel_name();
        if (!msg.empty())
            str += msg;
        user->Remove_Channel(chan[i]);
        chan[i]->Remove_user(user);
        
        _Output_channel(chan[i], str);
        if (chan[i]->Get_list_channel_user().empty())   // If channel empty
            _Remove_channel(chan[i]);                   // Remove channel
        else if (chan[i]->Get_list_operator().empty())  // If no oper but user
            chan[i]->Add_oper(user, chan[i]->Get_list_channel_user().front());  // older user become oper
    }
    return 0;
}

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

void    server::_Print_user()
{
    std::cout << "nombre user: " << _list_user.size() <<std::endl;

    std::vector<channel *> chan;
    for (size_t i=0; i< _list_user.size();i++)
    {
        std::cout << i << ": fd " << _list_user[i]->Get_fd_client() << ":" << _list_user[i]->Get_nickname() << std::endl;
        chan = _list_user[i]->Get_channel_register();
        for (size_t i = 0; i < chan.size(); i++)
            std::cout << "   " << i << ":" << chan[i]->Get_channel_name() << std::endl;
    }
};

bool    server::_User_is_in_chan(user *use, channel *chan){
    std::vector<user*> liste;

    liste = chan->Get_list_channel_user();
    for (size_t i = 0; i < liste.size(); i++){
        if (liste[i]->Get_fd_client() == use->Get_fd_client())
            return true;
    }
    return false;
}

user *server::_Get_user_by_nick(std::string str) {
    for (size_t i = 0; i < _list_user.size(); i++) {
        if (Compare_case_sensitive(_list_user.at(i)->Get_nickname(), str)) {
            return _list_user.at(i);
        }
    }
    return NULL;
};