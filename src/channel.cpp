# include "../class/utils.hpp"

/***************************/
/******* CONSTRUCTOR *******/
/***************************/

channel::channel(std::string name, user *creator)
    :_channel_name(name) {
    _list_channel_user.push_back(creator);
    _list_operator.push_back(creator);
    _key = "";
    _topic = "";
    _invite_only = false;
    _topic_settable = true;
    _channel_private = false;
};

channel::channel(std::string name, user *creator, std::string key)
    :_channel_name(name), _key(key) {
    _list_channel_user.push_back(creator);
    _list_operator.push_back(creator);
    _topic = "";
    _invite_only = false;
    _topic_settable = true;
    _channel_private = false;
};

/**************************/
/******* DESTRUCTOR *******/
/**************************/

channel::~channel() {
};


/**************/
/****  MOD ****/
/**************/

int channel::Add_user(user *use){
    // use pas deja present
    std::vector<user *>::iterator   it = _list_channel_user.begin();
    unsigned    i = 0;
    for (; it != _list_channel_user.end(); it++) { // Check user not already in channel
        if (_list_channel_user[i]->Get_fd_client() == use->Get_fd_client())
            return -1;
        i++;
    }
    _list_channel_user.push_back(use);
    return 0;
};

int channel::Remove_user(user *use){
    std::vector<user *>::iterator it;

    it = _list_channel_user.begin();
    size_t  i = 0;
    for (size_t i = 0; i < _list_channel_user.size(); i++) {
        if (_list_channel_user[i]->Get_fd_client() == use->Get_fd_client()) {
            _list_channel_user.erase(it);
            break;
        }
        it++;
    }

    it = _list_operator.begin();
    i = 0;
    // while (i < _list_operator.size())
    for (i = 0; i < _list_operator.size(); i++) {
        if (_list_operator[i]->Get_fd_client() == use->Get_fd_client()) {
            _list_operator.erase(it);
            break;
        }
        it++;
    }
    // ajouter oper si vide
    if (_list_operator.empty() && !_list_channel_user.empty())
        Add_oper(use, _list_channel_user.front());
    return 0;
};

void channel::Add_oper(user *sender, user *use)
{
    std::vector<user *> list = Get_list_channel_user();
    std::vector<user *>::iterator it;
    std::string msg;

    if (User_in_channel(use, this) && !User_in_channel_is_op(use, this))
    {
        _list_operator.push_back(use);
        for (it = list.begin(); it < list.end(); it++) { // previent mode +o le nouvel oper channel pour les membres
            msg = RPL_CHANNELNEWOP(sender->Get_nickname() + "!" + sender->Get_username() + "@" + sender->Get_hostname(), Get_channel_name(), "+o ",  use->Get_nickname());
            msg.append("\r\n");
            std::cout << (*it)->Get_fd_client() << " oper >> " << msg << std::endl;
            send((*it)->Get_fd_client(), msg.c_str(), msg.size(), 0);
        }
    }
}

void channel::Mod_topic(std::string str){
    _topic = str;
}

void channel::Set_channel_key(std::string key) {
    _key = key;
};

void channel::Set_invite_only(bool b) {
    _invite_only = b;
};

void channel::Set_topic_settable(bool b) {
    _topic_settable = b;
};

void channel::Set_channel_private(bool b) {
    _channel_private = b;
};

void    channel::Add_invited_user(user *use)
{
    for (size_t i = 0; i < _list_invited_user.size(); i++)
    {
        if (Compare_case_sensitive(use->Get_nickname(), _list_invited_user[i]->Get_nickname()))
            return;
    }
    _list_invited_user.push_back(use);
};

void    channel::Remove_invited_user(user *use) {
    std::vector<user *>::iterator   it = _list_invited_user.begin();

    for (; it < _list_invited_user.end(); it++) {
        if ((*it)->Get_nickname() == use->Get_nickname()) {
            _list_invited_user.erase(it);
            return ;
        }
    }
}


/************************/
/******* ACCESSOR *******/
/************************/

const std::string channel::Get_channel_name() const {
    return _channel_name;
};

const std::string   channel::Get_channel_key() const {
    return(_key);
};

const std::string   channel::Get_channel_topic() const {
    return(_topic);
};

std::vector<user *> channel::Get_list_channel_user() const {
    return _list_channel_user;
};

std::vector<user *> channel::Get_list_operator() const {
    return(_list_operator);
};

bool channel::Get_invite_only() const {
    return(_invite_only);
};

bool channel::Get_topic_settable() const {
    return(_topic_settable);
};

bool channel::Get_channel_private() const {
    return _channel_private;
};

std::string channel::Print_mode() const {
    std::string str = "+";
    if (_invite_only) {
        str += "i";
    }
    if (!_topic_settable) {
        str += "t";
    }
    if (_channel_private) {
        str += "k";
    }
    return str;
};

std::vector<user *> channel::Get_invited_user() const {
    return _list_invited_user;
};

bool    channel::_Is_invit(user *use) const {
    for (size_t i = 0; i < _list_invited_user.size(); i++)
        if (_list_invited_user[i]->Get_nickname() == use->Get_nickname())
            return true;
    return false;
};

void    channel::print_user_channel(){
    std::cout << "   user:" << _list_channel_user.size() << std::endl;
    for (size_t i = 0; i < _list_channel_user.size(); i++)
        std::cout << "      " << i << ": fd." << _list_channel_user[i]->Get_fd_client() << " " << _list_channel_user[i]->Get_nickname() << std::endl; 
};
void    channel::print_oper_channel(){
    std::cout << "   oper:" << std::endl;
    for (size_t i = 0; i < _list_operator.size(); i++)
        std::cout << "      " << i << ": fd." << _list_operator[i]->Get_fd_client() << " " << _list_operator[i]->Get_nickname() << std::endl; 
};
