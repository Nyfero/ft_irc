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

int channel::Remove_user(user *use){
    std::vector<user *>::iterator it;
    // if ((it = User_in_channel(use, this)) != _list_channel_user.end())
    //     _list_channel_user.erase(it);
    
    // if ((it = User_in_channel_is_op(use, this)) != _list_operator.end())
    //     _list_operator.erase(it);


    it = _list_channel_user.begin();
    size_t  i = 0;
    while (i < _list_channel_user.size()){
        if (_list_channel_user[i]->Get_fd_client() == use->Get_fd_client())
        {
            _list_channel_user.erase(it);
            break;
        }
        i++;
        it++;
    }

    it = _list_operator.begin();
    i = 0;
    while (i < _list_operator.size()){
        if (_list_operator[i]->Get_fd_client() == use->Get_fd_client())
        {
            _list_operator.erase(it);
            break;
        }
        i++;
        it++;
    }
        
    return 0;
};

int channel::Add_oper(user *use)
{
    if (User_in_channel(use, this) && !User_in_channel_is_op(use, this))
        _list_operator.push_back(use);
}

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
