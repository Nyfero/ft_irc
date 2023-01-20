#include "../class/utils.hpp"

int server::Check_command(user *user, std::string str)
{
    std::cout << "Check_command: " << str << std::endl;

    t_IRCMessage msg = split_message(user, str);

    std::cout << "command: " << msg.command << std::endl;
    std::cout << "prefix: " << msg.prefix << std::endl;
    for (size_t i = 0; i < msg.params.size(); i++) {
        std::cout << "params[" << i << "]: " << msg.params[i] << std::endl;
    }

    std::string list_command[17] = {"PASS", "USER", "NICK", "MODE", "QUIT", "JOIN", "PART", "NAMES", "INVITE", "KICK", "PRIVMSG", "NOTICE", "AWAY", "USERS", "WALLOPS", "PING", "OPER"};

    int i = 0;
    while (i < 17) {
        if (msg.command == list_command[i]) {
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

    _Output_client(user->Get_fd_client(), RPL_WELCOME(_name_serveur, user->Get_realname(), user->Get_username(), user->Get_hostname()));
};

void server::Nick_cmd(user *user, t_IRCMessage cmd) {

    // Verifie que le user a rentrer le mot de passe
    if (!user->Get_login_status()) {
        _Output_client(user->Get_fd_client(), ERR_NOLOGIN(_name_serveur, ""));
        return;
    }
    if (user->Get_login_status() != 3) {
        user->Set_login_status(2);
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

    // Verifie si le nick est deja pris
    for (size_t i = 0; i < _list_user.size(); i++) {
        if (Compare_case_sensitive(_list_user[i]->Get_nickname(), cmd.params[0])) {
            _Output_client(user->Get_fd_client(), ERR_NICKNAMEINUSE(_name_serveur, user->Get_nickname()));
            return;
        }
    }

    // Modifie le nickname ou le definit
    if (!user->Get_username().empty()) {
        user->Set_nickname(cmd.params[0]);
        _Output_client(user->Get_fd_client(), "Your nickname is now " + cmd.params[0]);
    }
    else {
        
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
    if (cmd.params[0].empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "MODE"));
        return;
    }

    // check if chan or user

    // Verifie que le premier parametre est le nickname ou le realname
    //change (mode channel a implementer)
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
                user->Remove_Channel(chan);
                chan->Remove_user(user);
                _Output_client(user->Get_fd_client(), ":" + user->Get_nickname() + " PART " + chan->Get_channel_name()+ " :");
                if (chan->Get_list_channel_user().empty()) {
                // si chan vide degager
                    _Remove_channel(chan);
                }
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

    (void)cmd;
    (void)user;
};

/*  
TODO :   
    Parse (Check enough param == 2
    Check is a Nickname, Check Inviter is part of channel, Check dest is already in the channel
    Check if channel exists, if not create
    Check if privilege needed
    Check if user is away or Invite user
    Send REPLY INVITE to ender */
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
    if (_parse_invite(sender, cmd))         // Parse INVITE command
        return;
    user *target_nick = _check_nick_invite(sender, cmd); 
    if (target_nick == NULL)                // Check that the param 1 : Nickname is correct
        return;
    channel *target_chan = _check_chan_invite(sender, cmd);
    if (target_chan == NULL)
        return;
    if (_user_already_member(target_nick, target_chan))
    {
		_Output_client(sender->Get_fd_client(), ERR_USERONCHANNEL(_name_serveur, target_nick->Get_nickname(), target_chan->Get_channel_name()));
    }
};

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
    (void)cmd;
    (void)sender;
};

/* TODO :
  - Check if /msg #chan,nick
  - Check away message + send message anyway
  
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
