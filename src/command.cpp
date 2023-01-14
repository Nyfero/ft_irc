#include "../class/server.hpp"
#include "../class/utils.hpp"
#include "../class/user.hpp"

int server::Check_command(user *user, std::string str)
{
    std::cout << "Check_command: " << str << std::endl;

    t_IRCMessage msg = split_message(str);

    std::cout << "command: " << msg.command << std::endl;
    std::cout << "prefix: " << msg.prefix << std::endl;
    for (size_t i = 0; i < msg.params.size(); i++) {
        std::cout << "params[" << i << "]: " << msg.params[i] << std::endl;
    }

    std::string list_command[16] = {"PASS", "USER", "NICK", "MODE", "QUIT", "JOIN", "PART", "NAMES", "INVITE", "KICK", "PRIVMSG", "NOTICE", "AWAY", "USERS", "WALLOPS", "PING"};

    int i = 0;
    while (i < 15)
    {
        if (msg.command == list_command[i])
        {
            break;
        }
        i++;
    }
    switch (i)
    {
    case 0:
        Pass_cmd(user, msg);
        return 0;
    case 1:
        User_cmd(user, msg);
        return 0;
    case 2:
        Nick_cmd(user, msg);
        return 0;
    case 3:
        Mode_cmd(user, msg);
        return 0;
    case 4:
        return (Quit_cmd(user, msg));
    case 5:
        Join_cmd(user, msg);
        return 0;
    case 6:
        Part_cmd(user, msg);
        return 0;
    case 7:
        Names_cmd(user, msg);
        return 0;
    case 8:
        Invite_cmd(user, msg);
        return 0;
    case 9:
        Kick_cmd(user, msg);
        return 0;
    case 10:
        Privmsg_cmd(user, msg);
        return 0;
    case 11:
        Notice_cmd(user, msg);
        return 0;
    case 12:
        Away_cmd(user, msg);
        return 0;
    case 13:
        Users_cmd(user, msg);
        return 0;
    case 14:
        Wallops_cmd(user, msg);
        return 0;
    case 15:
        Pong_cmd(user, msg);
        return 0;
    default:
        std::cout << "*** server::Check_command - ***" << std::endl;
        break;
    }
    return 0
;};

// NON UTILISE
// bool    server::Check_prefix(user *user, std::string str) {

//     // Verifie si le user est deja connecte
//     if (user->Get_username().empty()) {
//         _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
//         return false;
//     }

//     // Verifie si le prefix correspond au nickname du user
//     std::string prefix = str.substr(1, str.find(" ") - 1);
//     std::string check_nick = prefix.substr(0, prefix.find("!"));
//     if (check_nick != user->Get_nickname()) {
//         _Output_client(user->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur, check_nick));
//         return false;
//     }

//     // Verifie si le prefix contient un ! apres le nickname
//     size_t pos = prefix.find("!");
//     if (pos == std::string::npos) {
//         if (pos + 1 == 0) {
//             return true;
//         }
//         _Output_client(user->Get_fd_client(), ERR_PARSINGPREFIX);
//         return false;
//     }

//     // Verifie si le prefix contient un @ apres le username
//     size_t pos2 = prefix.find("@");
//     if (pos2 == std::string::npos) {
//         _Output_client(user->Get_fd_client(), ERR_PARSINGPREFIX);
//         return false;
//     }

//     // Verifie si le prefix correspond au username du user
//     std::string check_user = prefix.substr(pos + 1, pos2 - pos - 1);
//     if (check_user != user->Get_username()) {
//         _Output_client(user->Get_fd_client(), ERR_NOSUCHUSER);
//         return false;
//     }

//     // Verifie si le prefix correspond au hostname du user
//     std::string check_host = prefix.substr(pos2 + 1, prefix.size());
//     if (check_host != user->Get_hostname()) {
//         _Output_client(user->Get_fd_client(), ERR_NOSUCHHOSTNAME);
//         return false;
//     }

//     return true;
// };

void server::Pass_cmd(user *user, t_IRCMessage cmd) {

    // Verifie les arguments de PASS
    if (cmd.params.empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PASS"));
        return;
    }

    if (user->Get_is_register()) { // Verifie si le user est deja connecte
        _Output_client(user->Get_fd_client(), ERR_ALREADYREGISTRED(_name_serveur));
    }
    else if (cmd.params[0] != _password) { // Verifie le mot de passe
        _Output_client(user->Get_fd_client(), ERR_PASSWDMISMATCH(_name_serveur));
    }
    else { // Connecte le user et envoie un message de bienvenue
        std::cout << "New user connected on " << user->Get_fd_client() << std::endl;
        _Output_client(user->Get_fd_client(), "Welcome to the IRC server");
        user->Set_is_register(true);
    }
};

void server::User_cmd(user *user, t_IRCMessage cmd) {

    // Verifie si le user est connecte
    if (!user->Get_is_register() || !user->Get_username().empty()) {
        _Output_client(user->Get_fd_client(), ERR_ALREADYREGISTRED(_name_serveur));
        return;
    }

    // Verifie les arguments de USER
    if (cmd.params.size() < 4) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "USER"));
        return;
    }

    // Verifie si le username est vide
    if (cmd.params[0].empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "USER"));
        return;
    }

    // Verifie le mode

    if (cmd.params[1].empty() || cmd.params[1].at(0) == ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "USER"));
        return;
    }

    // Verifie le hostname
    if (cmd.params[2].empty() || cmd.params[2].at(0) == ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "USER"));
        return;
    }

    // Verifie le realname
    if (cmd.params[3].empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "USER"));
        return;
    }
    if (cmd.params[3].at(0) != ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "USER"));
        return;
    }
    std::string realname = Join(cmd.params, 3, cmd.params.end() - cmd.params.begin());
    realname = realname.substr(1, realname.size());

    // Enregistre le user
    user->Set_username(cmd.params[0]);
    user->Set_hostname(cmd.params[2]);
    user->Set_realname(realname);
    if (!isNumber(cmd.params[1])) {
        user->Set_mode("0");
    }
    else {
        user->Set_mode(cmd.params[1]);
    }

    _Output_client(user->Get_fd_client(), RPL_WELCOME(_name_serveur, user->Get_realname(), user->Get_username(), user->Get_hostname()));
};

void server::Nick_cmd(user *user, t_IRCMessage cmd) {

    // Verifie les arguments de NICK
    if (cmd.params[0].empty()) {
        _Output_client(user->Get_fd_client(), ERR_NONICKNAMEGIVEN(_name_serveur));
        return;
    }

    // Verifie la longueur du nickname
    if (cmd.params[0].length() > 9 || cmd.params[0].length() < 1) {
        _Output_client(user->Get_fd_client(), ERR_ERRONEUSNICKNAME(_name_serveur, user->Get_nickname()));
        return;
    }

    // Verifie si le nick contient des caracteres speciaux
    for (size_t i = 0; i < cmd.params[0].length(); i++) {
        if (!isalnum(cmd.params[0][i]) && cmd.params[0][i] != '-' && cmd.params[0][i] != '_') {
            _Output_client(user->Get_fd_client(), ERR_ERRONEUSNICKNAME(_name_serveur, user->Get_nickname()));
            return;
        }
    }

    // Verifie si le nick est deja pris
    for (size_t i = 0; i < _list_user.size(); i++) {
        if (Compare_case_sensitive(_list_user[i]->Get_nickname(), cmd.params[0])) {
            _Output_client(user->Get_fd_client(), ERR_NICKNAMEINUSE(_name_serveur, user->Get_nickname()));
            return;
        }
    }

    if (!user->Get_is_register()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
    }
    else if (!user->Get_username().empty()) { // Modifie le nickname
        user->Set_nickname(cmd.params[0]);
        _Output_client(user->Get_fd_client(), "Your nickname is now " + cmd.params[0]);
    }
    else { // Definit le nickname
        user->Set_nickname(cmd.params[0]);
        std::cout << "Introducing new " << cmd.params[0] << " user" << std::endl;
        _Output_client(user->Get_fd_client(), "Hello " + cmd.params[0] + " !");
    }
};

void server::Mode_cmd(user *user, t_IRCMessage cmd) {

    // Verifie que le user est enregistre
    if (user->Get_username().empty()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
        return;
    }

    // Verifie les arguments de MODE
    if (cmd.params[0].empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "MODE"));
        return;
    }

    // Verifie que le premier parametre est le nickname ou le realname 

    if (cmd.params[0] != user->Get_nickname() && cmd.params[0] != user->Get_realname().substr(0, user->Get_realname().find(' '))) {
        _Output_client(user->Get_fd_client(), ERR_USERSDONTMATCH(_name_serveur));
        return;
    }

    if (cmd.params.size() == 1) {
        _Output_client(user->Get_fd_client(), user->Get_mode().Print_mode());
    }
    else {
        if (cmd.params[0] == user->Get_nickname() || cmd.params[0] != user->Get_realname().substr(0, user->Get_realname().find(' '))) {
            if (cmd.params[1].at(0) == '+') {
                for (size_t i = 1; i < cmd.params[1].size(); i++) {
                    if (user->Get_mode().Add_mode(cmd.params[1].at(i))) {
                        _Output_client(user->Get_fd_client(), ERR_UMODEUNKNOWNFLAG(_name_serveur));
                    }
                }
                _Output_client(user->Get_fd_client(), user->Get_mode().Print_mode());
            }
            else {
                for (size_t i = 1; i < cmd.params[1].size(); i++) {
                    if (user->Get_mode().Remove_mode(cmd.params[1].at(i))) {
                        _Output_client(user->Get_fd_client(), ERR_UMODEUNKNOWNFLAG(_name_serveur));
                    }
                }
                _Output_client(user->Get_fd_client(), user->Get_mode().Print_mode());
            }
        }
    }
};

int server::Quit_cmd(user *user, t_IRCMessage cmd)
{ // jgour
    std::cout << "COMMANDE -> QUIT" << std::endl;
    (void)cmd;
    (void)user;
    return -2;
};

void server::Join_cmd(user *user, t_IRCMessage cmd)
{ // jgour
    std::cout << "COMMANDE -> JOIN" << std::endl;

        // ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
        // ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
        // ERR_CHANNELISFULL               ERR_BADCHANMASK
        // ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
        // ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
        // RPL_TOPIC

    (void)user;
    (void)cmd;
    std::vector<std::string>    chan;
    std::vector<std::string>    key;

    if (cmd.params.empty()) { //vide
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, user->Get_nickname()));
        return;
    }
    else if (cmd.params.size() == 1 && cmd.params[0] == "0") {
        // join 0
    }
    else if (cmd.params.size() == 1) {
        // join channel
        chan = Split(cmd.params[0], ',');
        _Join_treat(user, chan);

    }
    else if (cmd.params.size() >= 2) {
        // join channel key
        chan = Split(cmd.params[0], ',');
        key = Split(cmd.params[1], ',');
        _Join_treat(user, chan, key);

    }
    //  check param < 3 ?

};

void server::Part_cmd(user *user, t_IRCMessage cmd)
{ // gjour
    std::cout << "COMMANDE -> PART" << std::endl;
    (void)cmd;
    (void)user;
    std::vector<std::string> v_chan;
    channel *chan;

    // ERR_NEEDMOREPARAMS
    // ERR_NOSUCHCHANNEL
    // ERR_NOTONCHANNEL

    // <channel> *( "," <channel> ) [ <Part Message> ]

    // PART #oz-ops,&group5 
    // PART #playzone :I lost
    if (cmd.params.empty()){
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, user->Get_nickname()));
    }
    else if (cmd.params.size() == 1) // NO MSG
    {
        v_chan = Split(cmd.params[0], ',');
        while (!v_chan.empty())
        {
            if ((chan = _Channel_already_exist(v_chan[0])) == NULL)
            {
                _Output_client(user->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), v_chan[0]));
            }
            else if (User_in_channel(user, chan))
            {
                user->Remove_Channel(chan);
                chan->Remove_user(user);
                _Output_client(user->Get_fd_client(), "PART " + chan->Get_channel_name()+ " : bye bye");
                if (chan->Get_list_channel_user().empty()) {
                // si chan vide degager
                    _Remove_channel(chan);
                }
            }
            else
            {
                 _Output_client(user->Get_fd_client(), ERR_NOTONCHANNEL(_name_serveur, v_chan[0]));
            }
            v_chan.erase(v_chan.begin());
        }
    }
    else if (cmd.params.size() >= 2) // MSG
    {
        // check no channel list
    }

    // Servers MUST be able to parse arguments in the form of a list of
    // target, but SHOULD NOT use lists when sending PART messages to
    // clients.
};

void server::Names_cmd(user *user, t_IRCMessage cmd)
{
    std::cout << "COMMANDE -> NAMES" << std::endl;
    (void)cmd;
    (void)user;
};

void server::Invite_cmd(user *user, t_IRCMessage cmd)
{ // jgoru
    std::cout << "COMMANDE -> INVITE" << std::endl;
    (void)cmd;
    (void)user;
};

void server::Kick_cmd(user *user, t_IRCMessage cmd)
{ // jgour
    std::cout << "COMMANDE -> KICK" << std::endl;
    (void)cmd;
    (void)user;
};

/* TODO :
  - Check if User is restricted/muted in the channel -> ERR_CANNOTSENDTOCHAN
  - Handle wildcard and send message to mask with a wild-card for a top level domain + ERR_WILDTOPLEVEL + ERR_NOTOPLEVEL
  - Handle ERR_TOOMANYTARGETS (how could this happen actually ?)
  - Improve message sent back to user
  - Handle AWAY response if user mode a
*/
void server::Privmsg_cmd(user *sender, t_IRCMessage cmd)
{
    (void)cmd;
    (void)sender;
    std::cout << "COMMANDE -> PRIVMSG" << std::endl;
    (void)sender;

    /*                       Check si il y a des arguments                       */
    if (cmd.params.empty())
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return;
    }

    /*                                        Handle Targets                                        */
    // Check if there are any target specified
    if (cmd.params[0][0] == ':')
    {
        _Output_client(sender->Get_fd_client(), ERR_NORECIPIENT(_name_serveur));
        return;
    }

    // Create vector of targets
    std::vector<std::string> target;
    target.push_back(cmd.params[0]);

    std::stringstream targstream(target[0]);
    std::string newtarget;
    while (std::getline(targstream, newtarget, ','))
    {
        target.push_back(newtarget);
    }
    target.erase(target.begin());

// TODO : Add other prefixes
    /*                        Check if targets are valid & add to fd to send                        */
    std::vector<int> targets_fds;
    for (std::vector<std::string>::iterator it = target.begin(); it != target.end(); ++it)
    {

        // CHECK if target is a Channel
        if ((*it)[0] == '#')
        {
            // Check if channel Exist
            if (_list_channel.empty())
            {
                // FIXME : check if is good message error              
                _Output_client(sender->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, sender->Get_nickname(), *it));
                return;
            }
            for (size_t i = 0; i < _list_channel.size(); i++)
            {
                std::string target_channel = _list_channel[i]->Get_channel_name();
                if (Compare_case_sensitive(target_channel, *it))
                {
                    // Check is user is in the channel
                    if (User_in_channel(sender, _list_channel[i]))
                    {
                        std::vector<user *> channel_users = _list_channel[i]->Get_list_channel_user();

                        // Add all channel_users_fd
                        for (size_t i = 0; i < channel_users.size(); i++)
                                targets_fds.push_back(channel_users[i]->Get_fd_client());
                    }
                    else
                    {
                        std::string chan_name = _list_channel[i]->Get_channel_name();
                        _Output_client(sender->Get_fd_client(), ERR_CANNOTSENDTOCHAN(_name_serveur, chan_name));
                        return;
                    }
                }
                else if (i + 1 == _list_channel.size())
                {
                    _Output_client(sender->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, sender->Get_nickname(), *it));
                    return;
                }
            }
        }
        else
        {
            // Check if target is user
            for (size_t i = 0; i < _list_user.size(); i++)
            {

                // Check if username exists
                if (Compare_case_sensitive(_list_user[i]->Get_nickname(), *it))
                    // Add user_fd
                    targets_fds.push_back(_list_user[i]->Get_fd_client());
                else if (i + 1 == _list_user.size())
                {
                    _Output_client(sender->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur, *it));
                    return;
                }
            }
        }
    }

    /*                             Parse Message (exist, lenght, prefix?)                             */
    // Check if a message is specified
    if (cmd.params.size() == 1)
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return;
    }

    // merge all params in one message if more than 1 word
    std::string message = cmd.params[1];
    for (size_t i = 2; i < cmd.params.size(); i++) {
        message += " " + cmd.params[i];
    }
    // Check if message start with ':' and delete ":"
    if (message[0] != ':')
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return;
    }
    message = message.substr(1, message.size());

    /*                                           Send message                                           */
    std::vector<int>::iterator dest;
    for (dest = targets_fds.begin(); dest != targets_fds.end(); dest++)
        _Output_client(*dest, message);

    // Notify message well sent
    _Output_client(sender->Get_fd_client(), "Message has been successfully sent");
};

void server::Notice_cmd(user *user, t_IRCMessage cmd)
{
    (void) user;
    (void) cmd;
    // std::cout << "Notice_cmd" << std::endl;

    // // Verifie que le user est enregistre
    // if // std::cout << "Notice_cmd" << std::endl;

    // // Verifie que le user est enregistre
    // if (user->Get_username().empty())
    // {
    //     return;
    // }

    // // Verifie les arguments de NOTICE
    // size_t pos = cmd.find_first_not_of(" ", 6);
    // if (pos == std::string::npos)
    // {
    //     return;
    // }

    // // Recupere les destinataires
    // std::string dest = cmd.substr(pos, cmd.find(" ", pos) - pos);
    // std::user->Get_username().empty())
    // {
    //     return;
    // }

    // // Verifie les arguments de NOTICE
    // size_t pos = cmd.find_first_not_of(" ", 6);
    // if (pos == std::string::npos)
    // {
    //     return;
    // }

    // // Recupere les destinataires
    // std::string dest = cmd.substr(pos, cmd.find(" ", pos) - pos);
    // std::vectector<std::strr<std::string> target;
    // for (size_t i = 0; i < ng> target;
    // for (size_t i = 0; i < dest.size(est.size();; i++)
    // {
    //     if (dest[i] == ',')
    //     {
    //         for (size_t j = 0; j < target.size(); j++)
    //         {
    //             if (target[j] == dest.si++)
    // {
    //     if (dest[i] == ',')
    //     {
    //         for (size_t j = 0; j < target.size(); j++)
    //         {
    //             if (target[j] == dest.subbstr(0, i))
    //             {
    //                 dtr(0, i))
    //             {
    //                 dest = dest.substst = dest.substr(i + 1, dest.size() - i)(i + 1, dest.size() - i);
    //             }
    //             else
    //             {
    //                 ta//             }
    //             else
    //             {
    //                 targget.p.push_back(dest.substsh_back(dest.substr(0, i));
    //                 dest = dest.substr(i + 1, dest.size() - i);
    //             }
    //             i = 0;
    //         }
    //     }
    // }
    // target.push_back(dest);

    // // Recupere le message
    // size_t strt_omsg = cmd.fi(0, i));
    //                 dest = dest.substr(i + 1, dest.size() - i);
    //             }
    //             i = 0;
    //         }
    //     }
    // }
    // target.push_back(dest);

    // // Recupere le message
    // size_t strt_omsg = cmd.find_first_not_of("d_first_not_of(" ", cmd.find(" ", pos));
    // if (strt_omsg == std::string::npos)
    // {
    //     return;
    // }
    // std::string msg = cmd.substr(strt_omsg, cmd.size() - strt_omsg);
    // if (msg[", cmd.find(" ", pos));
    // if (strt_omsg == std::string::npos)
    // {
    //     return;
    // }
    // std::string msg = cmd.substr(strt_omsg, cmd.size() - strt_omsg);
    // if (msg[0] != ':')
    // {
    //     return] != ':')
    // {
    //     return;
    // }

    // // Envoie le message
    // for (size_t j = 0; j < target.size(); j++)
    // {
    //     dest = target[j];
    //     std::string real_msg = ":" + user->Get_nickname() + "!" + user->Get_username() + "@" + user->Get_hostname() + " :NOTICE " + dest + " " + msg;
    //     std::cout << "real_msg = " << real_msg << std::endl;
    //     if (dest[0] == '#')
    //     {
    //         // Si le destinataire est un channel
    //         for (size_t i = 0; i < _list_channel.size(); i++)
    //         {
    //             if (Compare_case_sensitive(_list_channel[i]->Get_channel_name(), dest))
    //             {
    //                 // Verifie que le user est dans le channel (sinon, il ne peut pas envoyer de message)
    //                 if (!User_in_channel(user, _list_channel[i]))
    //                 {
    //                     ;
    //                 }
    //                 // Verifie que le user n'est pas restreint (flag +r)
    //                 else if (Get_user_in_channel(user, _list_channel[i])->Get_mode().Get_restricted())
    //                 {
    //                     ;
    //                 }
    //                 // Envoie le message
    //                 else
    //                 {
    //                     _Output_channel(_list_channel[i], msg);
    //                 }
    //             }
    //         }
    //     }
    //     else
    //     {
    //         // Si le destinataire est un user
    //         for (size_t i = 0; i < _list_user.size(); i++)
    //         {
    //             if (Compare_case_sensitive(_list_user[i]->Get_nickname(), dest))
    //             {
    //                 // Envoie le message
    //                 std::cout << _list_user[i]->Get_nickname() << std::endl;
    //                 std::cout << _list_user[i]->Get_fd_client() << std::endl;
    //                 _Output_client(_list_user[i]->Get_fd_client(), msg);
    //             }
    //         }
    //     }
    // }
    // }

    // // Envoie le message
    // for (size_t j = 0; j < target.size(); j++)
    // {
    //     dest = target[j];
    //     std::string real_msg = ":" + user->Get_nickname() + "!" + user->Get_username() + "@" + user->Get_hostname() + " :NOTICE " + dest + " " + msg;
    //     std::cout << "real_msg = " << real_msg << std::endl;
    //     if (dest[0] == '#')
    //     {
    //         // Si le destinataire est un channel
    //         for (size_t i = 0; i < _list_channel.size(); i++)
    //         {
    //             if (Compare_case_sensitive(_list_channel[i]->Get_channel_name(), dest))
    //             {
    //                 // Verifie que le user est dans le channel (sinon, il ne peut pas envoyer de message)
    //                 if (!User_in_channel(user, _list_channel[i]))
    //                 {
    //                     ;
    //                 }
    //                 // Verifie que le user n'est pas restreint (flag +r)
    //                 else if (Get_user_in_channel(user, _list_channel[i])->Get_mode().Get_restricted())
    //                 {
    //                     ;
    //                 }
    //                 // Envoie le message
    //                 else
    //                 {
    //                     _Output_channel(_list_channel[i], msg);
    //                 }
    //             }
    //         }
    //     }
    //     else
    //     {
    //         // Si le destinataire est un user
    //         for (size_t i = 0; i < _list_user.size(); i++)
    //         {
    //             if (Compare_case_sensitive(_list_user[i]->Get_nickname(), dest))
    //             {
    //                 // Envoie le message
    //                 std::cout << _list_user[i]->Get_nickname() << std::endl;
    //                 std::cout << _list_user[i]->Get_fd_client() << std::endl;
    //                 _Output_client(_list_user[i]->Get_fd_client(), msg);
    //             }
    //         }
    //     }
    // }
};

void server::Away_cmd(user *user, t_IRCMessage cmd) {

    std::cout << "AWAY" << std::endl;
    // Verifie que le user est enregistre
    if (user->Get_username().empty()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
        return;
    }

    // Verifie les arguments de AWAY
    // Si l'utilisateur ne passe pas de parametre, l'indicateur d'absence est supprime
    if (cmd.params.empty()) {
        user->Set_mode("-a");
        user->Get_mode().Set_away_reply("");
        _Output_client(user->Get_fd_client(), RPL_UNAWAY(_name_serveur, user->Get_nickname()));
        return;
    }

    // Si l'utilisateur passe un parametre, le message d'absence est celui passe en parametre
    if (cmd.params[0].at(0) != ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "AWAY"));
        return;
    }
    std::string away_msg = Join(cmd.params, 0, cmd.params.size());
    away_msg = away_msg.substr(1, away_msg.size());
    user->Set_mode("+a");
    user->Get_mode().Set_away_reply(away_msg);
    _Output_client(user->Get_fd_client(), RPL_NOWAWAY(_name_serveur, user->Get_nickname()));
};

void server::Users_cmd(user *user, t_IRCMessage cmd)
{

    // Verifie que le user est enregistre
    if (user->Get_username().empty() || !user->Get_mode().Get_operator())
    {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
        return;
    }

    // Verifie les arguments de USERS
    if (cmd.params.empty())
    {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "USERS"));
        return;
    }

    std::string check_serveur = cmd.params[0];
    if (check_serveur != _name_serveur)
    {
        _Output_client(user->Get_fd_client(), ERR_NOSUCHSERVER(_name_serveur, check_serveur));
        return;
    }

    // Affiche la liste des utilisateurs connectes
    _Output_client(user->Get_fd_client(), "Connected on " + _name_serveur + ":");
    for (size_t i = 0; i < _list_user.size(); i++)
    {
        _Output_client(user->Get_fd_client(), "\n***\n-" + _list_user[i]->Get_username() + "\n-" + _list_user[i]->Get_hostname() + "\n-" + _list_user[i]->Get_realname() + "\n-" + _list_user[i]->Get_mode().Print_mode() + "\n***");
    }
};

void server::Wallops_cmd(user *user, t_IRCMessage cmd)
{

    // Verifie que le user est enregistre
    if (user->Get_username().empty() || !user->Get_mode().Get_operator())
    {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
        return;
    }

    // Verifie les arguments de WALLOPS
    if (cmd.params.empty())
    {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "WALLOPS"));
        return;
    }

    // FIXME : A corriger, c'est pas clair ce que la commande doit faire ?
    // FIXME : A corriger, c'est pas clair ce que la commande doit faire ?
    // Envoie le message a tous les utilisateurs connectes avec le mode +w
    std::string message = cmd.params[0];
    for (size_t i = 1; i < cmd.params.size(); i++) {
        message += " " + cmd.params[i];
    }
    if (message[0] != ':')
    {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "WALLOPS"));
        return;
    }
    message = message.substr(1, message.size());
    for (size_t i = 0; i < _list_user.size(); i++)
   
    {
        if (_list_user[i]->Get_mode().Get_wallops())
       
        {
            _Output_client(_list_user[i]->Get_fd_client(), "WALLOPS: " + message);
        }
    }
};

void server::Pong_cmd(user *user, t_IRCMessage cmd)
{
    std::cout << "PONG" << std::endl;

    // Verifie les arguments de PONG
    if (cmd.params.empty())
    {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PONG"));
        return;
    }

    std::string pong = cmd.params[0];
    std::cout << ":" + user->Get_nickname() + " PONG " + user->Get_nickname() + " :" + pong + "\r\n"
              << std::endl;
};
