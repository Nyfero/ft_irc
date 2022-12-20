# include "../class/server.hpp"
# include "../class/user.hpp"

void    server::Check_command(user *user, std::string str) {
    std::cout << "*** sserver::Check_command + ***" << std::endl;

    if (str[0] == ':') { // :nick!user@host COMMAND
        // if (!Check_prefix(user, str)) {
        //     return;
        // }
        // Supprime le prefix de la commande
        str = str.substr(str.find(" ") + 1, str.size());
    }
    
    std::string command = str.substr(0, str.find(" "));
    std::string list_command[15] = {"PASS", "USER", "NICK", "MODE", "QUIT", "JOIN", "PART", "NAMES", "INVITE", "KICK", "PRIVMSG", "NOTICE", "AWAY", "USERS", "WALLOPS"};

    int i = 0;
    while (i < 15) {
        if (command == list_command[i]) {
            break;
        }
        i++;
    }
    switch (i) {
    case 0: 
        return Pass_cmd(user, str);
    
    case 1:
        return User_cmd(user, str);

    case 2:
        return Nick_cmd(user, str);

    case 3:
        return Mode_cmd(user, str);

    case 4:
        return Quit_cmd(user, str);

    case 5:
        return Join_cmd(user, str);
    
    case 6:
        return Part_cmd(user, str);
    
    case 7:
        return Names_cmd(user, str);

    case 8:
        return Invite_cmd(user, str);
    
    case 9:
        return Kick_cmd(user, str);
    
    case 10:
        return Privmsg_cmd(user, str);
    
    case 11:
        return Notice_cmd(user, str);

    case 12:
        return Away_cmd(user, str);
    
    case 13:
        return Users_cmd(user, str);

    case 14:
        return Wallops_cmd(user, str);
    
    default:
        std::cout << "*** server::Check_command - ***" << std::endl;
        break;
    }
};

bool    server::Check_prefix(user *user, std::string str) {

    // Verifie si le user est deja connecte
    if (user->Get_username().empty()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
        return false;
    }

    // Verifie si le prefix correspond au nickname du user
    std::string prefix = str.substr(1, str.find(" ") - 1);
    std::string check_nick = prefix.substr(0, prefix.find("!"));
    if (check_nick != user->Get_nickname()) {
        _Output_client(user->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur, check_nick));
        return false;
    }

    // Verifie si le prefix contient un ! apres le nickname
    size_t pos = prefix.find("!");
    if (pos == std::string::npos) {
        if (pos + 1 == 0) {
            return true;
        }
        _Output_client(user->Get_fd_client(), ERR_PARSINGPREFIX);
        return false;
    }

    // Verifie si le prefix contient un @ apres le username
    size_t pos2 = prefix.find("@");
    if (pos2 == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_PARSINGPREFIX);
        return false;
    }

    // Verifie si le prefix correspond au username du user
    std::string check_user = prefix.substr(pos + 1, pos2 - pos - 1);
    if (check_user != user->Get_username()) {
        _Output_client(user->Get_fd_client(), ERR_NOSUCHUSER);
        return false;
    }

    // Verifie si le prefix correspond au hostname du user
    std::string check_host = prefix.substr(pos2 + 1, prefix.size());
    if (check_host != user->Get_hostname()) {
        _Output_client(user->Get_fd_client(), ERR_NOSUCHHOSTNAME);
        return false;
    }
    
    return true;
};

void  server::Pass_cmd(user *user, std::string cmd) {

    // Verifie les arguments de PASS
    size_t pos = cmd.find_first_not_of(" ", 4);
    if (pos == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    std::string check_password = cmd.substr(pos, cmd.length());
    if (user->Get_is_register()) { // Verifie si le user est deja connecte
        _Output_client(user->Get_fd_client(), ERR_ALREADYREGISTRED(_name_serveur));
    }
    else if (check_password != _password) { // Verifie le mot de passe
        _Output_client(user->Get_fd_client(), ERR_PASSWDMISMATCH(_name_serveur));
    }
    else { // Connecte le user et envoie un message de bienvenue
        std::cout << "New user connected on " << user->Get_fd_client() << std::endl;
        _Output_client(user->Get_fd_client(), "Welcome to the IRC server");
        user->Set_is_register(true);
    }
};

void server::User_cmd(user *user, std::string cmd) {
    
    // Verifie si le user est connecte
    if (!user->Get_is_register() || user->Get_nickname().empty()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
        return;
    }

    // Verifie si le user a deja envoye une commande USER
    if (!user->Get_username().empty()) {
        _Output_client(user->Get_fd_client(), ERR_ALREADYREGISTRED(_name_serveur));
        return;
    }

    // Verifie les arguments de USER
    size_t pos = cmd.find_first_not_of(" ", 4);
    if (pos == std::string::npos || cmd[pos] == ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    // Verifie si le username est vide
    std::string check_username = cmd.substr(pos, cmd.find(" ", pos) - pos);
    if (check_username.empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    // Verifie le mode
    size_t pos2 = cmd.find_first_not_of(" ", pos + check_username.length());
    if (pos2 == std::string::npos || cmd[pos2] == ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }
    std::string check_mode = cmd.substr(pos2, cmd.find(" ", pos2) - pos2);
    if (isNumber(check_mode)) {
        if (Stoi(check_mode) < 0 || Stoi(check_mode) > 15) {
            _Output_client(user->Get_fd_client(), ERR_UMODEUNKNOWNFLAG(_name_serveur));
            return;
        }
    }

    // Verifie le hostname
    size_t pos3 = cmd.find_first_not_of(" ", pos2 + check_mode.length());
    if (pos3 == std::string::npos || cmd[pos3] == ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }
    std::string check_hostname = cmd.substr(pos3, cmd.find(" ", pos3) - pos3);
    if (check_hostname.empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    // Verifie le realname
    size_t pos4 = cmd.find_first_not_of(" ", pos3 + check_hostname.length());
    if (pos4 == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }
    if (cmd[pos4] != ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }
    std::string check_realname = cmd.substr(pos4 + 1, cmd.length() - pos4);
    if (check_realname.empty()) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    // Enregistre le user
    user->Set_username(check_username);
    user->Set_hostname(check_hostname);
    user->Set_realname(check_realname);
    if (!isNumber(check_mode)) {
        check_mode = "0";
    }
    user->Set_mode(check_mode);

    _Output_client(user->Get_fd_client(), RPL_WELCOME(_name_serveur, user->Get_realname(), user->Get_username(), user->Get_hostname()));
};

void    server::Nick_cmd(user *user, std::string cmd) {

    //Verifie les arguments de NICK
    size_t pos = cmd.find_first_not_of(" ", 4);
    if (pos == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NONICKNAMEGIVEN(_name_serveur));
        return;
    }

     // Verifie la longueur du nickname
    std::string check_nick = cmd.substr(pos, cmd.length());
    if (check_nick.length() > 9 || check_nick.length() < 1) {
        _Output_client(user->Get_fd_client(), ERR_ERRONEUSNICKNAME(_name_serveur, user->Get_nickname()));
        return;
    }

    // Verifie si le nick contient des caracteres speciaux
    for (size_t i = 0; i < check_nick.length(); i++) {
        if (!isalnum(check_nick[i]) && check_nick[i] != '-' && check_nick[i] != '_') {
            _Output_client(user->Get_fd_client(), ERR_ERRONEUSNICKNAME(_name_serveur, user->Get_nickname()));
            return;
        }
    }

    // Verifie si le nick est deja pris
    for (size_t i = 0; i < _list_user.size(); i++) {
        if (Compare_case_sensitive(_list_user[i]->Get_nickname(), check_nick)) {
            _Output_client(user->Get_fd_client(), ERR_NICKNAMEINUSE(_name_serveur, user->Get_nickname()));
            return;
        }
    }

    if (!user->Get_is_register()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
    }
    else if (!user->Get_username().empty()) { // Modifie le nickname
        user->Set_nickname(check_nick);
        _Output_client(user->Get_fd_client(), "Your nickname is now " + check_nick);
    }
    else {  // Definit le nickname
        user->Set_nickname(check_nick);
        std::cout << "Introducing new " << check_nick << " user" << std::endl;
        _Output_client(user->Get_fd_client(), "Hello " + check_nick + " !");
    }
};

void    server::Mode_cmd(user *user, std::string cmd) {

    // Verifie que le user est enregistre
    if (user->Get_username().empty()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
        return;
    }
    
    // Verifie les arguments de MODE
    size_t pos = cmd.find_first_not_of(" ", 4);
    if (pos == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    // Verifie le nickname
    std::string check_nick = cmd.substr(pos, cmd.find(" ", pos) - pos);
    std::string tmp = user->Get_realname().substr(0, user->Get_realname().find(" ", 0));
    if (check_nick != user->Get_nickname() && check_nick != tmp) {
        _Output_client(user->Get_fd_client(), ERR_USERSDONTMATCH(_name_serveur));
        return;
    }

    // Verifie si on affiche le mode ou si on le modifie
    pos = cmd.find_first_not_of(" ", pos + check_nick.length());
    if (pos == std::string::npos) {
         _Output_client(user->Get_fd_client(), user->Get_mode().Print_mode());
        return;
    }

    // Verifie qu'il n'y a qu'un mode en parametre
    std::string check_mode = cmd.substr(pos, cmd.find(" ", pos) - pos);
    if (check_mode.length() > 2) {
        _Output_client(user->Get_fd_client(), ERR_TOMUCHPARAMS);
        return;
    }

    // Verifie que le prefix du mode est correct
    if (check_mode[0] != '+' && check_mode[0] != '-') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    // Verifie que le mode est correct
    if (check_mode[1] == 'i' || check_mode[1] == 'w' || check_mode[1] == 's') {
        user->Set_mode(check_mode);
        _Output_client(user->Get_fd_client(), "Mode change [" + check_mode + "] for " + user->Get_nickname());
    }
    else if (check_mode[1] == 'o' || check_mode[1] == 'O' || check_mode[1] == 'a') {
        if (check_mode[0] == '-') { // Ignore le mode +o et +O
            user->Set_mode(check_mode);
            _Output_client(user->Get_fd_client(), "Mode change [" + check_mode + "] for " + user->Get_nickname());
        }
        else if (check_mode[1] == 'a' && check_mode[0] == '+') { // L'utilisateur doit utiliser AWAY
            _Output_client(user->Get_fd_client(), ERR_USEAWAY);
        }
    }
    else if (check_mode[1] == 'r') {
        if (check_mode[0] == '+') { // Ignore le mode -r
            user->Set_mode(check_mode);
            _Output_client(user->Get_fd_client(), "Mode change [" + check_mode + "] for " + user->Get_nickname());
        }
    }
    else {
        _Output_client(user->Get_fd_client(), ERR_UMODEUNKNOWNFLAG(_name_serveur));
    }

};

void    server::Quit_cmd(user *user, std::string cmd) { //jgour
    std::cout << "COMMANDE -> QUIT" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Join_cmd(user *user, std::string cmd) { //jgour
    std::cout << "COMMANDE -> JOIN" << std::endl;

    (void) user;
    (void) cmd;

    std::string chan, key;
    size_t  pos, pos2;

    pos = cmd.find_first_not_of(" ", 4);
    if (pos == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }
    chan = cmd.substr(pos, cmd.size()); //lpha
    pos = chan.find(" ");
    if (((pos = chan.find(" ")) != std::string::npos) && (pos2 = chan.find_first_not_of(" ", pos))!= std::string::npos)
    {
        key = chan.substr(pos2, chan.size()); //lpha
        chan = chan.substr(0, pos);
    }

    std::cout << "chan = " << chan << std::endl;
    if (!key.empty())
    {
        std::cout << "key = " << key << std::endl;
        pos = key.find(" ", 0);
        if (pos != std::string::npos && (pos2 = key.find_first_not_of(" ", pos)) != std::string::npos){
            _Output_client(user->Get_fd_client(), ERR_TOMUCHPARAMS);
            return;
        }
        _Join_treat(user, chan, key);
        std::cout << "JOIN <channel> <key>" << std::endl;

    }
    else if (!chan.compare("0"))// 1 arg && 0
    {
        user->Remove_all_channel();
    }
    else // 1 arg
    {
        std::cout << "JOIN <channel>" << std::endl;
        _Join_treat(user, chan);
    }
};

void    server::Part_cmd(user *user, std::string cmd) { //gjour
    std::cout << "COMMANDE -> PART" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Names_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> NAMES" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Invite_cmd(user *user, std::string cmd) { // jgoru
    std::cout << "COMMANDE -> INVITE" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Kick_cmd(user *user, std::string cmd) { // jgour
    std::cout << "COMMANDE -> KICK" << std::endl;
    (void) cmd;
    (void) user;
};

/* TODO : 
  - Check if User is restricted/muted in the channel -> ERR_CANNOTSENDTOCHAN	
  - Handle wildcard and send message to mask with a wild-card for a top level domain + ERR_WILDTOPLEVEL + ERR_NOTOPLEVEL
  - Handle ERR_TOOMANYTARGETS (how could this happen actually ?)
  - Improve message sent back to user
  - Handle AWAY response if user mode a
*/
void    server::Privmsg_cmd(user *sender, std::string cmd) {
    std::cout << "COMMANDE -> PRIVMSG" << std::endl;
    (void) sender;

    /*                       Skip la command & check si il y a des arguments                       */
    size_t pos = cmd.find_first_not_of(" ", 7);
    if (pos == std::string::npos) {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    /*                                        Handle Targets                                        */
    // Check if there are any target specified
    if(cmd.at(pos) == ':')
      _Output_client(sender->Get_fd_client(), ERR_NORECIPIENT(_name_serveur));

    // Create vector of targets
    std::vector<std::string> target;
    size_t end_otarget;
    end_otarget = cmd.find(" ", pos);
    target.push_back(cmd.substr(pos, end_otarget - pos));
    
    std::stringstream targstream(target[0]);
    std::string newtarget;
    while (std::getline(targstream, newtarget, ',')) {
      target.push_back(newtarget);
    }
    target.erase(target.begin());

    /*                        Check if targets are valid & add to fd to send                        */
    std::vector<int> targets_fds;
    for (std::vector<std::string>::iterator it = target.begin() ; it != target.end(); ++it)
    {
      
      // CHECK if target is a Channel
      if ((*it)[0] == '#') {

        // Check if channel Exist
        for (size_t i = 0; i < _list_channel.size(); i++) {
          std::string target_channel = _list_channel[i]->Get_channel_name();  
          if (Compare_case_sensitive(target_channel, *it)) {

            // Check is user is in the channel
            if (User_in_channel(sender, _list_channel[i])) {
              std::vector<user *> channel_users = _list_channel[i]->Get_list_channel_user();
              
              // Add all channel_users_fd, but don't copy doublons (fd already present in target_fds)
              for (size_t i = 0; i < channel_users.size(); i++) {
                if (IsInTargetFds(channel_users[i]->Get_fd_client(), targets_fds) == false)
                  targets_fds.push_back(channel_users[i]->Get_fd_client());
              }
            }
            else {
              std::string chan_name = _list_channel[i]->Get_channel_name();
              _Output_client(sender->Get_fd_client(), ERR_CANNOTSENDTOCHAN(_name_serveur, chan_name));
            }
          }
          else if (i + 1 == _list_channel.size())
            _Output_client(sender->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, *it));
        }
      }
      else {
      
        // Check if target is user 
        for (size_t i = 0; i < _list_user.size(); i++) {

          // Check if username exists
          if (Compare_case_sensitive(_list_user[i]->Get_nickname(), *it)) {
            
            // Add user_fd, but don't copy doublons (fd already present in target_fds)
            if (IsInTargetFds(_list_user[i]->Get_fd_client(), targets_fds) == false)
              targets_fds.push_back(_list_user[i]->Get_fd_client());
          }
          else if (i + 1 == _list_user.size())
            _Output_client(sender->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur, *it));
        }
      }
    }

    /*                             Parse Message (exist, lenght, prefix?)                             */
    // Check if a message is specified
    size_t strt_omsg = cmd.find_first_not_of(" ", end_otarget);
    if (strt_omsg == std::string::npos) {
      _Output_client(sender->Get_fd_client(), ERR_NOTEXTTOSEND(_name_serveur));
      return ;
    }

    // Check if message start with ':'
    if (cmd.at(strt_omsg) != ':') {
      _Output_client(sender->Get_fd_client(), ERR_UNVALIDMSG(_name_serveur));
      return ;        
    }

    // Create message
    std::string msg = cmd.substr(strt_omsg, cmd.size() - strt_omsg);

    /*                                           Send message                                           */
    std::vector<int>::iterator dest;
    for (dest = targets_fds.begin(); dest != targets_fds.end(); dest++)
      _Output_client(*dest, msg);

    // Notify message well sent
    _Output_client(sender->Get_fd_client(), "Message has been successfully sent");
};

void    server::Notice_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> NOTICE" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Away_cmd(user *user, std::string cmd) {

    // Verifie que le user est enregistre
    if (user->Get_username().empty()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
        return;
    }
    
    // Verifie les arguments de AWAY
    // Si l'utilisateur ne passe pas de parametre, le message d'absence est vide
    size_t pos = cmd.find_first_not_of(" ", 4);
    if (pos == std::string::npos) {
        user->Set_mode("+a");
        user->Get_mode().Set_away_reply("");
        _Output_client(user->Get_fd_client(), "Changed away status to away with no message");
        return;
    }

    // Si l'utilisateur passe un parametre, le message d'absence est celui passe en parametre
    std::string away = cmd.substr(pos, cmd.size());
    if (away[0] != ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }
    away = away.substr(1, away.size());
    user->Set_mode("+a");
    user->Get_mode().Set_away_reply(away);
    _Output_client(user->Get_fd_client(), "Changed away status to away with message: " + away);
};

void    server::Users_cmd(user *user, std::string cmd) {

    // Verifie que le user est enregistre
    if (user->Get_username().empty() || !user->Get_mode().Get_operator()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
        return;
    }

    // Verifie les arguments de USERS
    size_t pos = cmd.find_first_not_of(" ", 5);
    if (pos == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    std::string check_serveur = cmd.substr(pos, cmd.size());
    if (check_serveur != _name_serveur) {
        _Output_client(user->Get_fd_client(), ERR_NOSUCHSERVER(_name_serveur));
        return;
    }

    // Affiche la liste des utilisateurs connectes
    _Output_client(user->Get_fd_client(), "Connected on " + _name_serveur + ":");
    for (size_t i = 0; i < _list_user.size(); i++) {
         _Output_client(user->Get_fd_client(), "\n***\n-" + _list_user[i]->Get_username() + "\n-" + _list_user[i]->Get_hostname() + "\n-" + _list_user[i]->Get_realname() + "\n-" + _list_user[i]->Get_mode().Print_mode() + "\n***");
    }
};

void    server::Wallops_cmd(user *user, std::string cmd) {
    
    // Verifie que le user est enregistre
    if (user->Get_username().empty()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
        return;
    }

    // Verifie les arguments de WALLOPS
    size_t pos = cmd.find_first_not_of(" ", 7);
    if (pos == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }

    // Envoie le message a tous les utilisateurs connectes avec le mode +w
    std::string message = cmd.substr(pos, cmd.size());
    if (message[0] != ':') {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur));
        return;
    }
    message = message.substr(1, message.size());
    for (size_t i = 0; i < _list_user.size(); i++) {
        if (_list_user[i]->Get_mode().Get_wallops()) {
            _Output_client(_list_user[i]->Get_fd_client(), "WALLOPS: " + message);
        }
    }
};
