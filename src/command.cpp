#include "../class/server.hpp"
#include "../class/utils.hpp"
#include "../class/user.hpp"

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
    while (i < 17)
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
        Nick_cmd(user, msg); // Add Restricted
        return 0;
    case 3:
        Mode_cmd(user, msg);
        return 0;
    case 4:
        return (Quit_cmd(user, msg));
    case 5:
        Join_cmd(user, msg); // Add Restricted
        return 0;
    case 6:
        Part_cmd(user, msg);
        return 0;
    case 7:
        Names_cmd(user, msg);
        return 0;
    case 8:
        Invite_cmd(user, msg); // Add Restricted
        return 0;
    case 9:
        Kick_cmd(user, msg); // Add Restricted
        return 0;
    case 10:
        Privmsg_cmd(user, msg); // Add Restricted
        return 0;
    case 11:
        Notice_cmd(user, msg); // Add Restricted
        return 0;
    case 12:
        Away_cmd(user, msg);
        return 0;
    case 13:
        Users_cmd(user, msg);
        return 0;
    case 14:
        Wallops_cmd(user, msg); // Add Restricted
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

    if (isRestricted(user))
    {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, sender->Get_nickname()));
        return ;
    } 
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
    if (isRestricted(user))
    {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, sender->Get_nickname()));
        return ;
    }
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

void server::Names_cmd(user *user, t_IRCMessage cmd)
{
    std::cout << "COMMANDE -> NAMES" << std::endl;
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
void server::Invite_cmd(user *sender, t_IRCMessage cmd)
{
    std::cout << "COMMANDE -> INVITE" << std::endl;

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

void server::Kick_cmd(user *sender, t_IRCMessage cmd)
{ // jgour
    if (isRestricted(sender))
    {
        _Output_client(sender->Get_fd_client(), ERR_RESTRICTED(_name_serveur, sender->Get_nickname()));
        return ;
    } 
    std::cout << "COMMANDE -> KICK" << std::endl;
    (void)cmd;
    (void)sender;
};

/* TODO :
  - Check if /msg #chan,nick
  - Check away message + send message anyway
  
  - Check for correct ERR MESSAGE
  - Handle ERR_TOOMANYTARGETS (how could this happen actually ?)
*/
void server::Privmsg_cmd(user *sender, t_IRCMessage cmd)
{
    std::cout << "COMMANDE -> PRIVMSG" << std::endl;

    if (isRestricted(sender))
    {
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

void server::Notice_cmd(user *sender, t_IRCMessage cmd)
{
    std::cout << "COMMANDE -> NOTICE" << std::endl;

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

    std::cout << "AWAY" << std::endl;
    // Verifie que le user est enregistre
    if (user->Get_username().empty() || isRestricted(user))
    {
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

void server::Wallops_cmd(user *sender, t_IRCMessage cmd)
{
    std::cout << "COMMANDE -> WALLOPS" << std::endl;

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

void server::Pong_cmd(user *user, t_IRCMessage cmd)
{
    std::cout << "PONG" << std::endl;

    if (cmd.params.empty())
    {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PONG"));
        return;
    }
    std::string pong = "PONG " + cmd.params[0];
    std::cout << ":" + user->Get_nickname() + " :" + pong + "\r\n"
              << std::endl;
    _Output_client(user->Get_fd_client(), pong);
};

void server::Oper_cmd(user *user, t_IRCMessage cmd) {
    
    std::cout << "OPER" << std::endl;
    std::cout << "Admin password : " << _admin_password << std::endl;
    // Verifie que le user est enregistre
    if (user->Get_username().empty()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED(_name_serveur, user->Get_nickname()));
        return;
    }

    // Verifie les arguments de OPER
    if (cmd.params.size() < 2) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "OPER"));
        return;
    }

    // Verifie que le nom d'utilisateur et le mot de passe sont corrects
    std::string username = cmd.params[0];
    if (username != user->Get_username().substr(0, user->Get_username().find(' ', 0)) && username != user->Get_nickname()) {
        _Output_client(user->Get_fd_client(), ERR_NOOPERHOST(_name_serveur));
        return;
    }

    std::string password = cmd.params[1];
    if (password != _admin_password) {
        _Output_client(user->Get_fd_client(), ERR_PASSWDMISMATCH(_name_serveur));
        return;
    }

    // Si tout est correct, l'utilisateur devient operateur
    user->Set_mode("+o");
    _Output_client(user->Get_fd_client(), RPL_YOUREOPER(_name_serveur, user->Get_nickname()));
};
