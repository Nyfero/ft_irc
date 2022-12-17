# include "../class/server.hpp"

void    server::Check_command(user *user, std::string str) {
    std::cout << "*** server::Check_command + ***" << std::endl;

    std::string command;
    if (str[0] == ':') { // :nick!user@host COMMAND
        command = str.substr(str.find(" ") + 1, str.size());
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
        std::cerr << ERR_NEEDMOREPARAMS << std::endl;
        return;
    }
    if (cmd[0] == ':') {
        std::cerr << ERR_PASSWITHPREFIX << std::endl;
        return;
    }

    std::string check_password = cmd.substr(pos, cmd.length());
    if (user->Get_is_register()) { // Verifie si le user est deja connecte
        std::cerr << ERR_ALREADYREGISTRED << std::endl;
    }
    else if (check_password != _password) { // Verifie le mot de passe
        std::cerr << ERR_PASSWDMISMATCH << std::endl;
    }
    else { // Connecte le user et envoie un message de bienvenue
        std::cout << "New user connected on " << user->Get_fd_client() << std::endl;
        _Output_client(user->Get_fd_client(), "Welcome to the IRC server");
        user->Set_is_register(true);
    }
};

void server::User_cmd(user *user, std::string cmd) {
    std::cout << "COMMANDE -> USER" << std::endl;
    (void) cmd;
    (void) user;
};

void    server::Nick_cmd(user *user, std::string cmd) {

    //Verifie les arguments de NICK
    size_t pos = cmd.find_first_not_of(" ", 4);
    if (pos == std::string::npos) {
        std::cerr << ERR_NONICKNAMEGIVEN << std::endl;
        return;
    }

    std::string check_nick = cmd.substr(pos, cmd.length());

    if (check_nick.length() > 9 || check_nick.length() < 1) { // Verifie la longueur du nick
        std::cerr << ERR_ERRONEUSNICKNAME << std::endl;
        return;
    }

    for (size_t i = 0; i < check_nick.length(); i++) { // Verifie si le nick contient des caracteres speciaux
        if (!isalnum(check_nick[i]) && check_nick[i] != '-' && check_nick[i] != '_') {
            std::cerr << ERR_ERRONEUSNICKNAME << std::endl;
            return;
        }
    }

    for (size_t i = 0; i < _list_user.size(); i++) { // Verifie si le nick est deja pris
        if (Compare_case_sensitive(_list_user[i]->Get_username(), check_nick)) {
            std::cerr << ERR_NICKNAMEINUSE << std::endl;
            return;
        }
    }

    if (!user->Get_is_register()) {
        std::cerr << ERR_RESTRICTED << std::endl;
    }
    else if (!user->Get_username().empty()) { // Modifie le nickname
        user->Set_username(check_nick);
    }
    else {  // Definit le nickname
        user->Set_username(check_nick);
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