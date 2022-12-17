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

