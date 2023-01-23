# include "../class/utils.hpp"

void    server::_Join_rpl(user *use, channel *chan){
    std::string names = Create_names_rpl(chan);

    std::string prefix = use->Get_nickname() + "!" + use->Get_username() + "@" + use->Get_hostname();

    // JOIn message tout membre chan y compris moi meme
    _Output_channel(chan, ":" + prefix + " JOIN " + ":" + chan->Get_channel_name());

    if (chan->Get_list_channel_user().size() > 1) {// deja existant
        if (!chan->Get_channel_topic().empty())// 332
            _Output_client(use->Get_fd_client(), RPL_TOPIC(_name_serveur, use->Get_nickname(), chan->Get_channel_name(), chan->Get_channel_topic()));
    }
    // new channel
    _Output_client(use->Get_fd_client(), RPL_NAMREPLY(_name_serveur, use->Get_nickname(), chan->Get_channel_name(), names));// 353
    _Output_client(use->Get_fd_client(), RPL_ENDOFNAMES(_name_serveur, use->Get_nickname(),chan->Get_channel_name()));// 366
}

//to-do
//  chan n'est pas invit only ->ERR_INVITEONLYCHAN

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
                std::cout << "Create new channel" << std::endl;
                res = _Add_channel(chan[0], user, key[0]);
                user->Add_channel(res);
                _Join_rpl(user, res);
            }
            else // channel existe
            {
                std::cout << "Create new channel" << std::endl;
                std::cout << "Already exist channel" << std::endl;
                // check key + invit
                if (!res->Get_channel_key().empty() && key[0] != res->Get_channel_key()) // check if key needed and good key
                    _Output_client(user->Get_fd_client(), ERR_BADCHANNELKEY(_name_serveur, user->Get_nickname(), chan[0]));
                else if (!user->Is_user_channel(res)) // check user not in chan && invited
                {
                    user->Add_channel(res); // ajouter channel dans user
                    res->Add_user(user); // ajouter user dans channel
                    _Join_rpl(user, res);
                }
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
                std::cout << "Create new channel" << std::endl;
                res = _Add_channel(chan[i], user);
                user->Add_channel(res);
                _Join_rpl(user, res);
            }
            else // channel existe
            {
                std::cout << "Already exist channel" << std::endl;
                if (!res->Get_channel_key().empty()) // check si need key
                    _Output_client(user->Get_fd_client(), (ERR_BADCHANNELKEY(_name_serveur, user->Get_nickname(), chan[i])));
                else if (!user->Is_user_channel(res)) { // check invite + user n'est pas dedans
                    user->Add_channel(res); // ajouter channel dans user
                    res->Add_user(user); // ajouter user dans channel
                    _Join_rpl(user, res);
                }
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