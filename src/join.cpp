# include "../class/utils.hpp"

void    server::_Join_rpl(user *use, channel *chan){
    std::string names = Create_names_rpl(chan);
    std::string prefix = use->Get_nickname() + "!" + use->Get_username() + "@" + use->Get_hostname();

    // JOIN message tout membre chan y compris moi meme
    _Output_channel(chan, ":" + prefix + " JOIN " + ":" + chan->Get_channel_name());

    // Already exist channel
    if (chan->Get_list_channel_user().size() > 1) {
        if (!chan->Get_channel_topic().empty())// 332
            _Output_client(use->Get_fd_client(), RPL_TOPIC(_name_serveur, use->Get_nickname(), chan->Get_channel_name(), chan->Get_channel_topic()));
    }
    // new channel
    _Output_client(use->Get_fd_client(), RPL_NAMREPLY(_name_serveur, use->Get_nickname(), chan->Get_channel_name(), names));// 353
    _Output_client(use->Get_fd_client(), RPL_ENDOFNAMES(_name_serveur, use->Get_nickname(),chan->Get_channel_name()));// 366
}

int server::_Join_treat(user *user, std::vector<std::string> chan, std::vector<std::string> key)
{
    channel *res;

    while (!chan.empty())
    {
        if (key.empty()) {
            _Join_treat(user, chan);
            return 0;
        }
        if (Check_valid_channel(chan[0]))
        {
            if ((res = _Channel_already_exist(chan[0])) == NULL) { // new channel
                res = _Add_channel(chan[0], user, key[0]);
                user->Add_channel(res);
                _Join_rpl(user, res);
            }
            else { // channel already exist
                // check key + invit
                if (res->Get_channel_private() && key[0] != res->Get_channel_key()) // check if key needed and good key
                    _Output_client(user->Get_fd_client(), ERR_BADCHANNELKEY(_name_serveur, user->Get_nickname(), chan[0]));
                else if (res->Get_invite_only() && !res->_Is_invit(user)) // check invit only and not invited
                    _Output_client(user->Get_fd_client(), ERR_INVITEONLYCHAN(_name_serveur, user->Get_nickname(), res->Get_channel_name()));
                else if (!user->Is_user_channel(res)) // check user not in chan
                {
                    res->Remove_invited_user(user);
                    user->Add_channel(res);     // ajouter channel dans user
                    res->Add_user(user);        // ajouter user dans channel
                    _Join_rpl(user, res);       // Send Protocol
                }
            }
        }
        else
            _Output_client(user->Get_fd_client(), (ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), chan[0])));
        chan.erase(chan.begin());
        key.erase(key.begin());
    }
    return 0;
};

int server::_Join_treat(user *user, std::vector<std::string> chan)
{
    channel *res;

    for (size_t i = 0; i < chan.size(); i++) {
        if (Check_valid_channel(chan[i])) {
            if ((res = _Channel_already_exist(chan[i])) == NULL) { // channel a creer
                res = _Add_channel(chan[i], user);  // create new channel
                user->Add_channel(res);             // ajouter channel dans user
                _Join_rpl(user, res);               // Send Protocol
            }
            else { // channel already exist
                if (res->Get_channel_private()) // check si need key
                    _Output_client(user->Get_fd_client(), (ERR_BADCHANNELKEY(_name_serveur, user->Get_nickname(), chan[i])));
                else if (res->Get_invite_only() && !res->_Is_invit(user)) // check invit only and not invited
                    _Output_client(user->Get_fd_client(), ERR_INVITEONLYCHAN(_name_serveur, user->Get_nickname(), res->Get_channel_name()));
                else if (!user->Is_user_channel(res)) { // check invite + user n'est pas dedans
                    res->Remove_invited_user(user);
                    user->Add_channel(res);     // ajouter channel in user
                    res->Add_user(user);        // ajouter user in channel
                    _Join_rpl(user, res);       // Send Protocol
                }
            }
        }
        else
            _Output_client(user->Get_fd_client(), (ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), chan[i])));
    }
    return 0;
};

channel    *server::_Channel_already_exist(std::string str)
{
    std::vector<channel *>::iterator    it;

    it = _list_channel.begin();
    for (size_t  i = 0; i < _list_channel.size(); i++) {
        if (Compare_case_sensitive(str, _list_channel[i]->Get_channel_name()))
            return _list_channel[i];
    }
    return NULL;
}

int Check_valid_channel(std::string str){
    size_t  t = std::string::npos;
    // prefixe list '&', '#', '+' or '!'
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