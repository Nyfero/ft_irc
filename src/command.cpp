# include "../class/server.hpp"
# include "../class/user.hpp"

void    server::Check_command(user *user, std::string str) {
    std::cout << "*** sserver::Check_command + ***" << std::endl;

    if (str[0] == ':') { // :nick!user@host COMMAND
        if (!Check_prefix(user, str)) {
            return;
        }
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
        _Output_client(user->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur));
        return false;
    }

    // Verifie si le prefix contient un ! apres le nickname
    size_t pos = prefix.find("!");
    std::cout << "pos = " << pos << std::endl;
    std::cout << "prefix.size() = " << prefix.size() << std::endl;
    if (pos == std::string::npos) {
        if (pos == prefix.size() - 1) {
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

    /*
    **  TO DO
    **  Attention ircsi envoie une string et non un int
    */
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
    user->Set_mode(Stoi(check_mode));

    _Output_client(user->Get_fd_client(), RPL_WELCOME(_name_serveur, user->Get_realname(), user->Get_username(), user->Get_hostname()));
    //_Output_client(user->Get_fd_client(), "New user registered as: " + check_username + "\nHostname: " + check_hostname + "\nMode: " + check_mode + "\nRealname: " + check_realname);
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
    std::cout << "COMMANDE -> MODE" << std::endl;
    (void) cmd;
    (void) user;
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

void    server::Privmsg_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> PRIVMSG" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Notice_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> NOTICE" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Away_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> AWAY" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Users_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> USERS" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Wallops_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> WALLLOPS" << std::endl;
    (void) cmd;
    (void) user;
};
