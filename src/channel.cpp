/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiacomi <egiacomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 23:43:08 by egiacomi          #+#    #+#             */
/*   Updated: 2023/01/20 23:43:09 by egiacomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
};

channel::channel(std::string name, user *creator, std::string key)
    :_channel_name(name), _key(key) {
    _list_channel_user.push_back(creator);
    _list_operator.push_back(creator);
    _topic = "";
    _invite_only = false;
    _topic_settable = true;
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
    // ajouter oper si vide
    if (_list_operator.empty() && !_list_channel_user.empty())
        Add_oper(_list_channel_user.front());
    return 0;
};

void channel::Add_oper(user *use)
{
    if (User_in_channel(use, this) && !User_in_channel_is_op(use, this))
        _list_operator.push_back(use);
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
}

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
