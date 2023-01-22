#include "../class/utils.hpp"

int server::Check_command(user *user, std::string str)
{
    std::cout << std::endl << "Check_command: " << str << std::endl;

    t_IRCMessage msg = split_message(user, str);

    // std::cout << "command: " << msg.command << std::endl;
    // std::cout << "prefix: " << msg.prefix << std::endl;
    // for (size_t i = 0; i < msg.params.size(); i++) {
    //     std::cout << "params[" << i << "]: " << msg.params[i] << std::endl;
    // }

    std::string list_command[18] = {"PASS", "USER", "NICK", "MODE", "QUIT", "JOIN", "PART", "NAMES", "INVITE", "KICK", "PRIVMSG", "NOTICE", "AWAY", "USERS", "WALLOPS", "PING", "OPER", "TOPIC"};

    int i = 0;
    while (i < 18)
    {
        if (msg.command == list_command[i])
        {
            break;
        }
        i++;
    }

    switch (i) {

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
    
    case 16:
        Oper_cmd(user, msg);
        return 0;
    case 17:
        Topic_cmd(user, msg);
        return 0;
    default:
        std::cout << "*** server::Check_command - ***" << std::endl;
        break;
    }
    return 0;
};

void server::Pass_cmd(user *user, t_IRCMessage cmd) {

    // Verifie si le user est deja connecte
    if (user->Get_login_status()) { 
        _Output_client(user->Get_fd_client(), ERR_ALREADYREGISTRED(_name_serveur));
        return;
    }

    // Verifie les arguments de PASS
    if (cmd.params.empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PASS"));
        return;
    }

    // Verifie le mot de passe
    if (cmd.params[0] != _password) { 
        _Output_client(user->Get_fd_client(), ERR_PASSWDMISMATCH(_name_serveur));
        return;
    }
    
    // Connecte le user et envoie un message de bienvenue
    user->Set_login_status(1);
    std::cout << "New user connected on " << user->Get_fd_client() << std::endl;
    _Output_client(user->Get_fd_client(), "Welcome to the IRC server");

};

void server::User_cmd(user *user, t_IRCMessage cmd) {

    // Verifie que le user a deja set son nickname
    if (user->Get_login_status() != 2) {
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
    if (cmd.params[3].empty() || cmd.params[3].at(0) != ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "USER"));
        return;
    }

    std::string realname = Join(cmd.params, 3, cmd.params.end() - cmd.params.begin());
    realname = realname.substr(1, realname.size());

    // Enregistre le user
    user->Set_login_status(3);
    user->Set_username(cmd.params[0]);
    user->Set_hostname(cmd.params[2]);
    user->Set_realname(realname);
    if (!isNumber(cmd.params[1]))
        user->Set_mode("0");
    else
        user->Set_mode(cmd.params[1]);

    _Output_client(user->Get_fd_client(), RPL_WELCOME(_name_serveur, user->Get_nickname(), user->Get_username(), user->Get_hostname()));
};

void server::Nick_cmd(user *user, t_IRCMessage cmd) {

    // Verifie que le user a rentrer le mot de passe
    if (!user->Get_login_status()) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    // Verifie si le user n'est pas restreind
    if (isRestricted(user)) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
        return ;
    } 

    // Verifie les arguments de NICK
    if (cmd.params[0].empty()) {
        _Output_client(user->Get_fd_client(), ERR_NONICKNAMEGIVEN(_name_serveur));
        return;
    }

    // Verifie la longueur du nickname
    if (cmd.params[0].length() > 9 || cmd.params[0].length() < 1 ) {
        _Output_client(user->Get_fd_client(), ERR_ERRONEUSNICKNAME(_name_serveur, user->Get_nickname()));
        _Output_client(user->Get_fd_client(), "The nickname must contain at least 1 and a maximum of 9 characters");
        return;
    }

    // Verifie si le nick contient des caracteres speciaux
    for (size_t i = 0; i < cmd.params[0].length(); i++) {
        if (!isalnum(cmd.params[0][i]) && cmd.params[0][i] != '-' && cmd.params[0][i] != '_') {
            _Output_client(user->Get_fd_client(), ERR_ERRONEUSNICKNAME(_name_serveur, user->Get_nickname()));
            _Output_client(user->Get_fd_client(), "The nickname must not contain special characters (except - and _)");
            return;
        }
    }

    if (!user->Get_username().empty()) {
        // Verifie si le nick est deja pris
        for (size_t i = 0; i < _list_user.size(); i++) {
            if (Compare_case_sensitive(_list_user[i]->Get_nickname(), cmd.params[0])) {
                _Output_client(user->Get_fd_client(), ERR_NICKNAMEINUSE(_name_serveur, user->Get_nickname()));
                return;
            }
        }
        _Output_client(user->Get_fd_client(), ":" + user->Get_nickname() + " NICK :" + cmd.params[0]);
        user->Set_nickname(cmd.params[0]);
    }
    else {
        // Verifie si le nick est deja pris
        bool name_changed = false;
        for (size_t i = 0; i < _list_user.size(); i++) {
            if (Compare_case_sensitive(_list_user[i]->Get_nickname(), cmd.params[0])) {
                cmd.params[0] += '_';
                i = 0;
                name_changed = true;
            }
        }
        user->Set_login_status(2);
        user->Set_nickname(cmd.params[0]);
        std::cout << "Introducing new " << cmd.params[0] << " user" << std::endl;
        _Output_client(user->Get_fd_client(), "Hello " + cmd.params[0] + " !");
    }
};

void server::Mode_cmd(user *user, t_IRCMessage cmd) {

    // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    // Verifie les arguments de MODE
    if (cmd.params.size() == 1) {
        // _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "MODE"));
        return;
    }

    // check if chan
    if (cmd.params[0].at(0) == '!' || cmd.params[0].at(0) == '#' || cmd.params[0].at(0) == '&' || cmd.params[0].at(0) == '+') {

        // Verifie si le channel existe
        channel *chan = _Channel_already_exist(cmd.params[0]);
        if (!chan) {
            _Output_client(user->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, user->Get_nickname(), cmd.params[0]));
            return;
        }

        // Verifie s'il y a un mode
        if (cmd.params[1].empty()) {
            _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "MODE"));
            return;
        }

        // Verifie le si le mode existe
        if (cmd.params[1].at(0) == '+') {
            if (cmd.params[1].at(1) == 'i') {
                if (!user->Get_mode().Get_operator() && !user->Is_op_channel(chan)) { // ajout chan op : && user->Is_op_channel(chan)
                    _Output_client(user->Get_fd_client(), ERR_CHANOPRIVSNEEDED(_name_serveur, cmd.params[0]));
                    return;
                }
                if (Compare_case_sensitive(chan->Get_channel_name(), cmd.params[0])) {
                    chan->Set_invite_only(true);
                    _Output_channel(chan, RPL_CHANNELMODEIS(user->Get_nickname(), chan->Get_channel_name(), "+i"));
                    return;
                }
            }
            else if (cmd.params[1].at(1) == 't') {
                if (!user->Get_mode().Get_operator() && !user->Is_op_channel(chan)) {
                    _Output_client(user->Get_fd_client(), ERR_CHANOPRIVSNEEDED(_name_serveur, cmd.params[0]));
                    return;
                }
                if (Compare_case_sensitive(chan->Get_channel_name(), cmd.params[0])) {
                    chan->Set_topic_settable(false);
                    _Output_channel(chan, RPL_CHANNELMODEIS(user->Get_nickname(), chan->Get_channel_name(), "+t"));
                    return;
                }
            }
            else if (cmd.params[1].at(1) == 'o') {
                if (!user->Get_mode().Get_operator() && !user->Is_op_channel(chan)) {
                    _Output_client(user->Get_fd_client(), ERR_CHANOPRIVSNEEDED(_name_serveur, cmd.params[0]));
                    return;
                }
                if (Compare_case_sensitive(chan->Get_channel_name(), cmd.params[0])) {
                    if (cmd.params[2].empty()) {
                        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "MODE"));
                        return;
                    }

                    _Output_channel(chan, RPL_CHANNELMODEIS(user->Get_nickname(), chan->Get_channel_name(), "+o"));
                    return;
                }
            }
            else {
                // _Output_client(user->Get_fd_client(), ERR_UNKNOWNMODE(_name_serveur, cmd.params[1]));
                _Output_client(user->Get_fd_client(), cmd.prefix + " PRIVMSG " + chan->Get_channel_name() + " : Unknown MODE flag" ); 
                return;
            }
        }
        else if (cmd.params[1].at(0) == '-') {
            if (cmd.params[1].at(1) == 'i') {
                if (!user->Get_mode().Get_operator() && !user->Is_op_channel(chan)) {
                    _Output_client(user->Get_fd_client(), ERR_CHANOPRIVSNEEDED(_name_serveur, cmd.params[0]));
                    return;
                }
                if (Compare_case_sensitive(chan->Get_channel_name(), cmd.params[0])) {
                    chan->Set_invite_only(false);
                    _Output_channel(chan, RPL_CHANNELMODEIS(user->Get_nickname(), chan->Get_channel_name(), "-i"));
                    return;
                }
            }
            else if (cmd.params[1].at(1) == 't') {
                if (!user->Get_mode().Get_operator() && !user->Is_op_channel(chan)) {
                    _Output_client(user->Get_fd_client(), ERR_CHANOPRIVSNEEDED(_name_serveur, cmd.params[0]));
                    return;
                }
                if (Compare_case_sensitive(chan->Get_channel_name(), cmd.params[0])) {
                    chan->Set_topic_settable(true);
                    _Output_channel(chan, RPL_CHANNELMODEIS(user->Get_nickname(), chan->Get_channel_name(), "-t"));
                    return;
                }
            }
            else {
                _Output_client(user->Get_fd_client(), ERR_UNKNOWNMODE(_name_serveur, cmd.params[1]));
                return;
            }
        }
        else {
            _Output_client(user->Get_fd_client(), ERR_UNKNOWNMODE(_name_serveur, cmd.params[1]));
            return;
        }
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

int server::Quit_cmd(user *user, t_IRCMessage cmd) {
    // to-do
    //  definir message a envoyer
    (void)user;
    (void)cmd;
    return -2;
};

void server::Join_cmd(user *user, t_IRCMessage cmd) {
    
     // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    if (isRestricted(user)) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
        return ;
    }

    std::vector<std::string>    chan;
    std::vector<std::string>    key;

    if (cmd.params.empty()) { //vide
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, user->Get_nickname()));
        return;
    }
    else if (cmd.params.size() == 1 && cmd.params[0] == "0") {
        // join 0
        std::vector<channel*>    l_chan = user->Get_channel_register();
        while (!l_chan.empty())
        {
            // mettre msg
            user->Remove_Channel(l_chan[0]);
            l_chan[0]->Remove_user(user);
            l_chan = user->Get_channel_register();
        }
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
};

void server::Part_cmd(user *user, t_IRCMessage cmd) {
    
    // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    std::vector<std::string> v_chan;
    channel *chan;

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
                std::string prefix = user->Get_nickname() + "!" + user->Get_username() + "@" + user->Get_hostname();
                _Output_channel(chan,  ":" + prefix + " PART " + chan->Get_channel_name());
                user->Remove_Channel(chan);
                chan->Remove_user(user);
                // _Output_client(user->Get_fd_client(), user->Get_nickname() + " PART " + chan->Get_channel_name() + " :");

                // _Output_channel(chan,  user->Get_nickname() + " PART " + chan->Get_channel_name() + " :");
                if (chan->Get_list_channel_user().empty())// chan vide donc le delete
                    _Remove_channel(chan); 
                //:jgourlin!jgourlin@localhost PART #qw :
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

void server::Names_cmd(user *user, t_IRCMessage cmd) {
    
    // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    // Verifie si l'utilisateur est restreind
    if (isRestricted(user)) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
        return;
    }

    // Verifie les arguments de NAMES
    if (cmd.params.empty()) {
        for (size_t i = 0; i < user->Get_channel_register().size(); i++) {
            for (size_t j = 0; j < user->Get_channel_register().at(i)->Get_list_channel_user().size(); j++) {
                if (user->Get_channel_register().at(i)->Get_list_channel_user().at(j)->Get_nickname() != user->Get_nickname()) {
                    _Output_client(user->Get_fd_client(), RPL_NAMREPLY(_name_serveur, user->Get_nickname(),user->Get_channel_register().at(i)->Get_channel_name(), user->Get_channel_register().at(i)->Get_list_channel_user().at(j)->Get_nickname()));
                }
            }
            _Output_client(user->Get_fd_client(), RPL_ENDOFNAMES(_name_serveur, user->Get_nickname(), user->Get_channel_register().at(i)->Get_channel_name()));
        }
    }
    else {
        for (size_t i = 0; i < user->Get_channel_register().size(); i++) {
            if (cmd.params[0] == user->Get_channel_register().at(i)->Get_channel_name()) {
                for (size_t j = 0; j < user->Get_channel_register().at(i)->Get_list_channel_user().size(); j++) {
                    if (user->Get_channel_register().at(i)->Get_list_channel_user().at(j)->Get_nickname() != user->Get_nickname()) {
                        _Output_client(user->Get_fd_client(), RPL_NAMREPLY(_name_serveur, user->Get_nickname(),user->Get_channel_register().at(i)->Get_channel_name(), user->Get_channel_register().at(i)->Get_list_channel_user().at(j)->Get_nickname()));
                    }
                _Output_client(user->Get_fd_client(), RPL_ENDOFNAMES(_name_serveur, user->Get_nickname(), user->Get_channel_register().at(i)->Get_channel_name()));
                }
            }
        }
    }
};

/*  
TODO :   
    - Ajouter un if sender is operator si le channel est en mode invite only

    - Check other error messages
*/
void server::Invite_cmd(user *sender, t_IRCMessage cmd) {
    
    // Verifie que le user est enregistre
    if (sender->Get_login_status() != 3) {
        _Output_client(sender->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }
    if (isRestricted(sender))
    {
        _Output_client(sender->Get_fd_client(), ERR_RESTRICTED(_name_serveur, sender->Get_nickname()));
        return ;
    } 
    if (_parse_invite(sender, cmd))             // Parse INVITE command
        return;
    user *target_nick = _check_nick_invite(sender, cmd); 
    if (target_nick == NULL)                    // Check that the param 1 : Nickname is correct
        return;
    channel *target_chan = _check_chan_invite(sender, cmd);
    if (target_chan == NULL)
        return;
    if (_user_already_member(target_nick, target_chan))
    {
		_Output_client(sender->Get_fd_client(), ERR_USERONCHANNEL(_name_serveur, sender->Get_nickname(), target_chan->Get_channel_name(), target_nick->Get_nickname()));
        return;
    }
    _invite_success(sender, target_nick, cmd);                // Send success invite messages
};

/* 
TODO : ERR_CHANOPRIVSNEEDED (pas de flag operator) | 
*/
void server::Kick_cmd(user *sender, t_IRCMessage cmd) {
    
    // Verifie que le user est enregistre
    if (sender->Get_login_status() != 3) {
        _Output_client(sender->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }
    if (isRestricted(sender))
    {
        _Output_client(sender->Get_fd_client(), ERR_RESTRICTED(_name_serveur, sender->Get_nickname()));
        return ;
    }
    if (_parse_kick(sender, cmd))                                                           // Parse KICK command
        return; 
    user *target_nick = _check_nick_kick(sender, cmd);                                      // Check that the nickname parameter is correct
    if (target_nick == NULL)                                                                // Check that the param 1 : Nickname is correct
        return;
    std::vector<channel *> channels_kick = _split_channels_kick(sender, cmd);               // Split channel paramater (#chan1,#chan2,#chan3) & check if its correct
    for (size_t i = 0; i < channels_kick.size(); i++)
    {
        channels_kick[i] = _user_not_in_channel(sender, target_nick, channels_kick[i]);     // Check if user target is in channel
        if (_kick_from_channel(target_nick, channels_kick[i]))
            _kick_success_message(target_nick, channels_kick[i], cmd);
    }
};

/* TODO :
  - Check if /msg #chan,nick
  - Check away message
  
  - Check for correct ERR MESSAGE
  - Handle ERR_TOOMANYTARGETS (how could this happen actually ?)
*/
void server::Privmsg_cmd(user *sender, t_IRCMessage cmd) {

    // Verifie que le user est enregistre
    if (sender->Get_login_status() != 3) {
        _Output_client(sender->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    if (isRestricted(sender)) {
        _Output_client(sender->Get_fd_client(), ERR_RESTRICTED(_name_serveur, sender->Get_nickname()));
        return ;
    } 
    if (_parse_privmsg(sender, cmd))                                                // Parse PRIVMSG command
        return;
    std::vector<std::string> target = _target_handle(cmd);                          // Create a vector of targets
    std::vector<int> targets_fds = _targetfds_creator_privmsg(sender, target);      // Check targets and put all target fds in a vector
    std::string message = _create_msg(cmd);                                         // Merge all parameters in one Message String

    std::vector<int>::iterator dest;
    for (dest = targets_fds.begin(); dest != targets_fds.end(); dest++)
    {
        _Output_client(*dest, message);
        std::string success_msg = "Message sent to ";
        std::cout << success_msg << *dest << std::endl;
    }
};

void server::Notice_cmd(user *sender, t_IRCMessage cmd) {

    // Verifie que le user est enregistre
    if (sender->Get_login_status() != 3) {
        _Output_client(sender->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    if (isRestricted(sender))
    {
        _Output_client(sender->Get_fd_client(), ERR_RESTRICTED(_name_serveur, sender->Get_nickname()));
        return ;
    }
    if (_parse_notice_wallops(cmd))                                                 // Parse PRIVMSG command
        return;
    std::vector<std::string> target = _target_handle(cmd);                          // Create a vector of targets
    std::vector<int> targets_fds = _targetfds_creator_notice(sender, target);       // Check targets and put all target fds in a vector
    std::string message = _create_msg(cmd);                                         // Merge all parameters in one Message String

    std::vector<int>::iterator dest;
    for (dest = targets_fds.begin(); dest != targets_fds.end(); dest++)
    {
        _Output_client(*dest, message);
        std::string success_msg = "Notice sent to ";
        std::cout << success_msg << *dest << std::endl;
    }
};

void server::Away_cmd(user *user, t_IRCMessage cmd) {

    // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    // Verifie les arguments de AWAY
    // Si l'utilisateur ne passe pas de parametre, 2 cas:
    // - L'utilisateur est deja AWAY -> il ne l'est plus
    // - L'utilisateur n'est pas AWAY -> il devient AWAY avec aucun message
    if (cmd.params.empty()) {
        user->Get_mode().Set_away_reply("");
        if (user->Get_mode().Get_away()) {
            user->Set_mode("-a");
            _Output_client(user->Get_fd_client(), RPL_UNAWAY(_name_serveur, user->Get_nickname()));
        }
        else {
            user->Set_mode("+a");
            _Output_client(user->Get_fd_client(), RPL_NOWAWAY(_name_serveur, user->Get_nickname()));
        }
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

void server::Users_cmd(user *user, t_IRCMessage cmd) {

    // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    // Verifie que le user est operateur
    if (!user->Get_mode().Get_operator())
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

void server::Wallops_cmd(user *sender, t_IRCMessage cmd) {
    
     // Verifie que le user est enregistre
    if (sender->Get_login_status() != 3) {
        _Output_client(sender->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    if (isRestricted(sender))
    {
        _Output_client(sender->Get_fd_client(), ERR_RESTRICTED(_name_serveur, sender->Get_nickname()));
        return ;
    }
    if (_parse_notice_wallops(cmd))                                                 // Parse WALLOPS command
        return;
    if (sender->Get_mode().Get_operator() == false)                                 // Check is user is an Operator
    {
        std::cout << "ERROR : User is not OPERATOR" << std::endl;
        return;
    }
    std::vector<std::string> target = _target_handle(cmd);                           // Create a vector of targets
    std::vector<int> targets_fds = _targetfds_creator_wallops(sender, target);       // Check targets and put all target fds in a vector
    std::string message = _create_msg(cmd);                                          // Merge all parameters in one Message String

    std::vector<int>::iterator dest;
    for (dest = targets_fds.begin(); dest != targets_fds.end(); dest++)
    {
        _Output_client(*dest, message);
        std::string success_msg = "Wallops sent to ";
        std::cout << success_msg << *dest << std::endl;
    }
};

/* Si ca bug c'est car le meme irssi est connecte sur plusieurs instances */
void server::Pong_cmd(user *user, t_IRCMessage cmd) {

     // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    if (cmd.params.empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PONG"));
        return;
    }
    std::string pong = "PONG " + cmd.params[0];
    std::cout << ":" + user->Get_nickname() + " :" + pong + "\r\n" << std::endl;
    _Output_client(user->Get_fd_client(), pong);
};

void server::Oper_cmd(user *user, t_IRCMessage cmd) {
    
    // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

    // Verifie que le user n'est pas restreind
    if (isRestricted(user)) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
        return ;
    }

    // Verifie les arguments de OPER
    if (cmd.params.size() < 2) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "OPER"));
        return;
    }

    // Verifie que le nom d'utilisateur est correct
    if (cmd.params[0] != user->Get_username().substr(0, user->Get_username().find(' ', 0)) && cmd.params[0] != user->Get_nickname()) {
        _Output_client(user->Get_fd_client(), ERR_NOOPERHOST(_name_serveur));
        return;
    }

    // Verifie que le mot de passe est correct
    if (cmd.params[1] != _admin_password) {
        _Output_client(user->Get_fd_client(), ERR_PASSWDMISMATCH(_name_serveur));
        return;
    }

    // Si tout est correct, l'utilisateur devient operateur
    user->Set_mode("+o");
    _Output_client(user->Get_fd_client(), RPL_YOUREOPER(_name_serveur, user->Get_nickname()));
};

void server::Topic_cmd(user *user, t_IRCMessage cmd) {

    // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }

//     Command: TOPIC
//    Parameters: <channel> [ <topic> ]

//    The TOPIC command is used to change or view the topic of a channel.
//    The topic for channel <channel> is returned if there is no <topic>
//    given.  If the <topic> parameter is present, the topic for that
//    channel will be changed, if this action is allowed for the user
//    requesting it.  If the <topic> parameter is an empty string, the
//    topic for that channel will be removed.

        // ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
        // RPL_NOTOPIC                     RPL_TOPIC
        // ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES

//  Examples:

//    :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic
//      ; User Wiz setting the topic.

//    TOPIC #test :another topic
//      ; Command to set the topic on #test to "another topic".

//    TOPIC #test :
//      ; Command to clear the topic on #test.

//    TOPIC #test
//      ; Command to check the topic for #test.

    // 1 seul chan a la fois

    // size == 1: afficher topic du channel
    //      peut etre utiliser sans etre dans le chan

    // size == 2: changer topic du channel
    //      doit etre chans le chan
    //      tous les membres peuvent le changer

    //params[0] == channel
    //params [1+] == message, retirer ':'
    channel     *chan;
    std::string new_topic;

    if (cmd.params.empty()) { // ERR_NEEDMOREPARAMS 
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "TOPIC"));
        return ;
    }
    if (!(chan = _Channel_already_exist(cmd.params[0])))
    {
        std::cout << "chan no found" << std::endl;// print del
        //#qwe: No such channel
        return ;
    }
    std::cout << "topic: " << chan->Get_channel_topic() << std::endl;// print del

    if (cmd.params.size() == 1) // print chan's topic
    {
        std::cout << "TOPIC: 1 arg" << std::endl;// print del
        std::cout << cmd.params[0] << std::endl;// print del
        if (chan->Get_channel_topic().empty()) { //RPL_NOTOPIC
            _Output_client(user->Get_fd_client(), RPL_NOTOPIC(_name_serveur, chan->Get_channel_name())); // check si correct
        }
        else { //RPL_TOPIC
            _Output_client(user->Get_fd_client(), RPL_TOPIC(_name_serveur, chan->Get_channel_name(), chan->Get_channel_topic()));
        }
    }
    else if (cmd.params.size() >= 2) //change chan's topic
    {

        std::cout << "TOPIC: 2+ arg" << std::endl; // print del
        for (size_t i = 0; i < cmd.params.size(); i++){// print del
            std::cout << cmd.params[i] << std::endl;// print del
        }// print del

        if (!_User_is_in_chan(user, chan)) { // ERR_NOTONCHANNEL : User not in channel
            _Output_client(user->Get_fd_client(), ERR_NOTONCHANNEL(_name_serveur, chan->Get_channel_name()));
            return ;
        }
        if (!user->Get_mode().Get_operator() && chan->Get_topic_settable() == false && !user->Is_op_channel(chan)) { // ERR_NOCHANMODES : Need rights and user doesn't have
            _Output_client(user->Get_fd_client(),ERR_NOCHANMODES(_name_serveur, chan->Get_channel_name()));
            return ;
        }
        for (size_t i = 1; i < cmd.params.size(); i++) { // create new_topic
            new_topic += cmd.params[i];
            if (i + 1 < cmd.params.size())
                new_topic += " ";
        }
        new_topic.erase(new_topic.begin()); // delete ':' 
        std::cout << new_topic << std::endl; // print del
        chan->Mod_topic(new_topic);
        if (new_topic.empty())
            _Output_channel(chan, cmd.prefix + " TOPIC " + chan->Get_channel_name() + " :");
        else
            _Output_channel(chan, cmd.prefix + " TOPIC " + chan->Get_channel_name() + " :" + new_topic);
    }
};

void server::List_cmd(user *user, t_IRCMessage cmd) {
    (void) cmd;
    // Verifie que le user est enregistre
    if (user->Get_login_status() != 3) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }


};