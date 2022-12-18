# include "../class/channel.hpp"

/***************************/
/******* CONSTRUCTOR *******/
/***************************/

channel::channel(std::string name, user *creator)
    :_channel_name(name) {
    _list_channel_user.push_back(creator);
    _list_operator.push_back(creator);
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
    for (; it != _list_channel_user.end(); it++) // Check user not already in channel
    {
        if (_list_channel_user[i]->Get_fd_client() == use->Get_fd_client())
        {
            return -1;
        }
        i++;
    }
    _list_channel_user.push_back(use);
    return 0;
};

int channel::Remove_user(user use){
    (void) use;
    return 0;
};

/************************/
/******* ACCESSOR *******/
/************************/

const std::string channel::Get_channel_name() const {
    return _channel_name;
};

std::vector<user *> channel::Get_list_channel_user() const {
    return _list_channel_user;
};

std::vector<user *> channel::Get_list_operator() const {
    return(_list_operator);
};

