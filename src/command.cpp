# include "../class/server.hpp"

void    skip_cmd()
{

}

void    server::Check_command(user *user, std::string str) {
    std::cout << "*** server::Check_command + ***" << std::endl;

    std::string command;
    if (str[0] == ':') { // :nick!user@host COMMAND
        command = str.substr(str.find(" ") + 1, str.size());    // remove :nick!user@host
        command = command.substr(0, command.find(" "));         // remove arguments
    }
    else { // COMMAND
        command = str.substr(0, str.find(" "));
    }

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

void  server::Pass_cmd(user *user, std::string cmd) {

    // Verifie les arguments de PASS
    size_t pos = cmd.find_first_not_of(" ", 4);
    if (pos == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS);
        return;
    }
    if (cmd[0] == ':') {
        _Output_client(user->Get_fd_client(), ERR_PASSWITHPREFIX);
        return;
    }

    std::string check_password = cmd.substr(pos, cmd.length());
    if (user->Get_is_register()) { // Verifie si le user est deja connecte
        _Output_client(user->Get_fd_client(), ERR_ALREADYREGISTRED);
    }
    else if (check_password != _password) { // Verifie le mot de passe
        _Output_client(user->Get_fd_client(), ERR_PASSWDMISMATCH);
    }
    else { // Connecte le user et envoie un message de bienvenue
        std::cout << "New user connected on " << user->Get_fd_client() << std::endl;
        _Output_client(user->Get_fd_client(), "Welcome to the IRC server");
        user->Set_is_register(true);
    }
};

void server::User_cmd(user *user, std::string cmd) {
    (void)cmd;
    (void)user;
};

void    server::Nick_cmd(user *user, std::string cmd) {

    // Verifie s'il y a un prefix
    if (cmd[0] == ':') {
        if (!user->Get_is_register() || user->Get_nickname().empty() || user->Get_username().empty()) { // Verifie si le user est deja connecte
            _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
            return;
        }
        std::string prefix = cmd.substr(1, cmd.find(" ") - 1);
        std::string check_old_nick = prefix.substr(0, prefix.find("!"));
        if (check_old_nick != user->Get_nickname()) {   // Verifie si le prefix correspond au nickname du user
            _Output_client(user->Get_fd_client(), ERR_NICKCOLLISION);
            return;
        }
        size_t pos = prefix.find("!");
        if (pos == std::string::npos) { // Verifie si le prefix contient un ! apres le nickname
            _Output_client(user->Get_fd_client(), ERR_NICKCOLLISION);
            return;
        }
        size_t pos2 = prefix.find("@");
        if (pos2 == std::string::npos) { // Verifie si le prefix contient un @ apres le username
            _Output_client(user->Get_fd_client(), ERR_NICKCOLLISION);
            return;
        }
        std::string check_user = prefix.substr(pos + 1, pos2 - pos - 1);
        if (check_user != user->Get_username()) { // Verifie si le prefix correspond au username du user
            _Output_client(user->Get_fd_client(), ERR_NICKCOLLISION);
            return;
        }
        std::string check_host = prefix.substr(pos2 + 1, prefix.size());
        if (check_host != user->Get_hostname()) { // Verifie si le prefix correspond au hostname du user
            _Output_client(user->Get_fd_client(), ERR_NICKCOLLISION);
            return;
        }
        cmd = cmd.substr(cmd.find(" ") + 1, cmd.size());
    }

    //Verifie les arguments de NICK
    size_t pos = cmd.find_first_not_of(" ", 4);
    if (pos == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NONICKNAMEGIVEN);
        return;
    }

    std::string check_nick = cmd.substr(pos, cmd.length());

    if (check_nick.length() > 9 || check_nick.length() < 1) { // Verifie la longueur du nick
        _Output_client(user->Get_fd_client(), ERR_ERRONEUSNICKNAME);
        return;
    }

    for (size_t i = 0; i < check_nick.length(); i++) { // Verifie si le nick contient des caracteres speciaux
        if (!isalnum(check_nick[i]) && check_nick[i] != '-' && check_nick[i] != '_') {
            _Output_client(user->Get_fd_client(), ERR_ERRONEUSNICKNAME);
            return;
        }
    }

    for (size_t i = 0; i < _list_user.size(); i++) { // Verifie si le nick est deja pris
        if (Compare_case_sensitive(_list_user[i]->Get_username(), check_nick)) {
            _Output_client(user->Get_fd_client(), ERR_NICKNAMEINUSE);
            return;
        }
    }

    if (!user->Get_is_register()) {
        _Output_client(user->Get_fd_client(), ERR_RESTRICTED);
    }
    else if (!user->Get_username().empty()) { // Modifie le nickname
        user->Set_username(check_nick);
        _Output_client(user->Get_fd_client(), "Your nickname is now " + check_nick);
    }
    else {  // Definit le nickname
        user->Set_username(check_nick);
        std::cout << "Introducing new " << check_nick << " user" << std::endl;
        _Output_client(user->Get_fd_client(), "Hello " + check_nick + " !");
    }
};

void    server::Mode_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> MODE" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Quit_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> QUIT" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Join_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> JOIN" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Part_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> PART" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Names_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> NAMES" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Invite_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> INVITE" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Kick_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> KICK" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Privmsg_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> PRIVMSG" << std::endl;
    (void) user;

    // Skip la command & check si il y a des arguments
    size_t pos = cmd.find_first_not_of(" ", 7);
    if (pos == std::string::npos) {
        _Output_client(user->Get_fd_client(), ERR_NEEDMOREPARAMS);
        return;
    }

    // Check si la target est valide
    std::string target;
    target = cmd.substr(pos, cmd.find(" "));
    // Check if target is good (useror channel)

    // Check if message is "ok" (lenght, prefix?)

    // Send message

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