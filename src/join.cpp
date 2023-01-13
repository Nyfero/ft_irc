# include "../class/utils.hpp"

/* utils command */

int server::_Join_treat(user *user, std::string chan, std::string key)
{
    (void) user;
    (void) chan;
    (void) key;

    // verifier tout comme celui en bas mais avant d'ajouter verifier que key est bonne
    return 0;
};

int server::_Join_treat(user *user, std::string chan)
{
    std::cout << "_Join_treat chan" << std::endl;

    (void) user;
    (void) chan;
    
    std::string str = chan;
    size_t  pos;
    channel *res;


    while ((pos = chan.find(",", 0)) != std::string::npos) // check si d'autre channel
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
    if (!chan.empty() && Check_valid_channel(chan)) // dernier/seul channel
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
    {
        std::cout << "***********************8unvalid channel name" << std::endl;
        _Output_client(user->Get_fd_client(), (ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), chan)));
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
    if (str.empty() || str.size() > 49 || (str.find("\a", 0) != t) || str.find(" ", 0) != t || str.find(",", 0) != t)
        return 0;
    if (((str[0] == '&') || (str[0] == '#') || (str[0] == '+') || (str[0] == '!')))
        return 1;
    return 0;
};