#include "../class/utils.hpp"

/*********************/
/****** PARSING ******/
/*********************/

int parsing(int argc, char **argv) {

   // 3 Arguments
    if (argc != 3) {
        std::cerr << "The executable will run as follows:\n\t./ircserv <port> <password>" << std::endl;
        return 1;
    }

    // Port only contain number (0 - 9)
    // Port's range (1024 - 65535) 16-bits
    std::string valid_port(argv[1]);
    if (!isNumber(valid_port)) {
        std::cerr << "The port must be an int" << std::endl;
        return 1;
    }
    int range_port = std::atoi(argv[1]);
    if (range_port < 1024 || range_port > 65535) {
        std::cerr << "The port's range must be between 1024 and 65535" << std::endl;
        return 1;
    }

    // password lenght: 1 - 12 char
    // only 0-9 a-z A-Z
    std::string password(argv[2]);
    if (password.length() < 1 || password.length() > 12) {
        std::cerr << "Password's lenght must be between 1 and 12" << std::endl;
        return 1;
    }
    for(size_t i = 0; i < password.length(); i++) {
		if (!isalnum(password[i])) {
			std::cerr << "Password doesn't contains only alphanumerics characters :\n(0 - 9)\t(a - z)\t(A - Z)" << std::endl;
			return 1;
		}
    }
    return 0;
};

bool isNumber(const std::string& str) {
    for (size_t i = 0; i < (str.size()); i++) {
        if (std::isdigit(str[i]) == 0)
            return false;
    }
    return true;
};

bool Compare_case_sensitive(std::string str1, std::string str2) {
    if (str1.size() != str2.size())
        return false;
    for (size_t i = 0; i < str1.size(); i++) {
        if (tolower(str1[i]) != tolower(str2[i]))
            return false;
    }
    return true;
};

int Stoi(std::string str) {
    int res = 0;
    for (size_t i = 0; i < str.size(); i++) {
        res = res * 10 + (str[i] - '0');
    }
    return res;
};

std::string Itos(int nb) {
    std::string rev;

    while ((nb / 10) > 0) {
        rev += (nb % 10) + '0';
        nb = nb / 10;
    }
    rev += (nb % 10) + '0';

    std::string res;
    for (int i = rev.size(); i >= 0; i--) {
        res += rev[i];
    }

    return res;
}



/**********************/
/****** CHANNELS ******/
/**********************/

bool    User_in_channel(user *use, channel *chan) {
    // chan exist && user exist
    std::vector<user *> list_user;

    list_user = chan->Get_list_channel_user();
    size_t  i = 0;
    std::vector<user *>::iterator it = list_user.begin();
    while (i < list_user.size()) {
        if (list_user[i]->Get_fd_client() == use->Get_fd_client()) {
            return 1;
        }
        i++;
        it++;
    }
    return 0;
};

user    *Get_user_in_channel(user *use, channel *chan) {
    // chan exist && user exist
    std::vector<user *> list_user;

    list_user = chan->Get_list_channel_user();
    size_t  i = 0;
    std::vector<user *>::iterator it = list_user.begin();
    while (i < list_user.size()) {
        if (list_user[i]->Get_fd_client() == use->Get_fd_client()) {
            return list_user[i];
        }
        i++;
        it++;
    }
    return NULL;
};

bool   User_in_channel_is_op(user *use, channel *chan) {
    // chan exist && user exist
    std::vector<user *> list_user;

    list_user = chan->Get_list_operator();
    size_t  i = 0;
    std::vector<user *>::iterator it = list_user.begin();
    while (i < list_user.size()) {
        if (list_user[i]->Get_fd_client() == use->Get_fd_client()) {
            return 1;
        }
        i++;
        it++;
    }
    return 0;
};



/**********************/
/****** COMMANDE ******/
/**********************/

// Creer et rempli une structure_IRCMessage
t_IRCMessage split_message(user *user, std::string& input) {
    t_IRCMessage msg;

    msg.prefix = ":" + user->Get_nickname() + "!" + user->Get_username() + "@" + user->Get_hostname();

    // Extract the command and parameters
    std::stringstream ss(input);
    ss >> msg.command;
    while (ss) {
        std::string param;
        ss >> param;
        if (!param.empty()) {
            msg.params.push_back(param);
        }
    }

    return msg;
}

std::vector<std::string> Split(std::string str, char delimiter) {
    std::vector<std::string> res;

    for (size_t i = 0; i < str.size(); i++) {
        std::string tmp;
        while (str[i] != delimiter && i < str.size()) {
            tmp += str[i];
            i++;
        }
        res.push_back(tmp);
    }

    return res;
}

std::string Join(std::vector<std::string> vec, size_t start, size_t end) {
    std::string res;

    for (size_t i = start; i < end; i++) {
        res += vec[i];
        if (i != vec.size() - 1)
            res += ' ';
    }

    return res;
}

bool    isRestricted(user *sender)
{
    if(sender->Get_mode()->Get_restricted())
        return true;
    return false;
}

std::string Create_names_rpl(channel *chan) {
    std::string names;
    std::vector<user *> v_user;

    v_user = chan->Get_list_channel_user();
    for (size_t i = 0; i < v_user.size(); i++) {
        if (v_user[i]->Is_op_channel(chan)) {
            names += "@";
        }
        names += v_user[i]->Get_nickname();
        names += " ";
    }
    return names;
};

std::string Create_aff_mode(std::string str, char c) {
    for (size_t j = 0; j < str.size(); j++) {
        if (c == str[j]) {
            break;
        }
        if (j + 1 == str.size()) {
            str += c;
        }
    }
    return str;
};