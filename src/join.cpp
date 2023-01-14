# include "../class/utils.hpp"

/* utils command */

int server::_Join_treat(user *user, std::vector<std::string> chan, std::vector<std::string> key)
{
    std::cout << "_Join_treat key-chan" << std::endl;

    channel *res;

    while (!chan.empty())
    {
        if (key.empty())
        {
            _Join_treat(user, chan);
            return 0;
        }
        std::cout << "Channel name: " << chan[0] << std::endl;
        if (Check_valid_channel(chan[0]))
        {
            std::cout << "Valid channel name" << std::endl;

            if ((res = _Channel_already_exist(chan[0])) == NULL) // channel a creer
            {
                std::cout << "Already exist channel" << std::endl;
                res = _Add_channel(chan[0], user, key[0]);
                user->Add_channel(res);
            }
            else // channel existe
            {
                std::cout << "Create new channel" << std::endl;
                // check key
                if (key[0] == res->Get_channel_key() || res->Get_channel_key().empty())
                {
                    user->Add_channel(res); // ajouter channel dans user
                    res->Add_user(user); // ajouter user dans channel
                }
                else
                    _Output_client(user->Get_fd_client(), (ERR_BADCHANNELKEY(_name_serveur, chan[0])));

            }
        }
        else
        {
            std::cout << "Unvalid channel name" << std::endl;
            _Output_client(user->Get_fd_client(), (ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), chan[0])));
        }
        chan.erase(chan.begin());
        key.erase(key.begin());
    }
    std::cout << "size _list_channel: " << _list_channel.size() << std::endl;
    std::cout << "size _list_user   : " << _list_user.size() << std::endl;
    return 0;
};

int server::_Join_treat(user *user, std::vector<std::string> chan)
{
    std::cout << "_Join_treat chan" << std::endl;
    
    //size_t  pos;
    channel *res;

    for (size_t i = 0; i < chan.size(); i++)
    {
        std::cout << "Channel name: " << chan[i] << std::endl;
        if (Check_valid_channel(chan[i]))
        {
            std::cout << "Valid channel name" << std::endl;

            if ((res = _Channel_already_exist(chan[i])) == NULL) // channel a creer
            {
                std::cout << "Already exist channel" << std::endl;
                res = _Add_channel(chan[i], user);
                user->Add_channel(res);
            }
            else // channel existe
            {
                std::cout << "Create new channel" << std::endl;
                if (res->Get_channel_key().empty()){
                    user->Add_channel(res); // ajouter channel dans user
                    res->Add_user(user); // ajouter user dans channel
                }
                else
                    _Output_client(user->Get_fd_client(), (ERR_BADCHANNELKEY(_name_serveur, chan[i])));
            }
        }
        else
        {
            std::cout << "Unvalid channel name" << std::endl;
            _Output_client(user->Get_fd_client(), (ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), chan[i])));
        }
    }
    std::cout << "size _list_channel: " << _list_channel.size() << std::endl;
    std::cout << "size _list_user   : " << _list_user.size() << std::endl;
    return 0;
};

channel    *server::_Channel_already_exist(std::string str)
{
    std::vector<channel *>::iterator    it;

    it = _list_channel.begin();
    for (size_t  i = 0; i < _list_channel.size(); i++)
    {
        if (Compare_case_sensitive(str, _list_channel[i]->Get_channel_name()))
            return _list_channel[i];
    }
    return NULL;
}

int Check_valid_channel(std::string str){
    std::cout << "Check_valid_channel: " << str << "|" << std::endl;
    size_t  t = std::string::npos;
    // '&', '#', '+' or '!'
    // lenght 50 max
    // no scape ctrl+g '7' ','
    // TotO == toto
    if (str.empty())
        return (0);
    if (str.size() > 49)
        return (0);
    if ((str.find("\a", 0) != t) || str.find(" ", 0) != t || str.find(",", 0) != t)
        return 0;
    if (((str[0] == '&') || (str[0] == '#') || (str[0] == '+') || (str[0] == '!')))
        return 1;
    return 0;
};