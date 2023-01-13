# include "../class/utils.hpp"

/* utils command */

int server::_Join_treat(user *use, std::string chan, std::string key)
{
    std::cout << "_Join_treat chan" << std::endl;

    (void) use;
    (void) chan;
    
    std::string str_chan;
    std::string str_key;
    channel *res;
    (void) res;

    // ex chan:  #qwe,#qwer,,,,&qweasd
    // get chan -> check ok ->1 traiter
    //                      ->2 gonext

    std::cout << "pre chan =" << chan<< std::endl;

    str_chan = Next_arg(chan);
    chan.erase(0, str_chan.size());
    if (chan[0] == ',')
        chan.erase(0, 1);
    str_key = Next_arg(key);
    key.erase(0, str_key.size());
    if (key[0] == ',')
        key.erase(0, 1);
    // std::cout << "first str_chan =" << str_chan<< std::endl;
    // std::cout << "first chan =" << chan<< std::endl;
    while (!str_chan.empty() && !str_key.empty())
    {
        std::cout << "verifier si " << str_chan << " is okay "<< std::endl;
        if (Check_valid_channel(str_chan))
            _Join_add(use, str_chan, str_key);
        else
            _Output_client(use->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, chan));
        str_chan = Next_arg(chan);
        chan.erase(0, str_chan.size());
        if (chan[0] == ',')
            chan.erase(0, 1);
        str_key = Next_arg(key);
        key.erase(0, str_key.size());
        if (key[0] == ',')
            key.erase(0, 1);
        std::cout << "str_chan =" << str_chan<< std::endl;
        std::cout << "chan =" << chan<< std::endl;
    }
    if (!str_chan.empty())
    {
        if (Check_valid_channel(str_chan))
            _Join_add(use, str_chan, str_key);
        else
            _Output_client(use->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, chan));
        _Join_treat(use, chan);
    }
    return 0;
};

int server::_Join_treat(user *use, std::string chan)
{
    std::cout << "_Join_treat chan" << std::endl;

    (void) use;
    (void) chan;
    
    std::string str_chan;
    channel *res;
    (void) res;

    // ex chan:  #qwe,#qwer,,,,&qweasd
    // get chan -> check ok ->1 traiter
    //                      ->2 gonext

    std::cout << "pre chan =" << chan<< std::endl;

    str_chan = Next_arg(chan);
    chan.erase(0, str_chan.size());
    if (chan[0] == ',')
        chan.erase(0, 1);
    std::cout << "first str_chan =" << str_chan<< std::endl;
    std::cout << "first chan =" << chan<< std::endl;
    while (!str_chan.empty())
    {
        std::cout << "_Join_treat chan ALPHA" << std::endl;
        if (str[0] == ',')
        {
            _Output_client(user->Get_fd_client(), (ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), chan)));
        }
        else
        {
            std::cout << "_Join_treat chan BRAVO" << std::endl;
            str = chan.substr(0, pos);
            if (Check_valid_channel(str))
            {
                std::cout << "channel valide" << std::cout;
                if ((res = _Channel_already_exist(str)) == NULL)
                {
                    res = _Add_channel(str, user);
                    user->Add_channel(res);
                }
                else
                {
                    user->Add_channel(res); // ajouter channel dans user
                    res->Add_user(user); // ajouter user dans channel
                }
            }
            else
                _Output_client(user->Get_fd_client(), (ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), chan)));
        }
        chan.erase(0, pos + 1);
        std::cout << chan << std::endl;
    }
    return 0;
};

int server::_Join_add(user *use, std::string chan, std::string key){
    channel *res;

    if ((res = _Channel_already_exist(chan)) != NULL)
    {
        std::cout << "_Join_treat chan already exist" << std::endl;
        if (res->Get_channel_key().empty() || res->Get_channel_key() == key)
        {
            res->Add_user(use);
            use->Add_channel(res);
        }
        else
           _Output_client(use->Get_fd_client(), ERR_BADCHANNELKEY(_name_serveur, res->Get_channel_name()));
    }
    else
    {
        std::cout << "***********************8unvalid channel name" << std::endl;
        _Output_client(user->Get_fd_client(), (ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), chan)));
    }
    return (0);
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

std::string Next_arg(std::string str)
{
    size_t  pos;

    std::string res;

    if ((pos = str.find(",", 0)) == std::string::npos) // pas de ,
    {
        res = str.substr(0, str.size());
        str.clear();
        return (res);
    }
    res = str.substr(0, pos);
    std::cout << "Next_arg res= " << res << std::endl;
    if (str[0] == ',')
    {
        //str.erase(0, pos);
        std::cout << "Next_arg str= " << str << std::endl;
        return res;
    }
    //str.erase(0, pos + 1);
    std::cout << "Next_arg str= " << str << std::endl;
    return (res);
};


int Check_valid_channel(std::string str){
    std::cout << "Check_valid_channel: " << str << "|" << std::endl;
    size_t  t = std::string::npos;
    // '&', '#', '+' or '!'
    // lenght 50 max
    // no scape ctrl+g '7' ','
    // TotO == toto
    if (str.empty() || str.size() > 49 || (str.find("\a", 0) != t) || str.find(" ", 0) != t || str.find(",", 0) != t)
        return 0;
    if (((str[0] == '&') || (str[0] == '#') || (str[0] == '+') || (str[0] == '!')))
        return 1;
    return 0;
};