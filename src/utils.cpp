#include "../class/utils.hpp"

int parsing(int argc, char **argv) {


    /*
    **  3 Arg
    */

    if (argc != 3) {
        std::cerr << "The executable will run as follows:\n\t./ircserv <port> <password>" << std::endl;
        return 1;
    }

    /*
    **  Port only contain number (0 - 9)
    **  Port's range (1024 - 65535) 16-bits
    */
   
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

    /*
    **  password lenght: 1 - 12 char
    **  only 0-9 a-z A-Z
    */
   
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
    std::cout << "res: " << res << std::endl;
    return res;
};

bool    User_in_channel(user *use, channel *chan)
{
    // chan exist && user exist
    std::vector<user *> list_user;

    list_user = chan->Get_list_channel_user();
    size_t  i = 0;
    std::vector<user *>::iterator it = list_user.begin();
    while (i < list_user.size()){
        if (list_user[i]->Get_fd_client() == use->Get_fd_client())
            return 1;
        i++;
        it++;
    }
    return 0;
}

bool   User_in_channel_is_op(user *use, channel *chan)
{
    // chan exist && user exist
    std::vector<user *> list_user;

    list_user = chan->Get_list_operator();
    size_t  i = 0;
    std::vector<user *>::iterator it = list_user.begin();
    while (i < list_user.size()){
        if (list_user[i]->Get_fd_client() == use->Get_fd_client())
            return 1;
        i++;
        it++;
    }
    return 0;
}